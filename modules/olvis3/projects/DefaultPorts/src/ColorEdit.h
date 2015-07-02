#ifndef COLOREDIT_H
#define COLOREDIT_H

#include "StringEdit.h"

class ColorEdit : public StringEdit
{
    Q_OBJECT
public:
    ColorEdit(QWidget* parent = 0);
    virtual ~ColorEdit();
    virtual QString asString();
    virtual QVariant editValue(bool& ok);
protected slots:
    void onChoseColorClicked();
protected:
    QToolButton* mColorChoseButton;

};

#endif // COLOREDIT_H
