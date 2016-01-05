// OFFIS Automation Framework
// Copyright (C) 2013-2016 OFFIS e.V.
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

#ifndef PORTID_H
#define PORTID_H


struct PortId
{
    PortId() : filter(0){}
    PortId(int filter_, const QString& port_) : filter(filter_), port(port_) {}
    int filter;
    QString port;
    bool isValid() const { return filter > 0; }
};


inline bool operator==(const PortId &p1, const PortId &p2) { return p1.filter == p2.filter&& p1.port== p2.port; }
inline bool operator<(const PortId &p1, const PortId &p2) { return p1.filter < p2.filter || (!(p2.filter<p1.filter) && p1.port < p2.port); }

#endif // PORTID_H
