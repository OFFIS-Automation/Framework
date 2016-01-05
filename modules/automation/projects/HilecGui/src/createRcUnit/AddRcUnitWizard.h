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

#ifndef ADDRCUNITWIZARD_H
#define ADDRCUNITWIZARD_H

#include <QWizard>

namespace Ui {
class AddRcUnitWizard;
}
class RcUnitInterface;

class AddRcUnitWizard : public QWizard
{
    Q_OBJECT
    
public:
    explicit AddRcUnitWizard(const QString& projectFile, QWidget *parent = 0);
    ~AddRcUnitWizard();
    void addPlugin();
signals:
    void projectFileChanged(QString);
private slots:
    void on_selectFileButton_clicked();

    void on_rcUnitSelect_currentIndexChanged(const QString &arg1);

    void on_name_textChanged(const QString &arg1);

    void on_configFile_textChanged(const QString &arg1);

private:
    Ui::AddRcUnitWizard *ui;
    QString mProjectFile, mFileExtension;
    RcUnitInterface* mRcUnit;
};

#endif // ADDRCUNITWIZARD_H
