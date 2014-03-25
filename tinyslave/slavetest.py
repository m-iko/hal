from serial import Serial
from time import sleep
from math import sin, pi

class TinySlave(Serial):
	def __sendCommand(self, command, chunksize=32):
		for i in range(0, len(command), chunksize):
			chunk = command[i:i+chunksize]
			print "[%2d] Writing "%(i), map(ord, chunk)
			self.write(chunk)
			sleep(0.1)    # Do not fullfil the incoming buffer of ATTiny
		return self.read(1) == command[0]

	def changeFPS(self, anim_id, fps):
		cmd = '#' + chr(anim_id) + chr(fps)
		return self.__sendCommand(cmd)

	def uploadAnim(self, anim_id, frames):
		cmd = 'U' + chr(anim_id) + chr(len(frames)) + ''.join(map(chr, frames))
		return self.__sendCommand(cmd)

def sinusoid(n):
	return [int(127*(1+sin(2*pi*i/n))) for i in range(n)]

if __name__ == "__main__":
	tiny = TinySlave("/dev/ttyACM1", 115200)
	frames = sinusoid(29)
	
	tiny.uploadAnim(0, frames)
	sleep(1)
	tiny.changeFPS(0, 16)
