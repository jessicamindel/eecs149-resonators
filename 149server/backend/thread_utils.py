from threading import Event, Thread

def launch_thread(func, *args):
	stopped = Event()
	def loop():
		while not stopped.isSet():
			func(*args)
	Thread(target=loop, daemon=True).start()
	return stopped.set