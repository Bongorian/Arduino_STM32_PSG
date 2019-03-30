#include "SAA1099.h"
#include "arduino.h"

#include <math.h>

SAA1099::SAA1099(int latch, int clock, int data, int WE, int CS, int AZ)
    : latchPin(latch)
    , clockPin(clock)
    , dataPin(data)
    , WEPin(WE)
    , CSPin(CS)
    , azPin(AZ)
{
	pinMode(latchPin, OUTPUT);
	pinMode(clockPin, OUTPUT);
	pinMode(dataPin, OUTPUT);
	pinMode(WEPin, OUTPUT);
	pinMode(CSPin, OUTPUT);
	pinMode(azPin, OUTPUT);
	digitalWrite(CSPin, LOW);
	reset();
}
void SAA1099::setNote(byte channel, byte note)
{
	setFreqEnable((1 << channel) & 0xff);
	byte o = (note / 12) - 1;
	byte note_val = note - ((o + 1) * 12);
	byte note_addr[] = { 5, 32, 60, 85, 110, 132, 153, 173, 192, 210, 227, 243 };
	setOctave(channel, o);
	setFreq(channel, note_addr[note_val]);
}

void SAA1099::setVolume(byte channel, byte volume, byte side)
{
	byte volL = 0xff & volume;
	byte volR = 0xff & (volume << 4);
	switch (side) {
		case 0:
			writeData(channel, volL);
			break;
		case 1:
			writeData(channel, volR);
			break;
		case 2:
			writeData(channel, volume);
			break;
	}
}

void SAA1099::setFreq(byte channel, byte freq)
{
	writeData((0x08 | channel), freq);
}

void SAA1099::setOctave(byte channel, byte octave)
{
	switch (channel) {
		case 0:
			writeData(0x10, octave);
			break;
		case 1:
			writeData(0x10, octave << 4 & 0xf0);
			break;
		case 2:
			writeData(0x11, octave);
			break;
		case 3:
			writeData(0x11, octave << 4 & 0xf0);
			break;
		case 4:
			writeData(0x12, octave);
			break;
		case 5:
			writeData(0x12, octave << 4 & 0xf0);
			break;
	}
}

void SAA1099::setFreqEnable(byte channelbit)
{
	writeData(0x14, channelbit & 0xff);
}

void SAA1099::setNoiseEnable(byte channelbit)
{
	writeData(0x15, channelbit & 0xff);
}

void SAA1099::setNoise(byte noisechannel, byte mode)
{
	writeData(0x16, mode << ((noisechannel << 2) & 0x33));
}

void SAA1099::setEnvelope(byte envelopechannel, byte mode)
{
	writeData(0x18 + envelopechannel, mode);
}

void SAA1099::soundEnable()
{
	writeData(0x1C, 0x01);
}

void SAA1099::mute()
{
	writeData(0x1C, 0x00);
}

void SAA1099::reset()
{
	writeData(0x1C, 0x02);
}

void SAA1099::modeWrite()
{
	digitalWrite(WEPin, LOW);
}

void SAA1099::modeInactive()
{
	digitalWrite(WEPin, HIGH);
}

void SAA1099::writeData(byte address, byte data)
{
	digitalWrite(azPin, HIGH);
	modeWrite();
	digitalWrite(latchPin, LOW);
	shiftOut(dataPin, clockPin, MSBFIRST, address);
	digitalWrite(latchPin, HIGH);
	modeInactive();
	digitalWrite(azPin, LOW);
	modeWrite();
	digitalWrite(latchPin, LOW);
	shiftOut(dataPin, clockPin, MSBFIRST, data);
	digitalWrite(latchPin, HIGH);
	modeInactive();
}
