#ifndef VERSION_H
#define VERSION_H

namespace Version
{
    static const int BUILD_VERSION_NUMBER = BUILD_VERSION;
    static const QString DATE = QString(__DATE__).simplified();
    static QString versionString()
    {
        return QObject::tr("Build: %1 (%2)").arg(Version::BUILD_VERSION_NUMBER).arg(Version::DATE);
    }
}

#endif // VERSION_H
