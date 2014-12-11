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
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "TranslationLoader.h"
#include <QDir>
#include <QCoreApplication>
#include <QTranslator>
#include <QApplication>
#include <QLocale>
#include <QDebug>

TranslationLoader::TranslationLoader(QObject *parent) :
    QObject(parent)
{
}

QStringList TranslationLoader::scanLanguages()
{
    QDir dir(QCoreApplication::applicationDirPath() + "/lang/");
    return dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot, QDir::Name | QDir::IgnoreCase);
}

bool TranslationLoader::installSystemTranslator()
{
    QStringList availableLanguages = scanLanguages();
    QStringList strippedLangs;
    foreach(QString lang, QLocale::system().uiLanguages())
    {
        if(availableLanguages.contains(lang))
        {
            if(installTranslator(lang))
                return true;
        }
        lang = lang.replace("-", "_");
        if(availableLanguages.contains(lang))
        {
            if(installTranslator(lang))
                return true;
        }
        if(lang.contains("_"))
            strippedLangs << lang.left(lang.indexOf("_"));
    }
    foreach(QString lang, strippedLangs)
    {
        if(availableLanguages.contains(lang))
        {
            if(installTranslator(lang))
                return true;
        }
    }
    return true;

}

bool TranslationLoader::installTranslator(QString language)
{
    bool found = false;
    QDir dir(QCoreApplication::applicationDirPath() + "/lang/" + language);
    foreach(QString file, dir.entryList(QDir::Files, QDir::Name | QDir::IgnoreCase))
    {
        found = true;
        QTranslator* translator = new QTranslator(this);
        translator->load(file, dir.absolutePath());
        QApplication::installTranslator(translator);
    }
    return found;
}

