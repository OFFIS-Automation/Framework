# OFFIS Automation Framework
# Copyright (C) 2013-2017 OFFIS e.V.
# 
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
# 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http:#www.gnu.org/licenses/>.

import offisio;

from offis.types import *

def getUnit(name):
	return RcUnit(name);

class RcUnit:
	def __init__(self, name):
		self._name = str(name);
		constants = offisio.rcGetConstants(self._name);
		for k, v in constants.items():
			object.__setattr__(self, k, v);
		self._method = "";
		
	def __call(self, *_args):
		args = []
		for arg in _args:
			args.append(self.__formatCall(arg))
		var = offisio.rcCall(self._name, self._method, tuple(args));
		return self.__format(var)
	def __getattr__(self, name):
		self._method = name
		return self.__call
	def name(self):
		return self._name
		
	def __formatCall(self, val):
		if isinstance(val, list):
			l = []
			for elem in val:
				l.append(self.__formatCall(elem));
			return l;
		if isinstance(val, Point):
			return { 'type': 'Point', 'x': val.x, 'y': val.y }
		if isinstance(val, Size):
			return { 'type': 'Size', 'width': val.width, 'height': val.height }
		if isinstance(val, Pose2d):
			return { 'type': 'Pose2d', 'x': val.x, 'y': val.y, 'phi': val.phi }
		if isinstance(val, Rect):
			return { 'type': 'Rect', 'x': val.topLeft().x, 'y': val.topLeft().y, 'width': val.size().width, 'height': val.size().height }
		if isinstance(val, Vector3d):
			return { 'type': 'Vector3D', 'x': val.x, 'y': val.y, 'z': val.z }
		if isinstance(val, Vector4d):
			return { 'type': 'Vector4D', 'x': val.x, 'y': val.y, 'z': val.z, 'w' : val.w }
		return val;
	
	def __format(self, val):
		if isinstance(val, list):
			l = []
			for elem in val:
				l.append(self.__format(elem));
			return l;
		if not isinstance(val, dict):
			return val;
		if not "type" in val:
			for key in val:
				tmpVal = val[key];
				val[key] = self.__format(tmpVal);
			return val;
		if val["type"] == "Point":
			return Point(val["x"], val["y"]);
		if val["type"] == "Size":
			return Size(val["width"], val["height"]);
		if val["type"] == "Pose2d":
			return Pose2d(val["x"], val["y"], val["phi"]);
		if val["type"] == "Rect":
			return Rect(Point(val["x"], val["y"]), Size(val["width"], val["height"]));
		if val["type"] == "Vector3D":			
			return Vector3d(val["x"], val["y"], val["z"]);
		if val["type"] == "Vector4D":			
			return Vector4d(val["x"], val["y"], val["z"], val["w"]);
		return val;
