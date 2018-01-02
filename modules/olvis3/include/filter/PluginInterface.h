// OFFIS Automation Framework
// Copyright (C) 2013-2018 OFFIS e.V.
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

#ifndef PLUGININTERFACE_H
#define PLUGININTERFACE_H

#include <filter/UserFilter.h>

class UserFilterCreatorI;
class UserFilterFactoryI;
class OlvisPlugin;

class PluginInterface
{
private:
    static PluginInterface& instance();
    friend class UserFilterCreatorI;
    friend class UserFilterFactoryI;
    friend class OlvisPlugin;
    QList<UserFilterFactoryI*> factories;
    QList<UserFilterCreatorI*> creators;
    PluginInterface(){}
    Q_DISABLE_COPY(PluginInterface);
};

class UserFilterFactoryI
{
public:
    UserFilterFactoryI(){ PluginInterface::instance().factories.append(this); }
    virtual void setup() = 0;
    virtual int numFilters() = 0;
    virtual UserFilter* instance(int number) = 0;
};

class UserFilterCreatorI
{
public:
    UserFilterCreatorI(){ PluginInterface::instance().creators.append(this); }
    virtual UserFilter* instance() = 0;
};


template <class T> class UserFilterCreator : public UserFilterCreatorI
{
public:
    UserFilter* instance() { return new T(); }
};


#define REGISTER_FILTER(cls)  UserFilterCreator<cls> _user_filter_creator__##cls

#endif // PLUGININTERFACE_H
