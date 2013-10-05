from serial import Serial
from glob import glob
from time import sleep
from threading import Thread
from math import sqrt
import json

class AmbianceduinoNotFound(Exception):
	pass

class AmbianceduinoReader:
	def __init__(self, device):
		self.serial = serial

	def read_loop(self):
		while self.running:
			line = self.serial.readline().strip()
			if len(line) == 0:
				continue
			if line[0] == '#':
				delay = int(line[1:])
				self.when_delay(delay)
			elif line[0] == '@':
				analogs = json.loads(line[1:])
				self.when_analogs(analogs)
			elif line[0] == '!':
				self.when_error(line[1:])
			elif line[0] == '-':
				self.when_on()
			elif line[0] == '_':
				self.when_off()
			elif line[0] == 'R':
				anim_length = int(line[1:])
				self.when_anim(anim_length)

	def run(self):
		self.running = True
		self.reader = Thread(target=self.read_loop)
		self.reader.start()

	def stop(self):
		self.running = False
		self.reader.join()

	def default_handler(self, *args):
		print ' '.join(map(str, args))

	def when_delay(self, delay):
		self.default_handler("Delay:", delay)

	def when_analogs(self, analogs):
		self.default_handler("Analogs:", analogs)

	def when_anim(self, anim_length):
		self.default_handler("Animation length:", anim_length)

	def when_on(self):
		self.default_handler("Powered on")

	def when_off(self):
		self.default_handler("Powered off")

	def when_error(self, err_string):
		self.default_handler("Error:", err_string)


class Ambianceduino(AmbianceduinoReader):
	DEV_PATTERNS = ["/dev/ttyACM*", "/dev/ttyUSB*"]

	def __try_device(self, device, boot_time):
		self.serial = Serial(device, 115200, timeout=1)
		sleep(boot_time) #Wait arduino boot
		self.__request('?')
		got = self.serial.readline()
		#Magic string
		if "?jesuisuncanapequichante" not in got:
			self.serial.close()
			self.serial = None

	def __request(self, req_bytes):
		self.serial.write(req_bytes)

	def __init__(self, device_path=None, boot_time=2):
		if device_path:
			self.__try_device(device_path, boot_time)
		else:
			possible_devices = [f for pattern in self.DEV_PATTERNS for f in glob(pattern)]
			for device in possible_devices:
				self.__try_device(device, boot_time)
				if self.serial:
					break
		if not self.serial:
			raise AmbianceduinoNotFound("Tried "+str(possible_devices+[device_path]))

	def delay(self, delay=None):
		query = '#'
		if delay in range(0, 256):
			query += chr(delay)
		self.__request(query)

	def analogs(self):
		self.__request('@')

	def upload_anim(self, curve):
		dots = []
		for dot in curve:
			if 0 <= dot < 256: dots.append(chr(dot))
		self.__request('R' + chr(len(dots)) + ''.join(dots))

	def on(self):
		self.__request('-')

	def off(self):
		self.__request('_')

if __name__ ==  "__main__":
	from sys import stdout

	a = Ambianceduino()
	print "Got Ambianceduino"

	a.run() # Start async reader

	a.delay()
	a.delay(10)
	a.analogs()
	a.on()

	sleep(2)

	def fun(i):
		return int(-0.0625*i + 4)

	a.upload_anim(range(255))
	a.delay(50)

	sleep(10)
	a.off()
	a.stop()