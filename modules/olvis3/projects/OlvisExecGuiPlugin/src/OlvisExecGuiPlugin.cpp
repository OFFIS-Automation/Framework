// OFFIS Automation Framework
// Copyright (C) 2013 OFFIS e.V.
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
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "OlvisExecGuiPlugin.h"

#include <core/OlvisInterface.h>
#include <core/ProcessorInfo.h>

#include <QSettings>
#include <QFileInfo>
#include <QtPlugin>
#include <QFile>
#include <QSettings>
#include <QXmlStreamWriter>
#include <QMessageBox>
#include <QProgressDialog>

#include "OlvisSingleton.h"

#include "MainToolBar.h"
#include "VideoDisplay/VideoWidget.h"


OlvisExecGuiPlugin::OlvisExecGuiPlugin()
{
    mInterface = 0;
    mHilec = 0;
    connect(this, SIGNAL(pluginsWriteConfigReq(QXmlStreamWriter&)), SLOT(writeXmlConfig(QXmlStreamWriter&)), Qt::DirectConnection);
    connect(this, SIGNAL(pluginsReadConfigReq(QString)), SLOT(readXmlConfig(QString)), Qt::DirectConnection);
    mTimer.setInterval(5000);
    connect(&mTimer, SIGNAL(timeout()), SLOT(autoSave()));
}

void OlvisExecGuiPlugin::setModelInterface(const QString& name, QObject* object)
{
    if(!mInterface && name == "OlvisCore")
        mInterface= static_cast<OlvisInterface*>(object);
    if(!mHilec && name == "Hilec")
        mHilec = object;
}

bool OlvisExecGuiPlugin::requirementsMet()
{
    return mInterface != 0;
}

void OlvisExecGuiPlugin::initialize(const QString& pluginDir)
{
    OlvisSingleton::setInstance(*mInterface);
    Q_UNUSED(pluginDir);
    // give only const& to gui elements
    const OlvisInterface& model = *mInterface;
    toolbar = new MainToolBar(model);
    videoWidget = new VideoWidget();
    if(mHilec)
    {
        connect(mHilec, SIGNAL(videoCaptureStartRequested(int)), videoWidget, SLOT(startCapture(int)));
        connect(mHilec, SIGNAL(videoCaptureEndRequested(QString)), videoWidget, SLOT(endCapture(QString)));
        connect(mHilec, SIGNAL(saveScreenshotRequested(QString)), videoWidget, SLOT(saveScreenshot(QString)));
    }
    connect(toolbar, SIGNAL(setNumDisplays(int)), videoWidget, SLOT(updateMaxDisplays(int)), Qt::QueuedConnection);
    connect(toolbar, SIGNAL(restoreRequested()), SLOT(restore()));
    connect(toolbar, SIGNAL(saveRequested()), SLOT(save()));
    // prototype:
    //connect(toolbar, SIGNAL(), mInterface, SLOT(), Qt::QueuedConnection);
    toolbar->setEnabled(false);
}

void OlvisExecGuiPlugin::start()
{
    /*
    // load the last graph
    QSettings settings;
    QString lastGraph = settings.value("OlvisGuiPlugin/lastGraph").toString();

    loadGraphString(lastGraph);

    // create simgle processor if no graph was loaded
    if(mInterface->getProcessors().count() == 0)
        mInterface->createProcessor("Default");
    updateRecentFiles();
    */
}

void OlvisExecGuiPlugin::deinitialize()
{
}

void OlvisExecGuiPlugin::addElements(MainWindowInterface *mainWindow)
{
    mainWindow->addToolBar(toolbar, tr("Vision"));
    mainWindow->addDockWidget(Qt::TopDockWidgetArea, videoWidget, tr("Vision"));

    QMenu* fileMenu = &mainWindow->getMenu(tr("&File"));
    toolbar->initMenu(fileMenu, &mainWindow->getMenu(tr("&Vision")));

    PerspectiveInterface& perspective = mainWindow->getPerspective(tr("Vision"));
    perspective.setCentralWidget(videoWidget, 1);
    perspective.addToolbar(toolbar);
}

void OlvisExecGuiPlugin::setGuiInterface(const QString &name, QObject *guiInterface)
{
    Q_UNUSED(name);
    Q_UNUSED(guiInterface);
}


void OlvisExecGuiPlugin::loadGraphFile(const QString &filename)
{
    QFile file(filename);
    file.open(QFile::ReadOnly | QFile::Text);
    loadGraphString(file.readAll());
}

void OlvisExecGuiPlugin::loadGraphString(const QString& data)
{
    mInterface->clear();
    mInterface->loadFromData(data);
    emit pluginsReadConfigReq(data);
}

void OlvisExecGuiPlugin::writeXmlConfig(QXmlStreamWriter &writer)
{
    writer.writeStartElement(getName());
    writer.writeStartElement("videoWidget");
    videoWidget->writeCurrentConfig(writer);
    writer.writeEndElement();
    writer.writeEndElement();
}

void OlvisExecGuiPlugin::readXmlConfig(const QString& data)
{
    QXmlStreamReader reader(data);
    reader.readNextStartElement();
    while(reader.readNextStartElement())
    {
        if(reader.name() == getName())
        {
            while (reader.readNextStartElement()) {
                if (reader.name() == "videoWidget")
                    videoWidget->readConfig(reader);
                else
                    reader.skipCurrentElement();
            }
            break;
        }
        else
            reader.skipCurrentElement();
    }
}

#ifdef Q_OS_WIN
#include <Windows.h>
#endif

void OlvisExecGuiPlugin::autoSave()
{
    if(mInterface->isRunning())
        return;
    if(autoSaveFile.isEmpty())
        return;
    if(!mInterface->testUpdateFlag())
        return;
    saveGraphFile(autoSaveFile);
    QFileInfo file(autoSaveFile);
#ifdef Q_OS_WIN
    wchar_t tmp[1024];
    autoSaveFile.toWCharArray(tmp);
    SetFileAttributes(tmp, FILE_ATTRIBUTE_HIDDEN);
#endif

}

void OlvisExecGuiPlugin::saveGraphFile(const QString &filename)
{
    QFile file(filename);
    file.open(QFile::WriteOnly | QFile::Text);
    QXmlStreamWriter writer(&file);
    writer.setAutoFormatting(true);
    writer.writeStartDocument();
    saveGraph(writer);
    writer.writeEndDocument();
}

void OlvisExecGuiPlugin::saveGraphString(QString &data)
{
    QXmlStreamWriter writer(&data);
    writer.writeStartDocument();
    saveGraph(writer);
    writer.writeEndDocument();
}

void OlvisExecGuiPlugin::saveGraph(QXmlStreamWriter &writer)
{
    writer.writeStartElement("olvisFullConfig");
    mInterface->writeCurrentConfig(&writer);
    emit pluginsWriteConfigReq(writer);
    writer.writeEndElement();
}

void OlvisExecGuiPlugin::save()
{
    if(!graphFile.isEmpty())
    {
        saveGraphFile(graphFile);
        mInterface->clearUpdateFlag();
    }
}

void OlvisExecGuiPlugin::restore()
{
    if(!graphFile.isEmpty())
    {
        loadGraphFile(graphFile);
        mInterface->clearUpdateFlag();
    }
}

void OlvisExecGuiPlugin::loadProject(const QString &projectFile)
{
    QSettings settings(projectFile, QSettings::IniFormat);
    if(!settings.contains("visionFile"))
        settings.setValue("visionFile", QFileInfo(projectFile).baseName() + ".ogr");
    QString graphName = settings.value("visionFile").toString();
    graphFile = QFileInfo(projectFile).absolutePath() + "/" + graphName;
    autoSaveFile =  QFileInfo(projectFile).absolutePath() + "/~" + graphName;

    if(QFileInfo(graphFile).lastModified() < QFileInfo(autoSaveFile).lastModified()
        &&QMessageBox::warning(0, "Auto save file detected (vision)",
                             "An automatic save file was detected that is newer than the original file. Recover auto save file?",
                             QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes) == QMessageBox::Yes)
    {
        loadGraphFile(autoSaveFile);
    }
    else
        loadGraphFile(graphFile);
    if(mInterface->getProcessors().count() == 0)
    {
        mInterface->createProcessor("Default");
        saveGraphFile(graphFile);
    }
    mInterface->clearUpdateFlag();
    toolbar->setEnabled(true);
    mTimer.start();
}

void OlvisExecGuiPlugin::closeProject()
{
    mTimer.stop();
    mInterface->stop();
    QProgressDialog dlg("Waiting for vision system to stop", "Continue", 0, 0);
    connect(mInterface,SIGNAL(executionFinished()), &dlg, SLOT(close()));
    while(!mInterface->waitForStop(1000))
    {
        dlg.exec();
    }
    if(graphFile.isEmpty())
        return;
    if(mInterface->testUpdateFlag())
    {
        if(QMessageBox::question(0, tr("Save vision configuration?"), tr("Do you want to save the vision configuration?"),
                                 QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes) == QMessageBox::Yes)
            save();
    }
    graphFile = QString();
    QFile(autoSaveFile).remove();
    autoSaveFile = QString();
    toolbar->setEnabled(false);
    mInterface->clear();

}
#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(OlvisExecGuiPlugin, OlvisExecGuiPlugin);
#endif
