from amir import util, io;
import time;
# this demo shows how to execute time consuming or blocking operations in an extra thread
# review progress bar demo first

def timeConsumingFunction(name = "Thread execution", speed = 1.0):
	bar = io.ProgressBar(name, 20);
	for i in range(20):
		bar.setProgress(i+1);
		time.sleep(0.5/speed);


def main():
	# start with default values in extra thread
	thread1 = util.ThreadExecute(timeConsumingFunction);
	# start with arguments
	thread2 = util.ThreadExecute(timeConsumingFunction, ("Second Thread",));
	# start with keyword arguemnts
	thread3 = util.ThreadExecute(timeConsumingFunction, (), {'name': 'Third thread', 'speed': 2.0});
	
	# wait until all three are finished
	thread1.wait();
	thread2.wait();
	thread3.wait();

if __name__ == '__main__':
	main();