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


#ifndef GRAPHOVERLAYOPTIONS_H
#define GRAPHOVERLAYOPTIONS_H

#include <QDialog>

namespace Ui {
class GraphOverlayOptions;
}

class GraphOverlayOptions : public QDialog
{
    Q_OBJECT
    
public:
    explicit GraphOverlayOptions(bool showHistory, bool limitedSmaples, int numSamples, int listId, QWidget *parent = 0);
    ~GraphOverlayOptions();
    
    bool showHistory();
    bool limitedValues();
    int maxValues();
    int listId();
private:
    Ui::GraphOverlayOptions *ui;
};

#endif // GRAPHOVERLAYOPTIONS_H
