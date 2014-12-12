// OFFIS Automation Framework
// Copyright (C) 2013-2014 OFFIS e.V.
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

#ifndef RCREPEATABLE_H
#define RCREPEATABLE_H

template<typename t, unsigned min = 0, unsigned max = -1> class RcRepeatable : public QList<t>
{
public:
        RcRepeatable(const QList<t>& other ) : QList<t>(other) {}
        RcRepeatable() : QList<t>() {}
};

#endif // RCREPEATABLE_H