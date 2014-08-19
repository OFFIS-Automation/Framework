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

#include "ExtendedTextEdit.h"

#include <QScrollBar>
#include <QContextMenuEvent>
#include <QMenu>
#include <QFileDialog>
#include <QTextStream>
#include <QFile>


ExtendedTextEdit::ExtendedTextEdit(QWidget* parent) : QPlainTextEdit(parent)
{
}

void ExtendedTextEdit::saveContent(bool html)
{
        static QString file = "";
        QString filter = tr("Text files (*.txt)");
        if(html)
                filter = tr("HTML files (*.html)");
        QString filename = QFileDialog::getSaveFileName(this,tr("Select file to save content"), file, filter);
        if(filename.isEmpty())
                return;
        QFileInfo info(filename);
        file = info.absolutePath();
        QFile f(filename);
        if(!f.open(QIODevice::WriteOnly |QIODevice::Truncate | QIODevice::Text))
                return;
        QTextStream stream(&f);
        if(html)
                stream << document()->toHtml();
        else
                stream << toPlainText();

}

bool ExtendedTextEdit::atScrollEnd()
{
        return verticalScrollBar()->value() == verticalScrollBar()->maximum();
}

void ExtendedTextEdit::scrollToEnd()
{
        verticalScrollBar()->setValue(verticalScrollBar()->maximum());
}

void ExtendedTextEdit::toggleWrapping()
{
    if(lineWrapMode() != QPlainTextEdit::NoWrap)
        setLineWrapMode(QPlainTextEdit::NoWrap);
    else
        setLineWrapMode(QPlainTextEdit::WidgetWidth);
}


QMenu* ExtendedTextEdit::createMenu()
{
    QMenu *menu = createStandardContextMenu();
    menu->addSeparator();
    QAction* action = menu->addAction(tr("Clear"));
    connect(action, SIGNAL(triggered()), this, SLOT(clear()));
    action = menu->addAction(tr("Save as text"));
    connect(action, SIGNAL(triggered()), this, SLOT(saveContentText()));
    action = menu->addAction(tr("Save as HTML"));
    connect(action, SIGNAL(triggered()), this, SLOT(saveContentHtml()));
    menu->addSeparator();
    action = menu->addAction(tr("Wrap text"));
    action->setCheckable(true);
    action->setChecked(lineWrapMode() != QPlainTextEdit::NoWrap);
    connect(action, SIGNAL(triggered()), this, SLOT(toggleWrapping()));
    connect(action, SIGNAL(triggered(bool)), action, SLOT(setChecked(bool)));
    return menu;
}


void ExtendedTextEdit::contextMenuEvent(QContextMenuEvent *event)
{
        QMenu* menu = createMenu();
        menu->exec(event->globalPos());
        delete menu;
}
