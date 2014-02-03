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

#ifndef DEFINELOLECWIZARDPAGE_H
#define DEFINELOLECWIZARDPAGE_H

#include <QWizardPage>

namespace Ui {
class AddLolecWizard;
}


class DefineLolecWizardPage : public QWizardPage
{
    Q_OBJECT
public:
    explicit DefineLolecWizardPage(QWidget *parent = 0);
    void init(Ui::AddLolecWizard* ui);
    void setHasError(bool error);
    bool isComplete() const;
signals:
    
public slots:
protected:
    bool mError;
};

#endif // DEFINELOLECWIZARDPAGE_H
