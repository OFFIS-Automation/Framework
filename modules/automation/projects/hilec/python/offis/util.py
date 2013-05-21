# OFFIS Automation Framework
# Copyright (C) 2013 OFFIS e.V.
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

﻿import threading;

class ThreadExecute(threading.Thread):
	def __init__(self, target, args = list(), kwargs = dict()):
		threading.Thread.__init__(self)
		self._target = target;
		self._args = args;
		self._kwargs = kwargs;
		self._finished = False;
		self._retVal = None;
		self._lock = threading.Condition();
		self.start();
	def run(self):
		retVal = None;
		try:
			retVal = self._target(*self._args, ** self._kwargs);
		finally: 
			with self._lock:
				self._retVal = retVal;
				self._finished = True
				self._lock.notify();
	
	def finished(self):
		with self._lock:
			return self._finished;
	def wait(self):
		with self._lock:
			if not self._finished:
				self._lock.wait();
	def value(self):
		self.wait();
		with self._lock:
			return self._retVal

