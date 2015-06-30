#include <ports/ColorPort.h>

QColor port::Color::rgb(const cv::Vec3b &vec)
{
    return QColor(vec[0], vec[1], vec[2]);
}

QColor port::Color::rgba(const cv::Vec4b &vec)
{
    return QColor(vec[0], vec[1], vec[2], vec[3]);
}

QColor port::Color::bgr(const cv::Vec3b &vec)
{
    return QColor(vec[2], vec[1], vec[0]);
}

QColor port::Color::bgra(const cv::Vec4b &vec)
{
    return QColor(vec[2], vec[1], vec[0], vec[3]);
}

cv::Vec3b port::Color::rgb(const QColor &color)
{
    return cv::Vec3b(color.red(), color.green(), color.blue());
}

cv::Vec4b port::Color::rgba(const QColor &color)
{
    return cv::Vec4b(color.red(), color.green(), color.blue(), color.alpha());
}

cv::Vec3b port::Color::bgr(const QColor &color)
{
    return cv::Vec3b(color.blue(), color.green(), color.red());
}

cv::Vec4b port::Color::bgra(const QColor &color)
{
    return cv::Vec4b(color.red(), color.green(), color.blue(), color.alpha());
}

port::Color::Color() : Port(QMetaType::QColor, "Color")
{
    setIcon(QImage(":/defaultPorts/images/color.png"));
}

QString port::Color::toString(const QVariant &var) const
{
    return var.value<QColor>().name(QColor::HexArgb);
}

QVariant port::Color::fromString(const QString &str) const
{
    return QColor(str);
}

QVariant port::Color::constrainedValue(const QVariant &var) const
{
    return var;
}

void out::Color::send(const QColor &color) { sendRaw(color); }

void out::Color::sendRgb(const cv::Vec3b &rgbValue) { send(rgb(rgbValue)); }

void out::Color::sendRgba(const cv::Vec4b &rgbaValue) { send(rgba(rgbaValue)); }

void out::Color::sendBgr(const cv::Vec3b &bgrValue) { send(bgr(bgrValue)); }

void out::Color::sendBgra(const cv::Vec4b &bgraValue) { send(bgra(bgraValue)); }

void out::Color::send(uchar red, uchar green, uchar blue, uchar alpha)
{
    send(QColor(red, green, blue, alpha));
}

void in::Color::setDefault(QColor value) { setDefaultValueRaw(value); }

QColor in::Color::getValue() { return getRawValue().value<QColor>(); }

cv::Vec3b in::Color::bgr() { return port::Color::bgr(getValue()); }

cv::Vec4b in::Color::bgra() { return port::Color::bgra(getValue()); }

cv::Vec3b in::Color::rgb() { return port::Color::rgb(getValue()); }

cv::Vec4b in::Color::rgba() { return port::Color::rgba(getValue()); }

uchar in::Color::red() { return getValue().red(); }

uchar in::Color::green() { return getValue().green(); }

uchar in::Color::blue() { return getValue().blue(); }

uchar in::Color::alpha() { return getValue().alpha(); }
