#ifndef VERSION_H
#define VERSION_H

namespace Version
{
    static const int MAJOR = MAJOR_VERSION;
    static const int MINOR = MINOR_VERSION;
    static const int PATCHLEVEL = PATCH_VERSION;
    static const QString DATE = QString(__DATE__).simplified();
    static QString versionString()
    {
        if(Version::PATCHLEVEL > 0){
            return QObject::tr("Version: %1.%2.%3 (%4)").arg(Version::MAJOR).arg(Version::MINOR).arg(Version::PATCHLEVEL).arg(Version::DATE);
        } else {
            return QObject::tr("Version: %1.%2 (%3)").arg(Version::MAJOR).arg(Version::MINOR).arg(Version::DATE);
        }
    }
}

#endif // VERSION_H
