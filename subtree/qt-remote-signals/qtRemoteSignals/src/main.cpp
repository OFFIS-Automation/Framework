// OFFIS Automation Framework
// Copyright (C) 2013-2014 OFFIS e.V.
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http:#www.gnu.org/licenses/>.

#include <QDebug>
#include <QCoreApplication>
#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QCryptographicHash>
#include <QDateTime>
#include <QFileInfo>
#include <QDir>

struct ID {
    quint64 id1;
    quint64 id2;
};

struct Parameter {
    QString name;
    QString type;
    bool isBasic;
    QString signature() const
    {
        if (isBasic)
            return QString("%1 %2").arg(type, name);
        else
            return QString("const %1& %2").arg(type, name);
    }
};

struct Method {
    QString signature() const
    {
        QString sig;
        sig += name;
        sig += "(";
        QStringList paramSigs;
        foreach (const Parameter &param, params)
            paramSigs << param.signature();
        sig += paramSigs.join(", ");
        sig += ")";
        return sig;
    }
    QStringList paramNames() const
    {
        QStringList names;
        foreach (const Parameter &param, params)
            names << param.name;
        return names;
    }

    QString returnType() const
    {
        if (returnValue.name.isEmpty())
            return "void";
        else
            return returnValue.name;
    }

    int id;
    bool isReverse;
    bool isBlocking;
    Parameter returnValue;
    QString name;
    QList<Parameter> params;
};
quint64 createId(const QList<Method> &methods)
{
    quint64 numMethods = methods.size();
    quint64 numParams = 0;
    quint64 numChars = 0;
    foreach (const Method &method, methods) {
        numChars += method.signature().length();
        numParams += method.params.size();
    }
    quint64 part1 = 100000 * numMethods + numParams;
    part1 = part1 << 32;
    return part1 | numChars;
}

Parameter createParameter(QString param)
{
    QStringList basicTypes;
    basicTypes << "int"
               << "uint"
               << "short"
               << "ushort"
               << "char"
               << "uchar"
               << "long"
               << "ulong"
               << "quint64"
               << "qint64";
    Parameter paramType;
    param = param.trimmed();
    param.replace("unsigned long long", "quint64");
    param.replace("long long", "qint64");
    param.replace("unsigned ", "u");

    int split = param.lastIndexOf(' ');
    paramType.name = param.mid(split + 1).trimmed();
    paramType.type = param.mid(0, split).trimmed();
    paramType.isBasic = basicTypes.contains(paramType.type);
    return paramType;
}

Method parseLine(const QString &line)
{
    static int id = 1;
    Method method;
    method.id = id++;
    int start = line.indexOf('(');
    int end = line.indexOf(')');
    method.name = line.mid(0, start);
    method.isReverse = false;
    method.isBlocking = false;
    if (method.name.startsWith("signal ")) {
        method.name.remove(0, 7);
    } else if (method.name.startsWith("slot ")) {
        method.isReverse = true;
        method.name.remove(0, 5);
    } else if (method.name.startsWith('-')) // DEPRECATED
    {
        method.isReverse = true;
        method.name.remove(0, 1);
    } else if (method.name.contains(" ")) {
        int space = method.name.lastIndexOf(' ');
        method.returnValue = createParameter(method.name.mid(0, space));
        method.name = method.name.mid(space + 1);
        method.isBlocking = true;
    }

    QString paramsLine = line.mid(start + 1, end - start - 1);
    QStringList params = paramsLine.split(",", QString::SkipEmptyParts);
    QString prebuffer = "";
    foreach (QString param, params) {

        param = param.trimmed();
        if (param.isEmpty())
            continue;
        param = prebuffer + param;
        prebuffer = "";
        if (param.count("<") > param.count(">")) {
            prebuffer = param + ", ";
            continue;
        }

        method.params << createParameter(param);
    }
    return method;
}

void writeMethod(QTextStream &stream, Method method, QString ns)
{
    stream << method.returnType() << " " << ns << method.signature() << endl;
    stream << "{" << endl;
    stream << "\tQByteArray msgData;" << endl;
    stream << "\tQDataStream stream(&msgData, QIODevice::WriteOnly);" << endl;
    stream << "\tstream << RemoteSignals::version() << RemoteSignals::gid1() "
              "<< RemoteSignals::gid2() << (int)" << QString::number(method.id)
           << ";" << endl;
    if (method.isBlocking) {
        stream << "\tquint64 callUid = RemoteSignals::nextCallUid();" << endl;
        stream << "\tstream << callUid;" << endl;
    }
    foreach (const Parameter &param, method.params)
        stream << "\tstream << " << param.name << ";" << endl;
    stream << "\ttransmitSignal(msgData);" << endl;
    if (method.isBlocking) {
        stream << "\tconst QByteArray responseData = "
                  "RemoteSignals::waitForResponse(callUid);" << endl;
        if (method.returnType() != "void") {
            stream << "\t" << method.returnType() << " decodedResponse;"
                   << endl;
            stream << "\tQDataStream responseStream(responseData);" << endl;
            stream << "\tresponseStream >> decodedResponse;" << endl;
            stream << "\treturn decodedResponse;" << endl;
        }
    }
    stream << "}" << endl
           << endl;
}

void writeMethodParsing(QTextStream &stream, Method method)
{
    stream << "\tif(remoteSignalMethodId == " << method.id << ") {" << endl;
    foreach (const Parameter &param, method.params)
        stream << "\t\t" << param.type << " " << param.name << ";" << endl;
    foreach (const Parameter &param, method.params)
        stream << "\t\tstream >> " << param.name << ";" << endl;
    stream << "\t\temit " << method.name << "("
           << method.paramNames().join(", ") << ");" << endl;
    stream << "\t\treturn;" << endl;
    stream << "\t}" << endl;
}

void writeImplementation(ID id, const QList<Method> &methods, QDir dir,
                         const QString &className, bool reverse)
{
    QString ns = className + "::";
    QFile file(dir.absoluteFilePath(QString("%1.cpp").arg(className)));
    file.open(QFile::WriteOnly);
    QTextStream stream(&file);
    stream << "//qtRemoteSignals auto generated file" << endl;
    stream << "//do not edit this file; if needed subclass it or create a "
              "decorator" << endl;
    stream << "#include \"" << className << ".h\"" << endl;
    stream << endl;
    if (reverse)
        stream << ns << className << "(QIODevice* writeDevice, QIODevice* "
                                     "readDevice, bool initialize)" << endl;
    else
        stream << ns << className << "(QIODevice* readDevice, QIODevice* "
                                     "writeDevice, bool initialize)" << endl;
    stream << "\t: RemoteSignals(Q_UINT64_C(0x" + QString::number(id.id1, 16) +
                  "),Q_UINT64_C(0x" + QString::number(id.id2, 16) +
                  "), readDevice, writeDevice, initialize)" << endl;
    stream << "{}" << endl
           << endl;

    foreach (const Method &method, methods) {
        if (method.isReverse != reverse)
            writeMethod(stream, method, ns);
    }
    stream << "void " << ns << "processRemoteInputs(const QByteArray& data)"
           << endl;
    stream << "{" << endl;
    stream << "\tQDataStream stream(data);" << endl;
    stream << "\tint remoteSignalVersion = 0;" << endl;
    stream << "\tquint64 remoteSignalGid1 = 0;" << endl;
    stream << "\tquint64 remoteSignalGid2 = 0;" << endl;
    stream << "\tint remoteSignalMethodId = 0;" << endl;
    stream << "\tstream >> remoteSignalVersion >> remoteSignalGid1 >> "
              "remoteSignalGid2 >> remoteSignalMethodId;" << endl;
    stream << "\tRemoteSignals::checkId(remoteSignalVersion, remoteSignalGid1, "
              "remoteSignalGid2);" << endl
           << endl;
    QStringList mBlockIds;
    foreach (const Method &method, methods) {
        if (method.isReverse == reverse)
            writeMethodParsing(stream, method);
        else if(reverse && method.isBlocking)
        {
            mBlockIds << QString::number(method.id);
        }
    }
    stream << endl;
    if(!mBlockIds.empty())
    {
        stream << "\tif(remoteSignalMethodId == ";
        stream << mBlockIds.join("\n\t\t|| remoteSignalMethodId ==") << ")" << endl;
        stream << "\t{" << endl;
        stream << "\t\tquint64 callUid;" << endl;
        stream << "\t\tstream >> callUid;" << endl;
        stream << "\t\tqint64 pos = stream.device()->pos();" << endl;
        stream << "\t\tQMutexLocker lock(&mUidMutex);" << endl;
        stream << "\t\tmResponses[callUid] = data.mid(pos);" << endl;
        stream << "\t\treturn;" << endl;
        stream << "\t}" << endl;

    }
    stream << "\tRemoteSignals::handleError(remoteSignalMethodId);" << endl;
    stream << "}" << endl;
}

void writeHeader(ID id, const QList<QString> &includes,
                 const QList<Method> &methods, QDir dir,
                 const QString &className, bool reverse)
{
    QFile file(dir.absoluteFilePath(QString("%1.h").arg(className)));
    file.open(QFile::WriteOnly);
    QTextStream stream(&file);
    stream << "//qtRemoteSignals auto generated file" << endl;
    stream << "//do not edit this file; if needed subclass it or create a "
              "decorator" << endl;
    stream << QString("//md5 hash: %1%2")
                  .arg(QString::number(id.id1, 16), QString::number(id.id2, 16))
           << endl;
    stream << "#ifndef " << className.toUpper() << "_H" << endl;
    stream << "#define " << className.toUpper() << "_H" << endl
           << endl;
    stream << "#include \"RemoteSignals.h\"" << endl;
    foreach (const QString &includeStr, includes)
        stream << "#include " << includeStr << endl;
    stream << endl;
    stream << "class " << className << " : public RemoteSignals" << endl;
    stream << "{" << endl;
    stream << "\tQ_OBJECT" << endl;
    stream << "public:" << endl;
    if (reverse)
        stream << "\t" << className << "(QIODevice* writeDevice, QIODevice* "
                                       "readDevice = 0, bool initialize = "
                                       "false);" << endl;
    else
        stream << "\t" << className << "(QIODevice* readDevice, QIODevice* "
                                       "writeDevice = 0, bool initialize = "
                                       "false);" << endl;
    stream << "signals:" << endl;
    foreach (const Method &method, methods) {
        if (method.isReverse == reverse && !method.isBlocking)
            stream << "\tvoid " << method.signature() << ";" << endl;
    }
    stream << endl;
    stream << "public slots:" << endl;
    stream << "//remote signals" << endl;
    foreach (const Method &method, methods) {
        if (method.isReverse != reverse && !method.isBlocking)
            stream << "\t" << method.returnType() << " " << method.signature()
                   << ";" << endl;
    }
    stream << endl;
    stream << "public slots:" << endl;
    stream << "// blocking methods" << endl;
    foreach (const Method &method, methods) {
        if (method.isReverse != reverse && method.isBlocking)
            stream << "\t" << method.returnType() << " " << method.signature()
                   << ";" << endl;
    }
    stream << endl;
    stream << "private:" << endl;
    stream << "\tvoid processRemoteInputs(const QByteArray& data);" << endl;
    stream << "};" << endl
           << endl;
    stream << "#endif //" << className.toUpper() << "_H" << endl;
}
void writeClass(ID id, const QList<Method> &methods,
                const QList<QString> &includes, QDir dir,
                const QString &className, bool reverse)
{
    writeHeader(id, includes, methods, dir, className, reverse);
    writeImplementation(id, methods, dir, className, reverse);
}

void writeUserClass(const QDir &folder, const QString &className)
{
    QFileInfo fileInfo(folder.absoluteFilePath(className + ".h"));
    if (!fileInfo.exists()) {
        QFile file(fileInfo.absoluteFilePath());
        file.open(QFile::WriteOnly);
        QTextStream stream(&file);
        stream << "#ifndef " + className.toUpper() + "_H" << endl;
        stream << "#define " + className.toUpper() + "_H" << endl;
        stream << "#include \"" + className + "Base.h\"" << endl
               << endl;
        stream << "class " + className + " : public " + className + "Base"
               << endl;
        stream << "{" << endl
               << "public:" << endl;
        stream << "\t" << className << "(QIODevice* writeDevice, QIODevice* "
                                       "readDevice = 0, bool initialize = "
                                       "false)" << endl;
        stream << "\t\t:" << className
               << "Base(writeDevice, readDevice, initialize) {}" << endl;
        stream << "};" << endl;
        stream << "#endif // " + className.toUpper() + "_H" << endl;
    }
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QStringList args = a.arguments();
    if (args.size() < 3) {
        QTextStream(stdout) << "Usage: RemoteSignals definitionFile "
                               "codeTargetDir [--server-only|--client-only] "
                               "[--force]" << endl;
        return 1;
    }
    QString definitionFile = args[1];
    QDateTime modifyDateDefFile = QFileInfo(definitionFile).lastModified();
    QDir targetDir(args[2]);
    QStringList params;
    for (int i = 3; i < args.size(); i++)
        params << args[i];

    QString className = QFileInfo(definitionFile).baseName();
    QFile file(definitionFile);
    file.open(QFile::ReadOnly);
    QTextStream stream(&file);
    QList<Method> methods;
    QList<QString> includes;
    QByteArray hashData;
    while (!stream.atEnd()) {
        QString line = stream.readLine().trimmed();
        if (line.isEmpty() || line.startsWith('/'))
            continue;
        hashData.append(line.toLocal8Bit());
        if (line.startsWith("#include")) {
            line.replace("#include", "");
            includes << line.trimmed();
        } else
            methods << parseLine(line);
    }
    QByteArray hash =
        QCryptographicHash::hash(hashData, QCryptographicHash::Md5).toHex();
    ID id;
    id.id1 = hash.mid(0, 15).toULongLong(0, 16);
    id.id2 = hash.mid(16).toULongLong(0, 16);
    qWarning() << "Found " << methods.size() << " methods. ID:"
               << QString("0x%1%2").arg(id.id1, 16).arg(id.id2, 16);
    file.close();
    if (!params.contains("--server-only")) {
        QString baseClassName = className + "ClientBase";
        QFileInfo headerInfo(targetDir.absoluteFilePath(baseClassName + ".h"));
        QFileInfo sourceInfo(
            targetDir.absoluteFilePath(baseClassName + ".cpp"));
        bool noUpdateNeeded = headerInfo.exists() &&
                              headerInfo.lastModified() > modifyDateDefFile;
        noUpdateNeeded &= sourceInfo.exists() &&
                          sourceInfo.lastModified() > modifyDateDefFile;
        if (noUpdateNeeded && !params.contains("--force"))
            qWarning() << "Client source and header is newer than definition "
                          "file; omiting file generation";
        else
            writeClass(id, methods, includes, targetDir, baseClassName, true);
        writeUserClass(targetDir, className + "Client");
    }
    if (!params.contains("--client-only")) {
        QString baseClassName = className + "ServerBase";
        QFileInfo headerInfo(targetDir.absoluteFilePath(baseClassName + ".h"));
        QFileInfo sourceInfo(
            targetDir.absoluteFilePath(baseClassName + ".cpp"));
        bool noUpdateNeeded = headerInfo.exists() &&
                              headerInfo.lastModified() > modifyDateDefFile;
        noUpdateNeeded &= sourceInfo.exists() &&
                          sourceInfo.lastModified() > modifyDateDefFile;
        if (noUpdateNeeded && !params.contains("--force"))
            qWarning() << "Server source and header is newer than definition "
                          "file; omiting file generation";
        else
            writeClass(id, methods, includes, targetDir, baseClassName, false);
        writeUserClass(targetDir, className + "Server");
    }

    QDir srcDir(a.applicationDirPath());
    QFile::remove(targetDir.absoluteFilePath("RemoteSignals.h"));
    QFile::remove(targetDir.absoluteFilePath("RemoteSignals.cpp"));
    QFile::copy(srcDir.absoluteFilePath("RemoteSignals.h"),
                targetDir.absoluteFilePath("RemoteSignals.h"));
    QFile::copy(srcDir.absoluteFilePath("RemoteSignals.cpp"),
                targetDir.absoluteFilePath("RemoteSignals.cpp"));
    return 0;
}
