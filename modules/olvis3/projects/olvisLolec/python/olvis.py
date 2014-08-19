from amir import rc;

class Filter:
	def __init__(self, olvis, name):
		self._name = name;
		self._olvis = olvis;
	
	def getPort(self, name):
		return Port(self, name);
		
	def getPortValue(self, portName):
		return self._olvis.getPortValue(self._name, portName);
	
	def setPortValue(self, portName, value):
		return self._olvis.setPortValue(self._name, portName, value);

	def name(self):
		return self._name
		
	def __repr__(self):
		return "Filter(" + self._name + ")";
	
class Port:
	def __init__(self, filter, name):
		self._filter = filter;
		self._name = name;
		
	def getValue(self):
		return self._filter.getPortValue(self._name);

	def setValue(self, value):
		return self._filter.setPortValue(self._name, value);

	def __repr__(self):
		return "Port(" + self._filter.name() + "::" + self._name + ")";		

class Processor:
	def __init__(self, olvis, name):
		self._name = name;
		self._olvis = olvis;
	def pause(self):
		self._olvis.pauseProcessor(self._name);
	def resume(self):
		self._olvis.resumeProcessor(self._name);
	def __repr__(self):
		return "Processor(" + self._name + ")";
		
	

class Olvis(rc.RcUnit):
	def __init__(self):
		rc.RcUnit.__init__(self, "olvis");
	
	def getFilter(self, name):
		return Filter(self, name);
	
	def getProcessor(self, name):
		return Processor(self, name);
	
