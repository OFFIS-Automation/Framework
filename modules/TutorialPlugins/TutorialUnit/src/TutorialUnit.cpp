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

#include "TutorialUnit.h"
#include <QDebug>
#include <QDateTime>
#include <QPointF>
#include <QRect>
#include <stdexcept>

TutorialUnit::TutorialUnit()
{
    mAcquired = false;
    mOffset = QPointF(0, 0);
    mScaling = 1.0;
}

void TutorialUnit::setScene(GraphicsView *scene)
{
    mScene = scene;
    connect(this, SIGNAL(moveRel(QPointF,int)), scene, SLOT(moveRobotRel(QPointF,int)));
    connect(this, SIGNAL(moveAbs(QPointF,int)), scene, SLOT(moveRobotAbs(QPointF,int)));
    connect(this, SIGNAL(rotateAbs(double,int)), scene, SLOT(rotateRobotAbs(double,int)));
    connect(this, SIGNAL(rotateRel(double,int)), scene, SLOT(rotateRobotRel(double,int)));
    connect(this, SIGNAL(setGripperState(bool)), scene, SLOT(setGripperState(bool)));
    connect(this, SIGNAL(alternateGripperState()), scene, SLOT(alternateGripperState()));
    connect(this, SIGNAL(resetScene(bool)), scene, SLOT(reset(bool)));
    connect(scene, SIGNAL(movementFinished()), SLOT(onMovementFinished()));
    connect(scene, SIGNAL(positionUpdate(QPointF,qreal)), this, SIGNAL(positionUpdated(QPointF,qreal)));
}

void TutorialUnit::acquireHardware()
{
    QMutexLocker lock(&mMutex);
    mAcquired = true;
    emit connectStatusChanged(true);
}

void TutorialUnit::releaseHardware()
{
    QMutexLocker lock(&mMutex);
    mAcquired = false;
    emit connectStatusChanged(false);
}

Pose2d TutorialUnit::getPosition()
{
    QMutexLocker lock(&mMutex);
    checkHAL();
    Pose2d pos = mScene->getPosition();

    pos.x *= mScaling;
    pos.y *= mScaling;
    pos.x += mOffset.x();
    pos.y += mOffset.y();
    return pos;
}

void TutorialUnit::setPosition(QPointF targetPosition)
{
    QMutexLocker lock(&mMutex);
    checkHAL();
    targetPosition -= mOffset;
    targetPosition /= mScaling;
    QMutexLocker waitLock(&mWaitMutex);
    emit moveAbs(targetPosition);
    mWait.wait(&mWaitMutex, 10000);
}

void TutorialUnit::setRotation(double targetAngle)
{
    QMutexLocker lock(&mMutex);
    checkHAL();
    QMutexLocker waitLock(&mWaitMutex);
    emit rotateAbs(targetAngle);
    mWait.wait(&mWaitMutex, 5000);
}

void TutorialUnit::moveGamepad(double xAxis, double yAxis, double phi)
{
    QMutexLocker lock(&mMutex);
    checkHAL();
    QMutexLocker waitLock(&mWaitMutex);

    emit moveRel(QPointF(xAxis*20.0, yAxis*-20.0), 20);
    mWait.wait(&mWaitMutex, 20);
    emit rotateRel(phi*20, 1);
    mWait.wait(&mWaitMutex, 20);
}

QMap<int, double> TutorialUnit::moveHaptic(QMap<int, double> axes)
{
    moveGamepad(axes[Tc::HapticAxisX], axes[Tc::HapticAxisY], axes[Tc::HapticAxisZ]*0.5);

    // Create return value
    QMap<int, double> force;
    force[Tc::HapticAxisX] = 0.0;
    force[Tc::HapticAxisY] = 0.0;
    force[Tc::HapticAxisZ] = 0.0;

    // Check if robot is in "bounds"
    QPointF currentPosition = QPointF(getPosition().x, getPosition().y);
    QRect workspace = QRect(50,50,500,300);

    if(currentPosition.x() < workspace.left()){
        // Robot left workspace on left hand side
        force[Tc::HapticAxisX] = abs(currentPosition.x()-workspace.left())*10.0;
    } else if(workspace.right() < currentPosition.x()){
        // Robot left workspace on right hand side
        force[Tc::HapticAxisX] = abs(currentPosition.x()-workspace.right())*-10.0;
    }

    if(currentPosition.y() < workspace.top()){
        // Robot left workspace on the top
        force[Tc::HapticAxisY] = abs(currentPosition.y()-workspace.top())*-10.0;
    } else if(workspace.bottom() < currentPosition.y()){
        // Robot left workspace on the bottom
        force[Tc::HapticAxisY] = abs(currentPosition.y()-workspace.bottom())*10.0;
    }

    return force;
}

void TutorialUnit::alternateGripper(bool open)
{
    if(!open){
        openGripper();
    } else {
        closeGripper();
    }
}

void TutorialUnit::resetSetup()
{
    resetSetup(false);
}

void TutorialUnit::resetSetup(bool randomize)
{
    srand(QDateTime::currentDateTime().toTime_t());
    QMutexLocker lock(&mMutex);
    QMutexLocker waitLock(&mWaitMutex);
    mOffset = QPointF(0, 0);
    mScaling = 1.0;
    emit resetScene(randomize);
    mWait.wait(&mWaitMutex, 1000);
    mWait.wait(&mWaitMutex, 500);
}

void TutorialUnit::resetSetupRandom()
{
    resetSetup(true);
}

void TutorialUnit::randomizeCoordinateSystem()
{

    mOffset.setX(qrand() % 100);
    mOffset.setY(qrand() % 100);
    mOffset -= QPointF(50, 50);
    mScaling = (qrand() % 10000);
    mScaling /= 10000.0;
    mScaling += 0.5;
}

void TutorialUnit::openGripper()
{
    QMutexLocker lock(&mMutex);
    checkHAL();
    QMutexLocker waitLock(&mWaitMutex);
    mWait.wait(&mWaitMutex, 300);
    emit setGripperState(true);
    mWait.wait(&mWaitMutex, 300);
}

void TutorialUnit::closeGripper()
{
    QMutexLocker lock(&mMutex);
    checkHAL();
    QMutexLocker waitLock(&mWaitMutex);
    mWait.wait(&mWaitMutex, 300);
    emit setGripperState(false);
    mWait.wait(&mWaitMutex, 300);
}

void TutorialUnit::onMovementFinished()
{
    QMutexLocker waitLock(&mWaitMutex);
    mWait.wakeAll();
}


void TutorialUnit::checkHAL()
{
    if(!mAcquired){
        throw std::runtime_error("Hardware is not acquired");
    }
}
