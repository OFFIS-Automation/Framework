#include "WindowsXBOXGamepad.h"

#include <telecontrol/TcConfig.h>

WindowsXBOXGamepad::WindowsXBOXGamepad(const QString &name) : WindowsGamepad(name){}

void WindowsXBOXGamepad::update(QMap<int, double> &joysticks, QMap<int, bool> &buttons)
{
    // Axis
    DIJOYSTATE2& status = mState;
    joysticks[Tc::XAxisLeft] = correctedValue(float(status.lX));
    joysticks[Tc::YAxisLeft] = correctedValue(-float(status.lY));
    buttons[Tc::LeftAxisPush] = status.rgbButtons[8] != 0;
    joysticks[Tc::XAxisRight] = correctedValue(float(status.lRx));
    joysticks[Tc::YAxisRight] = correctedValue(-float(status.lRy));
    buttons[Tc::RightAxisPush] = status.rgbButtons[9] != 0;

    // Buttons
    buttons[Tc::Button1] = status.rgbButtons[2] != 0;
    buttons[Tc::Button2] = status.rgbButtons[0] != 0;
    buttons[Tc::Button3] = status.rgbButtons[1] != 0;
    buttons[Tc::Button4] = status.rgbButtons[3] != 0;
    buttons[Tc::Button5] = status.rgbButtons[4] != 0;
    buttons[Tc::Button6] = status.rgbButtons[5] != 0;
    buttons[Tc::Button7] = status.lZ > 0;
    buttons[Tc::Button8] = status.lZ < 0;
    buttons[Tc::Button9] = status.rgbButtons[6] != 0;
    buttons[Tc::Button10] = status.rgbButtons[7] != 0;

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

