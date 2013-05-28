#ifndef WINDOWSXBOXGAMEPAD_H
#define WINDOWSXBOXGAMEPAD_H

#include "../WindowsGamepad.h"

class WindowsXBOXGamepad : public WindowsGamepad
{
public:
    WindowsXBOXGamepad(const QString& name);
protected:
    virtual void update(QMap<int, double>& joysticks, QMap<int, bool>& buttons);

};

#endif // WINDOWSXBOXGAMEPAD_H
