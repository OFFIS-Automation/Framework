# first, include the module
from amir import io;
import time;

def main():
	# silly example, print out a word of a string each half second and show progress
	string = "Hello out there, is everything alright? Please wait for this demo to finish."
	parts = string.split(" ");
	
	# provide a name and the total number of steps to the progress bar
	progress = io.ProgressBar("print progress", len(parts));
	for i in range(len(parts)):
		print(parts[i], end=' ');
		# set the progress
		progress.setProgress(i+1);
		time.sleep(0.5);
	print(""); # provide a newline
	io.alert("Finished", "End this silly demo");
	
if __name__ == '__main__':
	main();	