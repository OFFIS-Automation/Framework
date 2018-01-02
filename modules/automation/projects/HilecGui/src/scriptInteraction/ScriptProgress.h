// OFFIS Automation Framework
// Copyright (C) 2013-2018 OFFIS e.V.
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

#ifndef SCRIPTPROGRESS_H
#define SCRIPTPROGRESS_H

#include <QGroupBox>

namespace Ui {
    class ScriptProgress;
}

class ScriptProgress : public QGroupBox
{
    Q_OBJECT

public:
    explicit ScriptProgress(const QString& name, int maximum, QWidget *parent = 0);
    ~ScriptProgress();
    void setProgress(int progress);
private:
    Ui::ScriptProgress *ui;
};

#endif // SCRIPTPROGRESS_H
