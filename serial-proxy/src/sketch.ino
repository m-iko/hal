/*
 * Util sketch that allows communication with an ATTiny with an Arduino board.
 */

#include <SoftwareSerial.h>

SoftwareSerial com(3, 4);

void setup(){
	Serial.begin(115200);
	com.begin(57600);
}

void loop(){
	if (Serial.available())
		com.write(Serial.read());
	if (com.available())
		Serial.write(com.read());
}
