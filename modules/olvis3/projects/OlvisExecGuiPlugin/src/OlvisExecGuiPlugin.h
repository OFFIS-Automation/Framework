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

#ifndef OLVISEXECGUIPLUGIN_H
#define OLVISEXECGUIPLUGIN_H

#include <interfaces/GuiPluginInterface.h>
#include <QTimer>

class MainToolBar;
class VideoWidget;
class QXmlStreamWriter;
class QXmlStreamReader;
class OlvisInterface;

class OlvisExecGuiPlugin : public QObject, public GuiPluginInterface
{
    Q_OBJECT
    Q_INTERFACES(GuiPluginInterface)
    Q_PLUGIN_METADATA(IID GuiPluginInterface_iid)
public:
    OlvisExecGuiPlugin();
    virtual QString getName() const { return "OlvisExecGuiInterface"; }
    virtual void setModelInterface(const QString& name, QObject* modelInterface);
    virtual bool requirementsMet();
    virtual void initialize(const QString& pluginDir);
    virtual void deinitialize();
    virtual void addElements(MainWindowInterface* mainWindow);
    virtual void start();
    virtual QObject* getConnectObject() { return this; }
    virtual void setGuiInterface(const QString &name, QObject *guiInterface);
    void saveGraph(QXmlStreamWriter& writer);
    void loadProject(const QString &projectFile);
    void closeProject();
protected slots:
    void loadGraphFile(const QString& filename);
    void loadGraphString(const QString& data);
    void saveGraphFile(const QString& filename);
    void saveGraphString(QString& data);
    virtual void writeXmlConfig(QXmlStreamWriter &writer);
    virtual void readXmlConfig(const QString& data);
    void autoSave();
    void save();
    void restore();
signals:
    void pluginsWriteConfigReq(QXmlStreamWriter &writer);
    void pluginsReadConfigReq(const QString &data);
private:
    QTimer mTimer;
    OlvisInterface* mInterface;

    MainToolBar* toolbar;
    VideoWidget* videoWidget;
    QString graphFile, autoSaveFile;


};

#endif // OLVISEXECGUIPLUGIN_H
