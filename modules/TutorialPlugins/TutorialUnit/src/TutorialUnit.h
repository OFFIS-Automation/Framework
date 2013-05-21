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

#ifndef TUTORIALUNIT_H
#define TUTORIALUNIT_H

#include <QObject>
#include "GraphicsView.h"
#include <lolecs/Pose2d.h>
#include <QMutex>
#include <QWaitCondition>

class TutorialUnit : public QObject
{
    Q_OBJECT
public:
    TutorialUnit();
    void setScene(GraphicsView* scene);
public slots:
    void acquireHardware();
    void releaseHardware();
    Pose2d getPosition();
    void setPosition(QPointF targetPosition);
    void setRotation(double targetAngle);
    void openGripper();
    void closeGripper();
    void moveGamepad(double xAxis, double yAxis, double phi);
    void resetSetup();
    void resetSetupRandom();
    void randomizeCoordinateSystem();

private slots:
    void onMovementFinished();
signals:
    void connectStatusChanged(bool connected);

    void moveRel(QPointF pos, int moveTime = -1);
    void moveAbs(QPointF pos, int moveTime = -1);
    void rotateAbs(double angle, int moveTime = -1);
    void rotateRel(double angle, int moveTime = -1);
    void setGripperState(bool open);
    void resetScene(bool randomPositions);
protected:
    QPointF mOffset;
    double mScaling;
    void checkHAL();
    bool mAcquired;
    GraphicsView* mScene;
    QMutex mMutex, mWaitMutex;
    QWaitCondition mWait;
};

#endif // TUTORIALUNIT_H
