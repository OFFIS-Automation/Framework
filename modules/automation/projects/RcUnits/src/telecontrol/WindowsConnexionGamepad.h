#ifndef WINDOWSCONNEXIONGAMEPAD_H
#define WINDOWSCONNEXIONGAMEPAD_H

#include "Gamepad.h"

#include <si.h>

class WindowsTelecontrolFactory;
class WindowsConnexionGamepad : public Gamepad
{
public:
    ~WindowsConnexionGamepad();

    // Called when a new system message is available
    // For 'internal' use only (by RawInputEventFilter class)!
    bool onSiEvent(void* eventData);

protected:
    WindowsConnexionGamepad(SiDevID devID, SiOpenData oData);

    void run();
    bool initialize();
    void buttonEvent(int buttonNumber, bool pressed);
    int buttonNumberToButtonId(int buttonNumber);


    QString getName() { return mName; }
    GamepadType getGamepadType() const { return WindowsConnexion; }

    QString mName;
    SiOpenData mOData;
    SiDevID mDevID;
    SiHdl mDevHdl;

    QMap<int,double> mJoysticks;
    QMap<int,bool> mButtons, mLastButtons;

    friend class WindowsTelecontrolFactory;
};

#endif // WINDOWSCONNEXIONGAMEPAD_H
