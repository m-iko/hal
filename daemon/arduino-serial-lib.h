/*
Thanks to https://github.com/todbot/arduino-serial

The MIT License (MIT)

Copyright (c) 2014 Tod E. Kurt

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

arduino-serial-lib -- simple library for reading/writing serial ports
2006-2013, Tod E. Kurt, http://todbot.com/blog/
*/


#ifndef __ARDUINO_SERIAL_LIB_H__
#define __ARDUINO_SERIAL_LIB_H__

#if defined __cplusplus
extern "C" {
#endif

	#include <stdint.h>   // Standard types 
	#include <stdbool.h>

	int serialport_init(const char* serialport, int baud);
	int serialport_close(int fd);
	int serialport_writebyte( int fd, uint8_t b);
	int serialport_write(int fd, const unsigned char* str);
	bool serialport_read_until(int fd, char* buf, char until, int buf_max, int timeout_ms);
	int serialport_flush(int fd);

#if defined __cplusplus
}
#endif

#endif

