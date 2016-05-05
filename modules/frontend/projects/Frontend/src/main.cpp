// OFFIS Automation Framework
// Copyright (C) 2013-2016 OFFIS e.V.
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

#include <QApplication>
#include <QSettings>
#include <QPainter>
#include <QDesktopWidget>
#include <QSplashScreen>
#include <QStyleFactory>
#include <Notifications.h>
#include <QSysInfo>
#include <QThread>

#include "Application.h"
#include "DockWidgetTitle.h"
#include "MasterWindow.h"
#include "PluginLoader.h"

#include "PerspectiveControl.h"
#include "TranslationLoader.h"
#include "version.h"
#include "qglobal.h"

#include <interfaces/logging.h>
#include <LogWindow.h>
#include <winSparkle/include/winsparkle.h>

int canShutdownCallback()
{
    return 1;
}

void shutdownRequestCallback()
{
    QApplication::quit();
}

int main(int argc, char *argv[])
{
    Application a(argc, argv);
    a.setLibraryPaths(a.libraryPaths() << a.applicationDirPath() + "/plugins");
    a.setOrganizationName("OFFIS");
    a.setApplicationName("Automation Framework");
    a.setOrganizationDomain("http://www.uni-oldenburg.de/amir");
    if(QSysInfo::windowsVersion() < QSysInfo::WV_10_0){
        a.setStyle(QStyleFactory::create("Fusion"));
    }

    // Init winSparkle, Setup update feeds.
    wchar_t charVersion[256];
    swprintf_s(charVersion, L"%d", Version::BUILD_VERSION_NUMBER);

    win_sparkle_set_appcast_url("http://134.106.47.173:8080/userContent/Framework/Framework.rss");
    win_sparkle_set_app_details(L"OFFIS", L"OFFIS Automation Framework", charVersion);
    win_sparkle_set_can_shutdown_callback(canShutdownCallback);
    win_sparkle_set_shutdown_request_callback(shutdownRequestCallback);

    // Initialize the updater and possibly show some UI
    win_sparkle_init();

    // Initialize translator
    TranslationLoader translator;
    translator.installSystemTranslator();

    // Migrate old settings
    QSettings::setDefaultFormat(QSettings::IniFormat);
    QFileInfo settingsFile(QSettings().fileName());
    if(!settingsFile.exists()){
        QFileInfo oldFile = settingsFile.dir().absoluteFilePath("Automation Toolbox.ini");
        if(oldFile.exists()){
            QFile::rename(oldFile.absoluteFilePath(), settingsFile.absoluteFilePath());
        }
    }

    // Create splash screen
    QPixmap* splashPicture = new QPixmap(":/img/SplashScreen.png");
    QString versionString = Version::versionString();
    QPainter* painter=new QPainter(splashPicture);
    painter->drawText(QRect(0, 16, 250, 20), Qt::AlignCenter, versionString);

    QSplashScreen splashScreen;
    splashScreen.setPixmap(*splashPicture);

    // Show
    splashScreen.show();
    splashScreen.showMessage(a.translate("splash screen", "Starting"), Qt::AlignHCenter);

    // Get window arguments
    QDesktopWidget* desktop = a.desktop();
    bool multiScreen = desktop->screenCount() > 1;
    if(a.arguments().contains("--singleScreen")){
        multiScreen = false;
    }
    bool noload = a.arguments().contains("--noload");

    PluginLoader loader(a.applicationDirPath() + "/plugins");
    loader.load(&splashScreen);

    MasterWindow* masterWindow = new MasterWindow();
    MainWindow* slaveWindow = 0;
    if(multiScreen){
        slaveWindow = new MainWindow();
    }

    // Set windowm titles
    if(multiScreen){
        QString title = masterWindow->windowTitle();
        masterWindow->setWindowTitle(a.translate("MainWindow", "%1 - Master window").arg(title));
        slaveWindow->setWindowTitle(a.translate("MainWindow", "%1 - Slave window").arg(title));
    }

    // Signal / slot connections
    QObject::connect(masterWindow, SIGNAL(openProject(QString)), &loader, SLOT(openProject(QString)));
    QObject::connect(masterWindow, SIGNAL(closeProject()), &loader, SLOT(closeProject()));

    // Create perspectives (inside master / slave)
    PerspectiveControl perspectiveControl(masterWindow, slaveWindow);

    // Add logWindow as default widget
    QDockWidget* logWindow = new LogWindow();
    perspectiveControl.addDockWidget(Qt::BottomDockWidgetArea, logWindow, "Default");

    // Load all other widgets (depending on perspective)
    loader.configure(&splashScreen, &perspectiveControl, noload);

    // Shows windows
    masterWindow->setAlternative(slaveWindow);
    masterWindow->move(desktop->availableGeometry(desktop->primaryScreen()).topLeft());
    masterWindow->showMaximized();

    if(multiScreen){
        slaveWindow->setAlternative(masterWindow);
        int screen = desktop->primaryScreen() == 0 ? 1 : 0;
        slaveWindow->move(desktop->availableGeometry(screen).topLeft());
        slaveWindow->showMaximized();
    }
    splashScreen.finish(masterWindow);
    perspectiveControl.start();

    // Set notification reference
    Notifications::setMainWindow(masterWindow);

    // Run
    int retVal = a.exec();

    // Free notification reference
    Notifications::setMainWindow(0);

    // Close project, save perspective, then deinit GUIs
    perspectiveControl.savePerspective();
    loader.closeProject();
    loader.deinitializeGuis();

    // Shut WinSparkle down cleanly when the app exits
    win_sparkle_cleanup();

    // Free memory
    if(multiScreen){
        delete slaveWindow;
    }
    delete masterWindow;

    loader.unload();
    return retVal;
}
