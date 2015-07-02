#include "ColorEdit.h"

#include <QColorDialog>

ColorEdit::ColorEdit(QWidget *parent) : StringEdit(parent)
{
    lineEdit->setInputMask("\\#>HHHHHHHH");
    lineEdit->setToolTip(tr("Hexadecimal format: #AARRGGBB.\nA = alpha "
                            "channel. two hexadecimal digits for each color"));
    mColorChoseButton = new QToolButton(this);
    mColorChoseButton->setIcon(
        QIcon(QPixmap(":/DefaultPortsGui/images/color.png")));
    mColorChoseButton->setToolTip(tr("Open color picker dialog"));
    ui->layout->insertWidget(1, mColorChoseButton);
    connect(mColorChoseButton, &QToolButton::clicked, this,
            &ColorEdit::onChoseColorClicked);
}

ColorEdit::~ColorEdit() {}

QString ColorEdit::asString()
{
    return mValue.value<QColor>().name(QColor::HexArgb).toUpper();
}

QVariant ColorEdit::editValue(bool &ok)
{
    QString t = lineEdit->text();
    if (t.length() != asString().length()) {
        ok = false;
        return QColor();
    }
    return QColor(lineEdit->text());
}

void ColorEdit::onChoseColorClicked()
{
    QColor c =
        QColorDialog::getColor(mValue.value<QColor>(), this, tr("Select Color"),
                               QColorDialog::ShowAlphaChannel);
    if (c.isValid()) {
        lineEdit->setText(c.name(QColor::HexArgb));
        ui->submit->click();
    }
}
