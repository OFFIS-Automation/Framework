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

#ifndef LOLECINTERFACE_H
#define LOLECINTERFACE_H

#include <QObject>
#include <QVariant>

#include "RcExceptions.h"
#include "../telecontrol/TcConfig.h"

class RcWrapperFactoryItf;
class RcBase
{
public:
    // remote control interface
    /**
     * @brief Adds a method to the published interface.
     * @param the method name
     * @param shortDesc a short description (one short sentence) HTML formatting is possible
     * @param longDesc a long description about the method and its parameters. HTML formatting is possible
     * The method must be a public slot of the low-level controller, and can have only supported types as params and return value
     * Currently, supported types are bool, int, float, double, QString, QPoint, QPointF, QRect, RectF and QVector3D
     * Also, you can use QLists of the supported types
     */
    virtual void addMethod(const QString& name, const QString& shortDesc, const QString& longDesc) = 0;

    /**
     * @brief adds a constant value to the object
     * @param name the constant name
     * @param constant a constant of any of the supported types (@see addMethod)
     * The constant can be accessed in the automation. You can use this for example to give enum values into automation
     */
    virtual void addConstant(const QString name, const QVariant& constant) = 0;

    /**
     * @brief registers a method to be called by the gamepad
     * The method registered with this function is periodically called if the operator uses the gamepad for this lolec.
     * The period is 10ms, the operation should not need longer than this.
     * The method must be void and must have only doubles as parameters. Each of the parameters represents a joystick.
     * The values given to this function are [-1:1]. The gamepad method should realize a relative movement with the parameter as speed.
     * The second parameter @a defaultMapping must have as many joystick ids as there are parameters in the method
     * The @a defaultActivateButton is a button on the gamepad that represents the "dead-mans-switch". Gamepad is only possible if this button is held down.
     * The @ defaultSensitivity is a factor that is multiplied to the gamepad value before is is given the the method. Range (0:1].
     * With the sensitivity, the gamepad value is scaled down. This can later be adjusted by the user.
     * Example:
     * void moveByGamepad(double x, double y); // values for x and y: [-1:1]
     * registerGamepadMethod("moveByGamepad", Tc::joysticks(Tc::LeftJoystickX, Tc::LeftJoystickY), Tc::Button5, 0.0125);
     */
    virtual void registerGamepadMethod(QString methodName, const QList<Tc::Joystick>& defaultMapping, int defaultActivateButton, double defaultSensitivity = 1.0/64.0) = 0;

    /**
     * @brief registers a method to be called if a gamepad button is pressed
     * The method registered is called when a button is toggled or pressed.
     * The signature can be methodName() -- method is called when the button is "clicked" (pressed and released)
     * The signature can be methodName(bool toggled) -- method is called when the button is pressed or released
     * @param defaultMapping the sensitive button
     * @param hiddenToUser this button assingment is hidden from the user, it is not displayed in the GUI
     */
    virtual void registerGamepadButtonMethod(QString methodName, int defaultMapping, bool hiddenToUser = false) = 0;

    /**
     * @brief registers a method to be called by the haptic device
     * The method registered with this function is periodically called if the operator uses the haptic device for this lolec.
     * The method must be QMap<int, double> and must have only one QMap<int, double> as parameter.
     * The map keys should be TC::Haptic axes. The values for each axes are in range [-1:1].
     * The haptic method should realize a relative movement with the parameter as speed.
     * The @a defaultActivateButton is a button on the gamepad that represents the "dead-mans-switch". Haptic is only posble if this button is held down.
     * The @ defaultSensitivity is a factor that is multiplied to the haptic value before is is given the the method. Range (0:1].
     * The @ defaultForceScaling is a factor that is multiplied with the forces before they are applied as haptic feedback. Range (0:1].
     * Example:
     * QMap<int, double> moveHaptic(QMap<int, double> axes);; // values for x and y: [-1:1]
     * registerHapticMethod("moveHaptic", Tc::hapticAxis(Tc::HapticAxisX, Tc::HapticAxisY, Tc::HapticAxisZ), Tc::PrimaryButton);
     */
    virtual void registerHapticMethod(QString methodName, const QList<Tc::HapticAxis> &defaultMapping, Tc::HapticButton defaultActivateButton, double defaultSensitivity = 1.0/64.0, double defaultForceScaling = 1.0/64.0) = 0;

    /**
     * @brief registers a method to be called if a haptic button is pressed
     * The method registered is called when a button is toggled or pressed.
     * The signature can be methodName() -- method is called when the button is "clicked" (pressed and released)
     * The signature can be methodName(bool toggled) -- method is called when the button is pressed or released
     * @param defaultMapping the sensitive button
     * @param hiddenToUser this button assingment is hidden from the user, it is not displayed in the GUI
     */
    virtual void registerHapticButtonMethod(QString methodName, Tc::HapticButton defaultMapping, bool hideFromUser = false) = 0;

    virtual void setParamNames(const QString& methodName, const QStringList& names) = 0;
    /**
     * This function should never be called directly. Instead, use the static method @a rcRegisterStruct in RcStruct.h
     */
    virtual void registerStruct(int id, const QByteArray& name, const QStringList& typeNames, const QList<int>& types, RcWrapperFactoryItf* wrapper) = 0;
};

class LolecInterface
{
public:
    /**
     * @brief this function must return true if the RC-Unit needs a config file
     * @return true if a config file is required, false if this is not the case
     */
    virtual bool needsConfigFile() const = 0;

    /**
     * @brief return the config file extension. This function can be overriden if a different file extension than the default is needed (e.g. xml)
     * This is just a hint for the user; the real config file can be of any ending
     * @return the extension name without the leading dot
     */
    virtual QString configFileExtension() const { return QString("ini"); }

    /**
     * @brief return a user friendly name
     * This function should return a user firendly name of the RC-Unit. This is a user information and is not connected with the name
     * given to instance()
     * @return a user friendly name of he Unit
     */
    virtual QString name() const = 0;

    /**
     * @brief return a description
     * This function should return a description of the RC-Unit. It should be a brief information about the use-case and the capablilities of the unit.
     * @return description
     */
    virtual QString description() const = 0;

    /**
     * @brief create a config file
     * This function can be overriden to write a default config file or to show a config wizard. The function is called in the main thread
     * creating widgets is allowed
     * The function is called once when a RC-Unit is added to a project
     * @return false if there was an error; the unit will NOT be added to the project. true otherwise
     */
    virtual bool createConfig(const QString& configFile, QWidget* parentForGuis) { Q_UNUSED(configFile); Q_UNUSED(parentForGuis); return true; }

    /**
     * @brief can be used to to preparations before creating the RC-Unit
     * this method can be overriden to do mandatory checks before the RC-Unit creation, like showing License wizards
     * this method is called in the main thread, using widgets is allowed
     * use qCritical() for logging purposes
     * @return true if the unit can be instantiated, false otherwise
     */
    virtual bool prepareInstantiation() { return true; }
    /**
     * @brief this function should return an instance of the RC-Unit controller
     * The function is called in a dedicated thread for this RC-Unit. Using of widgets or other GUI elements will crash the application
     * The lolec is deleted using the deleteInstance method
     * @param rc the above described interface; can be used to publish method of the controller to the automation or register telecontrol methods
     * @param configFilePath a path to a config file. Will be empty if needsConfigFile returns false. The config file may or may not exist on the hard drive
     * @param name The name of the RC-Unit in the automation, can be used for sensor data or logging
     * @return the created RC-Unit object or 0 if there was an error, use qCritical() for logging purposes
     */
    virtual QObject* instance(RcBase& rc, const QString& configFilePath, const QString& name) = 0;

    /**
     * @brief can return a GUI for the lolec
     * the gui should be small and compact and should privide mostly read-only information
     * this method is called in the main thread. A parent will be assinged to the gui, also the gui will be shown automatically
     * (dont call show or setVisible in this method);
     * @param instance this is the instance that was returned by the method @a instance
     * @return The main widget of the gui or 0, if there is no gui for this RC-Unit
     * The gui is automatically deleted by the automation main gui
     */
    virtual QWidget* guiForInstance(QObject* instance) = 0;

    /**
     * @brief deletes an instance created by the method instance
     * The default implementation deletes the object; this should be sufficient for most scenarions
     * If you need a special treatment (e.g. don't delete, just free some hardware) you can ovverride this method
     * this function is called in the same dedicated thread as the instance method.
     * Using GUI elements will crash the application
     */
    virtual void deleteInstance(QObject* instance) { delete instance; }
    /**
     * @brief extra method that is called if a function called from automation is not found
     * The default implementation raises an RcError("No such method")
     * @param instance the rcUnit instance
     * @param methodName the method that is requested
     * @param params the params that were given
     * @return return the return value of the function
     */
    virtual QVariant call(QObject* instance, const QByteArray& methodName, const QVariantList& params)
        { Q_UNUSED(instance); Q_UNUSED(params); throw RcError(QObject::tr("No such method: %1").arg(QString(methodName))); }

};
#define LolecInterface_iid "offis.automation.RcInterface/2.4"
Q_DECLARE_INTERFACE(LolecInterface, LolecInterface_iid)

#if QT_VERSION < 0x050000
#define Q_PLUGIN_METADATA(x)
#endif

#endif // LOLECINTERFACE_H
