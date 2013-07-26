// OFFIS Automation Framework
// Copyright (C) 2013 OFFIS e.V.
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

#ifndef USERREQUEST_H
#define USERREQUEST_H

#include <QString>
#include <QVariant>


struct UserRequestItem
{
    QString desc;
    QVariant value;
    QMap<QString, QVariant> constraints;
};

struct UserRequestButton
{
    QString name;
    int id;
};

struct UserRequest
{
public:
    int uid;
    QString title;
    QList<UserRequestItem> items;
    QList<UserRequestButton> buttons;

};
inline QDataStream& operator<<(QDataStream& stream, const UserRequestButton& requestButton)
{
    stream << requestButton.id;
    stream << requestButton.name;
    return stream;
}

inline QDataStream& operator>>(QDataStream& stream, UserRequestButton& requestButton)
{
    stream >> requestButton.id;
    stream >> requestButton.name;
    return stream;
}

inline QDataStream& operator<<(QDataStream& stream, const UserRequestItem& requestItem)
{
    stream << requestItem.desc;
    stream << requestItem.value;
    stream << requestItem.constraints;
    return stream;
}

inline QDataStream& operator>>(QDataStream& stream, UserRequestItem& requestItem)
{
    stream >> requestItem.desc;
    stream >> requestItem.value;
    stream >> requestItem.constraints;
    return stream;
}

inline QDataStream& operator<<(QDataStream& stream, const UserRequest& request)
{
    stream << request.uid;
    stream << request.title;
    stream << request.items;
    stream << request.buttons;
    return stream;
}

inline QDataStream& operator>>(QDataStream& stream, UserRequest& request)
{
    stream >> request.uid;
    stream >> request.title;
    stream >> request.items;
    stream >> request.buttons;
    return stream;
}


#endif // USERREQUEST_H
