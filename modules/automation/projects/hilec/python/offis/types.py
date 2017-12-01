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

import math;

class Point:
	def __init__(self, x, y):
		self.x = x;
		self.y = y;
	
	def __str__(self):
		return "Point(%f, %f)" % (self.x, self.y);
	
	def __repr__(self):	
		return str(self);
		
	def __add__(self, v):
		return Point(self.x + v.x, self.y + v.y);
		
	def __sub__(self, v):
		return Point(self.x - v.x, self.y - v.y);
	
	# 	@param number arbitrary real number
	def __mul__(self, number):
		return Point(self.x * number, self.y * number);
	
	def __truediv__(self, number):
		return Point(self.x / number, self.y / number);
		
	def __neg__(self):
		return Point(-self.x, -self.y)
		
	def toDict(self):
		return {'x': self.x, 'y': self.y }
				
	def __eq__(self, v):
		if type(self) != type(v):
			return False;
		return self.x == v.x and self.y == v.y;
	
	def __neq__(self, v):
		return not (self == v);

	def distTo(self, p):
		return self.euclidianNorm(p);
		
	def euclidianNorm(self, v):
		dx = v.x - self.x;
		dy = v.y - self.y;
		return math.sqrt(dx * dx + dy * dy);

class Pose2d:
	def __init__(self, x, y, phi):
		self.x = x;
		self.y = y;
		self.phi = phi;
	
	def __str__(self):
		return "Pose2d(%f, %f, %f)" % (self.x, self.y, self.phi);
	
	def __repr__(self):	
		return str(self);
		
	def __add__(self, v):
		return Pose2d(self.x + v.x, self.y + v.y, self.phi + v.phi);
		
	def __sub__(self, v):
		return Pose2d(self.x - v.x, self.y - v.y, self.phi - v.phi);
	
	def __neg__(self):
		return Pose2d(-self.x, -self.y, -self.phi)
		
	def toDict(self):
		return {'x': self.x, 'y': self.y , 'phi' : self.phi }
				
	def __eq__(self, v):
		if type(self) != type(v):
			return False;
		return self.x == v.x and self.y == v.y and self.phi == v.phi;
	
	def __neq__(self, v):
		return not (self == v);

	def toPoint(self):
		return Point(self.x, self.y);
		

class Size:
	def __init__(self, width, height):
		self.width = width;
		self.height = height;
	def __repr__(self):
		return "Size(%f, %f)" % (self.width, self.height);
		
	def __add__(self, v):
		return Size(self.width + v.width, self.height + v.height);
		
	def __sub__(self, v):
		return Size(self.width - v.width, self.height - v.height);
	
	# 	@param number arbitrarheight real number
	def __mul__(self, number):
		return Size(self.width * number, self.height * number);
	
	def __truediv__(self, number):
		return Size(self.width / number, self.height / number);
		
	def __neg__(self):
		return Size(-self.width, -self.height)
		
	def toDict(self):
		return {'width': self.width, 'height': self.height }
				
	def __eq__(self, v):
		if type(self) != type(v):
			return False;
		return self.width == v.width and self.height == v.height;
	
	def __neq__(self, v):
		return not (self == v);

	

class Rect:
	## constructor
	## @param x1 (int/float) x-value lower left corner
	## @param y1 (int/float) y-value lower left corner
	## @param x2 (int/float) x-value uper right corner
	## @param y2 (int/float) y-value uper right corner
	def __init__(self, arg0, arg1, arg2 = None, arg3 = None):
		if isinstance(arg0, Point) and isinstance(arg1, Point):
			self.setFromPoints(arg0, arg1)
		elif isinstance(arg0, Point) and isinstance(arg1, Size):
			self.setFromPointAndSize(arg0, arg1)
		else:
			self.set(arg0, arg1, arg2, arg3)
			
	def setFromPoints(self, p1, p2):
		self.set(p1.x, p1.y, p2.x, p2.y)
	
	def setFromPointAndSize(self, p, s):
		self.set(p.x, p.y, p.x + s.width, p.y + s.height)
	
	def set(self, x1, y1, x2, y2):
		self.x1 = min(x1, x2);
		self.y1 = min(y1, y2);
		self.x2 = max(x1, x2);
		self.y2 = max(y1, y2);
	
	def left(self):
		return self.x1;
	
	def right(self):
		return self.x2;
	
	def top(self):
		return self.y1;
		
	def bottom(self):
		return self.y2;
	
	def firstPoint(self):
		return Point(self.x1, self.y1)

	def secondPoint(self):
		return Point(self.x2, self.y2)
			
	## @return (int/float) width of rectangle
	def width(self):
		return abs(self.x2-self.x1);
	
	## @return (int/float) height of rectangle
	def height(self):
		return abs(self.y2-self.y1);

	## @return (int/float) area of rectangle
	def area(self):
		return self.width() * self.height();

	## @return (Point)
	def topLeft(self):
		return self.firstPoint();
	## @return (Point)
	def bottomRight(self):
		return self.secondPoint();
	
	## @return (Point) center of rectangle
	def center(self):
		return Point((self.x1 + self.x2) / 2, (self.y1 + self.y2) / 2)
	
	def size(self):
		return Size(abs(self.x1 - self.x2), abs(self.y1 - self.y2));
	
	## @return (Rectangle) translated rectangle
	def translate(self, arg0, arg1 = None):
		if not isinstance(arg0, Point):
			arg0 = Point(arg0, arg1)

		return Rect(self.x1 + arg0.x, self.y1 +arg0.y , self.x2 + arg0.x, self.y2 + arg0.y);
	
	## @return (Rectangle) to global coordinates translated rectangle
	def translateTo(self, arg0, arg1):
		if not isinstance(arg0, Point):
			arg0 = Point(arg0, arg1)
		return Rect(arg0, self.size());
		
	## expands (or reduces) the rectangle's borders by a fixed value around the center
	## @param offset (int/float) value the rectangle shall expand/reduce
	def expand(self, offset):
		p1 = self.topLeft() - Point(offset, offset);
		p2 = self.bottomRight() + Point(offset, offset);
		return Rect(p1, p2);
	
	def normalized(self):
		return Rect(self.topLeft(), self.bottomRight());
	## intersects two rectangles if possible
	## @param rect the rectangle which shall be intersected with this one
	## @return the intersection of self and rect (or (0,0,0,0) if no intersection exists)
	def intersect(self, rect):
		myRect = self.normalized();
		rect = rect.normalized();
		ix1 = max(myRect.x1, rect.x1);
		ix2 = min(myRect.x2, rect.x2);
		if (ix1 < ix2):
			iy1 = max(myRect.y1, rect.y1);
			iy2 = min(myRect.y2, rect.y2);
			if (iy1 < iy2):
				return Rect(ix1, iy1, ix2, iy2);
		return Rect(0, 0, 0, 0);
    		
	## combines two rectangles (union)
	## @param rect (Rectangle) the rectangle which shall be combined with this rectangle
	def unite(self, rect):
		myRect = self.normalized();
		rect = rect.normalized();
		ux1 = min(myRect.x1, rect.x1);
		uy1 = min(myRect.y1, rect.y1);
		ux2 = max(myRect.x2, rect.x2);
		uy2 = max(myRect.y2, rect.y2);
		return Rect(ux1, uy1, ux2, uy2);
	
	def contains(self, p):	
		p1 = self.topLeft();
		p2 = self.bottomRight();
		return (p.x >= p1.x) and (p.x <= p2.x) and (p.y >= p1.y) and (p.y <= p2.y);
	
	def __str__(self):
		return  "Rect[" + str(self.topLeft()) + "," + str(self.size()) + "]";
	
	def __repr__(self):
		return str(self);
	## checks if rectangle is empty
	## @return (bool) True, if rectangle is empty
	def isEmpty(self):
		if(self.x1 == 0 and self.y1 == 0 and self.x2 == 0 and self.y2 == 0):
			return True;
		else:
			return False;
	
	def toDict(self):
		return {'x1' : self.x1, 'y1' : self.y1, 'x2' : self.x2, 'y2' : self.y2 }
	
	## compares two Rectangles
	## @param rect (Rectangle) the rectangle to compare this rectangle with.
	## @return (bool) True, if actual and given Rectangle are equal
	def __eq__(self, rect):
		if type(self) != type(rect):
			return False;
		if(self.x1 == rect.x1 and self.y1 == rect.y1 and self.x2 == rect.x2 and self.y2 == rect.y2):
			return True;
		return False;
	
	def __neq__(self, rect):
		return not(self == rect);
		

class Vector3d:
	def __init__(self, x, y, z):
		self.x = x;
		self.y = y;
		self.z = z;
	def __repr__(self):
		return "Vector3d(%f, %f, %f)" % (self.x, self.y, self.z);
		
	def __add__(self, v):
		return Vector3d(self.x + v.x, self.y + v.y, self.z + v.z);
		
	def __sub__(self, v):
		return Vector3d(self.x - v.x, self.y - v.y, self.z - v.z);
	
	# 	@param number arbitrary real number
	def __mul__(self, number):
		return Vector3d(self.x * number, self.y * number, self.z * number);
	
	def __truediv__(self, number):
		return Vector3d(self.x / number, self.y / number, self.z / number);
		
	def __neg__(self):
		return Vector3d(-self.x, -self.y, - self.z)
		
	def toDict(self):
		return {'x': self.x, 'y': self.y, 'z': self.z }
				
	def __eq__(self, v):
		if type(self) != type(v):
			return False;
		return self.x == v.x and self.y == v.y and self.z == v.z;
	
	def __neq__(self, v):
		return not (self == v);
	
	def toPoint(self):
		return Point(self.x, self.y);

class Vector4d:
	def __init__(self, x, y, z, w):
		self.x = x;
		self.y = y;
		self.z = z;
		self.w = w;
	def __repr__(self):
		return "Vector4d(%f, %f, %f, %f)" % (self.x, self.y, self.z, self.w);
		
	def __add__(self, v):
		return Vector4d(self.x + v.x, self.y + v.y, self.z + v.z, self.w + v.w);
		
	def __sub__(self, v):
		return Vector4d(self.x - v.x, self.y - v.y, self.z - v.z,self.w - v.w);
	
	# 	@param number arbitrary real number
	def __mul__(self, number):
		return Vector4d(self.x * number, self.y * number, self.z * number, self.w*number);
		
	def __neg__(self):
		return Vector4d(-self.x, -self.y, - self.z, -self.w)
		
	def toDict(self):
		return {'x': self.x, 'y': self.y, 'z': self.z, 'w': self.w }
				
	def __eq__(self, v):
		if type(self) != type(v):
			return False;
		return self.x == v.x and self.y == v.y and self.z == v.z and self.w == v.w;
	
	def __neq__(self, v):
		return not (self == v);
	
	def toPoint(self):
		return Point(self.x, self.y);
	
	def toVector3d(self):
		return Vector3d(self.x, self.y, self.z);