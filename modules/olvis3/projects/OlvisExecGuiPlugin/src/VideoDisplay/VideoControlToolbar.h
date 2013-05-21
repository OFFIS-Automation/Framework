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

#ifndef VIDEOCONTROLTOOLBAR_H
#define VIDEOCONTROLTOOLBAR_H

#include <QToolBar>
#include <QActionGroup>
#include <core/PortInfo.h>
#include <core/PortId.h>
#include <QMap>

namespace Ui {
    class VideoControlToolbar;
}

class VideoControlToolbar : public QToolBar
{
    Q_OBJECT

public:
    explicit VideoControlToolbar(QWidget *parent = 0);
    ~VideoControlToolbar();

    void setZoomToFit(bool value);
    void setFlip(bool hFlip, bool vFlip);

    void addPortAction(const PortId &portId);
    void removePortAction(const PortId &portId);
    QAction* currentAction();
    PortInfo currentPortInfo();
    PortId portId(QAction* action);
    PortId currentPortId();
    void clear();
    bool showImageInfo();

public slots:
    void portValueChanged(int filterId, const QString& portId, QVariant value);

signals:
    void clearClicked();
    void zoomChanged(double zoom);
    void zoomToFitChanged(bool value);
    void horizontalFlipChanged(bool value);
    void verticalFlipChanged(bool value);
    void screenshotRequested();
    void videoRequested();

    void editChangeRequested(QAction* action);
    void boolChangeRequested(QAction* action);
    void choiceChangeRequested(QAction* action);
private:
    Ui::VideoControlToolbar *ui;
    QActionGroup mEditActions;
    QActionGroup mBoolActions;
    QMap<QAction*, PortId> mPortIds;
    QMap<QAction*, PortId> mChoicePortIds;

private slots:
    void on_actionSaveVideo_triggered();
    void on_actionSaveImage_triggered();
    void on_actionZoomFit_triggered(bool checked);
    void on_actionZoom100_triggered();
    void on_actionClear_triggered();

    void on_actionHorizontalFlip_triggered(bool checked);
    void on_actionVerticalFlip_triggered(bool checked);
};

#endif // VIDEOCONTROLTOOLBAR_H
