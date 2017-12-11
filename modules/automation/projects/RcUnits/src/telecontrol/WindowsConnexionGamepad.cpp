#include "WindowsConnexionGamepad.h"

#include <QElapsedTimer>
#include <QCoreApplication>
#include <QDebug>
#include <QAbstractNativeEventFilter>

#include <telecontrol/TcConfig.h>

// 3DxWare
#include <spwmacro.h>
#include <si.h>
#include <siapp.h>
#include <Windows.h>

#define AXIS_MAXIMUM 350

// Unique instance of this WindowsConnexionGamepad
static WindowsConnexionGamepad* sWindowsConnexionGamepadInstance = 0;

/**
 * @brief The RawInputEventFilter class
 * This class is used for event handling and forwarding
 * mouse input to our WindowsConnexionGamepad class
 */
class RawInputEventFilter : public QAbstractNativeEventFilter
{
public:
    virtual bool nativeEventFilter(const QByteArray& eventType, void* msg, long* result) Q_DECL_OVERRIDE
    {
        Q_UNUSED(eventType);

        if(!sWindowsConnexionGamepadInstance || !msg){
            return false;
        }

        SiGetEventData eventData;
        MSG* messageStruct = static_cast<MSG*>(msg);
        SiGetEventWinInit(&eventData, messageStruct->message, messageStruct->wParam, messageStruct->lParam);
        return sWindowsConnexionGamepadInstance->onSiEvent(&eventData);
    }
};


WindowsConnexionGamepad::WindowsConnexionGamepad(SiDevID devID, SiOpenData oData) :
    mDevID(devID),
    mOData(oData),
    mDevHdl(SI_NO_HANDLE)
{
    // Register current instance
    sWindowsConnexionGamepadInstance = this;

    // Setup event filter
    static RawInputEventFilter sRawInputEventFilter;
    qApp->installNativeEventFilter(&sRawInputEventFilter);
}

WindowsConnexionGamepad::~WindowsConnexionGamepad()
{
    if(mDevHdl){
        SiClose(mDevHdl);
        mDevHdl = 0;
    }

    // Unregister current instance
    if (sWindowsConnexionGamepadInstance == this){
        sWindowsConnexionGamepadInstance = 0;
    }
}

void WindowsConnexionGamepad::run()
{
    mStop = false;

    while(!mStop){
        QElapsedTimer timer;
        timer.start();

        // Check for button toggle
        QMapIterator<int,bool> iter(mButtons);
        while(iter.hasNext()){
            iter.next();
            int buttonId = iter.key();
            bool value = iter.value();

            if(!mLastButtons.contains(buttonId) || (mLastButtons.contains(buttonId) && mLastButtons[buttonId] != value)){
                if(mLastButtons.contains(buttonId)){
                    emit buttonToggled(buttonId, value, getName());
                } else {
                    if(value){
                        emit buttonToggled(buttonId, value, getName());
                    }
                }
            }
        }
        mLastButtons = mButtons;

        // Handle joysticks data
        emit dataUpdated(mJoysticks, getName());

        // Eventually sleep thread, limit update rate (20Hz)
        int remaining = 50 - timer.elapsed();
        if(remaining > 0){
            msleep(remaining);
        }
    }
}

bool WindowsConnexionGamepad::initialize()
{
    try{
        if(!SiIsInitialized()){
             throw std::runtime_error(qPrintable(tr("SiAppDll not initialized")));
        }

        mDevHdl = SiOpen(qPrintable(QCoreApplication::applicationName()), mDevID, SI_NO_MASK, SI_EVENT, &mOData);
        if(mDevHdl == SI_NO_HANDLE){
            throw std::runtime_error(qPrintable(tr("Could not open device")));
        }
        SiDevInfo info;
        if (SiGetDeviceInfo(mDevHdl, &info) == SPW_NO_ERROR){
            if (info.majorVersion == 0 && info.minorVersion == 0){
                throw std::runtime_error(qPrintable(tr("Not a real device")));
            }
        }

        // Config SoftButton Win Display
        SiSetUiMode(mDevHdl, SI_UI_NO_CONTROLS);

        // Read basic parameters
        SiDeviceName devName;
        SiGetDeviceName(mDevHdl, &devName);
        mName = devName.name;

        return true;
    } catch(const std::exception& e) {
        qWarning() << tr("Could not initialize connexion gamepad:") << " " <<  e.what();
        return false;
    }
}

void WindowsConnexionGamepad::buttonEvent(int buttonNumber, bool pressed)
{
    int buttonID = buttonNumberToButtonId(buttonNumber);
    if(buttonID > 0){
        mButtons[buttonID] = pressed;
    }
}

int WindowsConnexionGamepad::buttonNumberToButtonId(int buttonNumber)
{
    int buttonID = -1;
    if(buttonNumber == V3DK_MENU){
        buttonID = Tc::Connexion::MenuButton;
    } else if(buttonNumber == V3DK_FIT){
        buttonID = Tc::Connexion::FitButton;
    } else if(buttonNumber == V3DK_TOP || buttonNumber == V3DK_BOTTOM){
        buttonID = Tc::Connexion::TopBottomButton;
    } else if(buttonNumber == V3DK_RIGHT || buttonNumber == V3DK_LEFT){
        buttonID = Tc::Connexion::RightLeftButton;
    } else if(buttonNumber == V3DK_FRONT || buttonNumber == V3DK_BACK){
        buttonID = Tc::Connexion::FrontBackButton;
    } else if(buttonNumber == V3DK_ROLL_CW || buttonNumber == V3DK_ROLL_CCW){
        buttonID = Tc::Connexion::CWCCWButton;
    } else if(buttonNumber == V3DK_ROTATE){
        buttonID = Tc::Connexion::RotateButton;
    } else if(buttonNumber == V3DK_ISO1 || buttonNumber == V3DK_ISO2){
        buttonID = Tc::Connexion::ISO1ISO2Button;
    } else if(buttonNumber == V3DK_CTRL){
        buttonID = Tc::Connexion::CtrlButton;
    } else if(buttonNumber == V3DK_ALT){
        buttonID = Tc::Connexion::AltButton;
    } else if(buttonNumber == V3DK_SHIFT){
        buttonID = Tc::Connexion::ShiftButton;
    } else if(buttonNumber == V3DK_ESC){
        buttonID = Tc::Connexion::EscButton;
    } else if(buttonNumber == V3DK_ENTER){
        buttonID = Tc::Connexion::EnterButton;
    } else if(buttonNumber == V3DK_DELETE){
        buttonID = Tc::Connexion::DeleteButton;
    } else if(buttonNumber == V3DK_SPACE){
        buttonID = Tc::Connexion::SpaceButton;
    } else if(buttonNumber == V3DK_TAB){
        buttonID = Tc::Connexion::TabButton;
    } else if(buttonNumber == V3DK_1){
        buttonID = Tc::Connexion::OneButton;
    } else if(buttonNumber == V3DK_2){
        buttonID = Tc::Connexion::TwoButton;
    } else if(buttonNumber == V3DK_3){
        buttonID = Tc::Connexion::ThreeButton;
    } else if(buttonNumber == V3DK_4){
        buttonID = Tc::Connexion::FourButton;
    } else if(buttonNumber == V3DK_5){
        buttonID = Tc::Connexion::FiveButton;
    } else if(buttonNumber == V3DK_6){
        buttonID = Tc::Connexion::SixButton;
    } else if(buttonNumber == V3DK_7){
        buttonID = Tc::Connexion::SevenButton;
    } else if(buttonNumber == V3DK_8){
        buttonID = Tc::Connexion::EightButton;
    } else if(buttonNumber == V3DK_9){
        buttonID = Tc::Connexion::NineButton;
    } else if(buttonNumber == V3DK_10){
        buttonID = Tc::Connexion::TenButton;
    } else if(buttonNumber == V3DK_11){
            buttonID = Tc::Connexion::ElevenButton;
    } else if(buttonNumber == V3DK_12){
        buttonID = Tc::Connexion::TwelveButton;
    } else if(buttonNumber == V3DK_VIEW_1){
        buttonID = Tc::Connexion::View1Button;
    } else if(buttonNumber == V3DK_VIEW_2){
            buttonID = Tc::Connexion::View2Button;
    } else if(buttonNumber == V3DK_VIEW_3){
        buttonID = Tc::Connexion::View3Button;
    }
    return buttonID;
}

bool WindowsConnexionGamepad::onSiEvent(void *eventData)
{
    if(mDevHdl == SI_NO_HANDLE || !eventData){
        return false;
    }

    // Get 3DxWare data event
    SiSpwEvent event;
    if (SiGetEvent (mDevHdl, NULL, static_cast<SiGetEventData*>(eventData), &event) != SI_IS_EVENT){
        return false;
    }

    switch (event.type) {
    case SI_ZERO_EVENT:
    {
        // Reset joystick values
        foreach(int key, mJoysticks.keys()){
            mJoysticks[key] = 0;
        }
        // Reset implicit activation button
        mButtons[Tc::Connexion::ImplicitActivationButton] = false;

        return true; 
    }
    case SI_MOTION_EVENT:
    {
        const SiSpwData& data = event.u.spwData;
        if (data.mData[SI_TX] != 0 || data.mData[SI_TY] != 0 || data.mData[SI_TZ] != 0
                || data.mData[SI_RX] != 0 || data.mData[SI_RY] != 0 || data.mData[SI_RZ] != 0)
        {
            mJoysticks[Tc::Connexion::JoystickX] = data.mData[SI_TX] ;
            mJoysticks[Tc::Connexion::JoystickY] = data.mData[SI_TZ];
            mJoysticks[Tc::Connexion::JoystickZ] = data.mData[SI_TY];
            mJoysticks[Tc::Connexion::JoystickYaw] = data.mData[SI_RY];
            mJoysticks[Tc::Connexion::JoystickPitch] = data.mData[SI_RX];
            mJoysticks[Tc::Connexion::JoystickRoll] = data.mData[SI_RZ];
        }

        foreach(int key, mJoysticks.keys()){
            // Scale values to -1.0 to 1.0
            mJoysticks[key] = qBound<double>(-1.0, mJoysticks[key]/AXIS_MAXIMUM, 1.0);


            // Set implicit activation
            if(mJoysticks[key] != 0){
                mButtons[Tc::Connexion::ImplicitActivationButton] = true;
            }
        }

        return true;
    }
    case SI_APP_EVENT:
    {
        size_t len = strnlen_s(event.u.appCommandData.id.appCmdID, SI_MAXAPPCMDID);
        if (len == 0 || len == SI_MAXAPPCMDID){
            break;
        }
        buttonEvent(atoi(event.u.appCommandData.id.appCmdID), event.u.appCommandData.pressed);
        return true;
    }
    case SI_BUTTON_EVENT:
        break;
    case SI_BUTTON_PRESS_EVENT:
        buttonEvent(event.u.hwButtonEvent.buttonNumber, true);
        return true;
    case SI_BUTTON_RELEASE_EVENT:
        buttonEvent(event.u.hwButtonEvent.buttonNumber, false);
        return true;
    default:
        break;
    }

    return false;
}
