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

#ifndef EXTENDEDTEXTEDIT_H
#define EXTENDEDTEXTEDIT_H

#include <QPlainTextEdit>

class ExtendedTextEdit : public QPlainTextEdit
{
    Q_OBJECT
public:
    ExtendedTextEdit(QWidget* parent = 0);
    bool atScrollEnd();
    void scrollToEnd();

public slots:
    void saveContentText(){ saveContent(false); }
    void saveContentHtml(){ saveContent(true); }
    void toggleWrapping();

protected:
    void saveContent(bool asHtml);
    virtual void contextMenuEvent(QContextMenuEvent *event);
    virtual QMenu* createMenu();
};

#endif // EXTENDEDTEXTEDIT_H
