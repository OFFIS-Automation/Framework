# OFFIS Automation Framework
# Copyright (C) 2013-2016 OFFIS e.V.
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

import sys;
import offisio;
import traceback;
class myOut:
	def __init__(self, isErr = False):
		self.isErr = isErr
	def write(self, data):
		if self.isErr:
			offisio.logErr(str(data))
		else:
			offisio.log(str(data))
	def flush(self):
		pass

sys.stdout = myOut()
sys.stderr = myOut(True)
		
def _excepthook(errorType, value, tb):
	stack = traceback.extract_tb(tb)
	name = ("".join(traceback.format_exception_only(errorType, value))).strip()
	offisio.exception(name, stack);


def _exit(code = 0):
	raise SystemError("sys.exit called with code "+ str(code));
	
sys.excepthook = _excepthook
sys.exit = _exit
