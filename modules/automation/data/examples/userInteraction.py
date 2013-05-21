# first, include the module
from amir import io;

def main():
	# start with a simple message box
	io.alert("This is the user interaction demo");
	
	# request a choice
	if not io.requestYesNo("Do you wish to do the demo?"):
		# proceed anyway ;-) show message to the user, alter the button text
		io.alert("too bad, no canceling possible", "Okay then");
		
	# start a complex user request object
	request = io.UserRequest("please provide some data");
	# add a string input: message, default value(optional)
	request.addString("your name", "Embedded System"); 
	# add an integer: message, default(optional), min(optional), max(optional)
	request.addInteger("your age", 30, 1, 120); 
	# add an bool input: message, default(optional), yesName(optional) noName(optional)
	request.addBool("Are you married?", False, "Yep", "Nope");
	#add a floating point input: message, default(optinal)
	request.addFloat("enter PI", 3.14);
	
	#now, add some buttons
	BuyChoice = 1
	
	# add button: name, id(optional)
	request.addButton("I want to buy", BuyChoice);
	request.addButton("Abort");
	
	# now show the request, the method waits for the user input
	buttonId = request.execute();
	
	if buttonId == BuyChoice:
		print(request.values());
	else:
		print("aborted");

if __name__ == '__main__':
	main();