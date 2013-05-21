#
# please review the unserInteraction, progressBar and threadExecution demos first


from amir import io, util;
import time

def main():
	# first, setup a user request
	request = io.UserRequest("Hurry, time is running!");
	request.addFloat("15*22.5-133.2");
	result = 15*22.5-133.2;
	# execute the request in an extra thread
	
	threaded = util.ThreadExecute(request.execute);
	
	# give the user 10 seconds to answer
	duration = 10;
	
	# create a progress bar
	progress = io.ProgressBar("seconds elapsed", duration);
	
	hasAnswered = False;
	
	for i in range(duration):
		time.sleep(1);
		progress.setProgress(i+1);
		# check if the thread is finished: if so the user clicked
		if threaded.finished():
			hasAnswered = True;
			break;
	
	if not hasAnswered:
		request.abort(); # hides the user input screen
		io.alert("Sorry, time is up");
	else:
		if request.values()[0] == result:
			io.alert("Amazing, your a genius", "Yeae");
		else:
			io.alert("Sorry, wrong answer", "Damn it");
			
if __name__ == '__main__':
	main();