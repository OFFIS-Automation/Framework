#include "WindowsXBOXGamepad.h"

#include <telecontrol/TcConfig.h>

WindowsXBOXGamepad::WindowsXBOXGamepad(const QString &name) : WindowsGamepad(name, ""){}

void WindowsXBOXGamepad::update(QMap<int, double> &joysticks, QMap<int, bool> &buttons)
{
    // Axis
    DIJOYSTATE2& status = mState;
    joysticks[Tc::LeftJoystickX] = correctedValue(float(status.lX));
    joysticks[Tc::LeftJoystickY] = correctedValue(-float(status.lY));
    joysticks[Tc::RightJoystickX] = correctedValue(float(status.lRx));
    joysticks[Tc::RightJoystickY] = correctedValue(-float(status.lRy));



    // D-Pad
    bool up = false, down = false, left = false, right = false;
    switch(status.rgdwPOV[0]) // contour-clockwise from left direction
    {
        case 9000:  right = true; break;
        case 4500:  right =  true; up =  true; break;
        case 0:     up =  true; break;
        case 31500: left = true; up =  true; break;
        case 27000: left = true; break;
        case 22500: left = true; down = true; break;
        case 18000: down = true; break;
        case 13500: right = true; down = true; break;
    }
    buttons[Tc::ButtonUp] = up;
    buttons[Tc::ButtonDown] = down;
    buttons[Tc::ButtonLeft] = left;
    buttons[Tc::ButtonRight] = right;
}

