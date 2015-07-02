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

#ifndef TUTORIALUNIT_H
#define TUTORIALUNIT_H

#include <QObject>
#include "../../TutorialCore/src/GraphicsView.h"
#include <rc/Pose2d.h>
#include <QMutex>
#include <QWaitCondition>
#include <rc/types/RobotRcUnit.h>

class TutorialUnit : public RobotRcUnit
{
    Q_OBJECT
public:
    TutorialUnit();
    virtual ~TutorialUnit();
    void setScene(GraphicsView* scene);
    QVariant rcGetPosition();
    void rcSetPosition(const QVariant& var);
    RcFlagDefinitions rcFlagDefinitions() const;
    QVariantList rcFlags();

public slots:
    void stop();
    void acquire();
    void release();
    Pose2d getPosition();
    void setPosition(QPointF targetPosition);
    void setPosition(double x, double y) { setPosition(QPointF(x, y)); }
    void setRotation(double targetAngle);
    void openGripper();
    void closeGripper();
    void alternateGripper(bool open);
    void moveGamepad(double x, double y, double phi);
    void moveGamepad3d(double x, double y, double yaw);
    QMap<int, double> moveHaptic(QMap<int, double> axes);
    void resetSetup();
    void resetSetup(bool randomize);
    void resetSetupRandom();
    void randomizeCoordinateSystem();

private slots:
    void onMovementFinished();

signals:
    void positionUpdated(QPointF pos, qreal rot);
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

    QMap<int, double> oldAxes;
    bool oldAxesValid;
};

#endif // TUTORIALUNIT_H
