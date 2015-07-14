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
#include <QVector3D>
#include <QtMath>

#include <telecontrol/TcConfig.h>

TutorialUnit::TutorialUnit()
{
    mAcquired = false;
    mOffset = QPointF(0, 0);
    mScaling = 1.0;
    mSetHapticStartAxes = false;
    setScene(GraphicsView::instance());
}

TutorialUnit::~TutorialUnit()
{
    release();
}

void TutorialUnit::setScene(GraphicsView *scene)
{
    mScene = scene;
    connect(this, SIGNAL(moveRel(QPointF,int)), scene, SLOT(moveRobotRel(QPointF,int)));
    connect(this, SIGNAL(moveAbs(QPointF,int)), scene, SLOT(moveRobotAbs(QPointF,int)));
    connect(this, SIGNAL(rotateAbs(double,int)), scene, SLOT(rotateRobotAbs(double,int)));
    connect(this, SIGNAL(rotateRel(double,int)), scene, SLOT(rotateRobotRel(double,int)));
    connect(this, SIGNAL(setGripperState(bool)), scene, SLOT(setGripperState(bool)));
    connect(this, SIGNAL(resetScene(bool)), scene, SLOT(reset(bool)));
    connect(scene, SIGNAL(movementFinished()), SLOT(onMovementFinished()));
    connect(scene, SIGNAL(positionUpdate(QPointF,qreal)), this, SIGNAL(positionUpdated(QPointF,qreal)));
}

QVariant TutorialUnit::rcGetPosition()
{
    Pose2d pos = getPosition();
    return QVector3D(pos.x, pos.y, pos.phi);
}

void TutorialUnit::rcSetPosition(const QVariant &var)
{
    if(var.canConvert(QVariant::Vector3D))
    {
        QVector3D vec = var.value<QVector3D>();
        setPosition(vec.toPointF());
        setRotation(vec.z());
    }
}

RcFlagDefinitions TutorialUnit::rcFlagDefinitions() const
{
    RcFlagDefinitions defs;
    defs << RcFlagDefinition("x", 1);
    defs << RcFlagDefinition("y", 1);
    defs << RcFlagDefinition("rot", 1);
    return defs;
}

QVariantList TutorialUnit::rcFlags()
{
    Pose2d pos = getPosition();
    QVariantList list;
    list << pos.x;
    list << pos.y;
    list << pos.phi;
    return list;
}

void TutorialUnit::stop()
{

}

void TutorialUnit::acquire()
{
    QMutexLocker lock(&mMutex);
    mAcquired = true;
    emit hwConnectionStatusChanged(true);
}

void TutorialUnit::release()
{
    QMutexLocker lock(&mMutex);
    mAcquired = false;
    emit hwConnectionStatusChanged(false);
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

void TutorialUnit::moveGamepad(double x, double y, double phi)
{
    QMutexLocker lock(&mMutex);
    checkHAL();
    QMutexLocker waitLock(&mWaitMutex);

    emit moveRel(QPointF(x*20.0, y*-20.0), 20);
    mWait.wait(&mWaitMutex, 20);
    emit rotateRel(phi*20, 1);
    mWait.wait(&mWaitMutex, 20);
}

void TutorialUnit::moveGamepad3d(double x, double y, double yaw)
{
    moveGamepad(x, y, yaw);
}

void TutorialUnit::startMoveHaptic(bool toogled)
{
    if(toogled){
        mSetHapticStartAxes = true;
    }
}

QMap<int, double> TutorialUnit::moveHaptic(QMap<int, double> axes)
{
    if(axes.keys().count() == 0){
        stop();
    }

    if(mSetHapticStartAxes){
        mHapticStartAxes = axes;
        mHapticPreviousAxes = axes;
        mSetHapticStartAxes = false;
    }

    QPointF p0(mHapticPreviousAxes[Tc::Haptic::AxisX], mHapticPreviousAxes[Tc::Haptic::AxisY]);
    QPointF p1(axes[Tc::Haptic::AxisX], axes[Tc::Haptic::AxisY]);

    if(p0 != p1){
        double deltaXp = p1.x() - p0.x();
        double deltaYp = p1.y() - p0.y();

        double alpha1 = qAtan(deltaXp/deltaYp);
        double alpha2 = mHapticStartAxes[Tc::Haptic::AxisYaw] - alpha1;

        double d = qSqrt(qPow(deltaXp, 2) + qPow(deltaYp, 2));

        double deltaXa = qSin(alpha2) * d;
        double deltaYa = qCos(alpha2) * d;

        qDebug() << "prev:" << deltaXp << ";" << deltaYp;
        qDebug() << "corr:" << deltaXa << ";" << deltaYa;

        moveGamepad(deltaXa, deltaYa, 0.0f);
    }

    // Store axes for next iteration
    mHapticPreviousAxes = axes;

    // Create return value
    QMap<int, double> force;
    force[Tc::Haptic::AxisX] = 0.0;
    force[Tc::Haptic::AxisY] = 0.0;
    force[Tc::Haptic::AxisZ] = 0.0;

    // Check if robot is in "bounds"
    /*QPointF currentPosition = QPointF(getPosition().x, getPosition().y);
    QRect workspace = QRect(50,50,500,300);

    if(currentPosition.x() < workspace.left()){
        // Robot left workspace on left hand side
        force[Tc::Haptic::AxisX] = abs(currentPosition.x()-workspace.left())*10.0;
    } else if(workspace.right() < currentPosition.x()){
        // Robot left workspace on right hand side
        force[Tc::Haptic::AxisX] = abs(currentPosition.x()-workspace.right())*-10.0;
    }

    if(currentPosition.y() < workspace.top()){
        // Robot left workspace on the top
        force[Tc::Haptic::AxisY] = abs(currentPosition.y()-workspace.top())*-10.0;
    } else if(workspace.bottom() < currentPosition.y()){
        // Robot left workspace on the bottom
        force[Tc::Haptic::AxisY] = abs(currentPosition.y()-workspace.bottom())*10.0;
    }*/

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
