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

import offisio;
import sys;

class UserRequest:
	def __init__(self, title):
		self._title = title;
		self._requests = []
		self._buttons = []
		self._values = []
	def addString(self, desc, default = "", choices = list()):
		self._requests.append(("s", (str(desc), str(default), choices)))
	def addInteger(self, desc, default = 0, minVal = -sys.maxsize-1, maxVal = sys.maxsize, step = 1):
		self._requests.append(("i", (str(desc), int(default), int(minVal), int(maxVal), int(step))))
	def addFloat(self, desc, default = 0.0):
		self._requests.append(("d", (str(desc), float(default))))
	def addBool(self, desc, default = False, trueName = 'Yes', falseName = 'No'):
		self._requests.append(("b", (str(desc), bool(default), str(trueName), str(falseName))))
	def addChoice(self, desc, names, values, defaultValue = None):
		self._requests.append(("c", (str(desc), names, values, defaultValue)))
	def addInputFile(self, desc, default = ""):
		self._requests.append(("F", (str(desc), str(default), list())))
	def addOutputFile(self, desc, default = ""):
		self._requests.append(("f", (str(desc), str(default), list())))
	def addDirectory(self, desc, default = ""):
		self._requests.append(("D", (str(desc), str(default), list())))		
	def addButton(self, name, buttonId = 0):
		self._buttons.append((str(name), int(buttonId)));
	def execute(self, buttonText = None):
		if buttonText != None:
			self.addButton(buttonText)
		if len(self._buttons) == 0:
			if len(self._requests) == 0:
				self.addButton("Ok")
			else:
				self.addButton("Submit")
		val = offisio.userRequest(self, self._title, self._requests, self._buttons);
		if val == None:
			return None;
		self._values = val[1];
		return val[0];
	def values(self):
		return self._values;
	def abort(self):
		offisio.abortUserRequest(self);


def alert(text, buttonText = "Ok"):
	r = UserRequest(text);
	r.execute(buttonText);

def requestYesNo(text, yes = "Yes", no = "No"):
	r = UserRequest(text);
	r.addButton(yes, 1);
	r.addButton(no, 0);
	return r.execute();

def clearInfo():
	offisio.clearInfo();
	
def appendInfo(text):
	offisio.appendInfo(str(text));

def setInfo(text):
	clearInfo();
	appendInfo(text);
	
## Class for displaying ProgressBars to the Clients
## the minimum progress is 0.0, the maximum progress is 100.0
class ProgressBar:
    
    ## constructor
    ##@param name (String) Name of the progress bar
	def __init__(self, name = "ProgressBar", maximum = 100):
		self.id = offisio.createProgressBar(name, int(maximum))
		self.progress = 0.0
		self.active = True
    
    ## sets the progress
    ##@param progress (double) The new progress value 0.0 to 100.0
	def setProgress(self, progress):
		self.progress = progress
		offisio.updateProgressBar(self.id, int(self.progress))
    
    ## sets the progress
    ##@param progress (double) The new progress value 0.0 to 100.0
    ## alias for @see(setProgress)
	def set(self, progress):
		self.setProgress(progress)
    
    ## sets the progress
    ##@param progress (double) The new progress value 0.0 to 100.0
    ## alias for @see(setProgress)
	def setValue(self, progress):
		self.setProgress(progress)
        
    ## sets the progress
    ##@param progress (double) The new progress value 0.0 to 100.0
    ## alias for @see(setProgress)
	def update(self, progress):
		self.setProgress(progress)
    
    ## removes the progress bars form the clients
	def close(self):
		if self.active:
			offisio.removeProgressBar(self.id)
			self.active = False
			    
    ## destructor - automatically removes the progress bar
	def __del__(self):
		self.close()
	
class InfoPanel:
	def __init__(self, title = 'Info'):
		self._title = title;
		self._values = [];
		self._names = [];
		self._id = None;
		
	def add(self, name, value):
		self._values.append(str(value));
		self._names.append(str(name));

	def show(self):
		self._id = offisio.createInfoPanel(self._title, self._names);
		self.updateAll(self._values);
		
	def updateAll(self, values):
		if self._id == None:
			return;
		self._values = [];
		for val in values:
			self._values.append(str(val));
		offisio.updateInfoPanel(self._id, self._values);

	def updateOne(self, position, value):
		self._values[position] = str(value);
		offisio.updateInfoPanel(self._id, self._values);
		
	def close(self):
		if self._id == None:
			return;
		offisio.removeInfoPanel(self._id);
		self._id = None;

	def __del__(self):
		self.close()
		
def startVideoRecording(fps = 15):
	offisio.startVideoCapture(int(fps));

def quitVideoRecording():
	offisio.endVideoCapture("");
	
def finishVideoRecording(filename):
	offisio.endVideoCapture(str(filename));
	
def saveScreenshot(filename):
	offisio.saveScreenshot(str(filename));