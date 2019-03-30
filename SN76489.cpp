#include "SN76489.h"
#include "arduino.h"
unsigned int tp_sn[] = {
	956, 902, 851, 804, 758, 716, 676, 638, 602, 568, 536, 506, 478, 451, 426, 402, 379, 358, 338, 319, 301, 284, 268, 253, 239, 225, 213, 201, 190, 179, 169, 159, 150, 142, 134, 127, 119, 113, 106, 100, 95, 89, 84, 80, 75, 71, 6, 1012, 956, 902, 851, 804, 758, 716, 676, 638, 602, 568, 536, 506, 478, 451, 426, 402, 379, 358, 338, 319, 301, 284, 268, 253, 239, 225, 213, 201, 190, 179, 169, 159, 150, 142, 134, 127, 119, 113, 106, 100, 95, 89, 84, 80, 75, 71, 67, 63, 60, 56, 53, 50, 47, 45, 42, 40, 38, 36, 34, 32, 30, 28, 27, 25, 24, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 13, 12, 11, 11, 10
};

SN76489::SN76489(int latch, int clock, int data, int WE, int CE)
    : latchPin(latch)
    , clockPin(clock)
    , dataPin(data)
    , WEPin(WE)
    , CEPin(CE)
{
	pinMode(latchPin, OUTPUT);
	pinMode(clockPin, OUTPUT);
	pinMode(dataPin, OUTPUT);
	pinMode(WEPin, OUTPUT);
	pinMode(CEPin, OUTPUT);
}

void SN76489::setNote(byte channel, byte notenum)
{
	unsigned int t = tp_sn[notenum];
	byte tonedata1 = 0x80 | (channel << 5) | (t & 0xf);
	writeData(tonedata1);
	byte tonedata2 = (t >> 4) & 0x3f;
	writeData(tonedata2);
}

void SN76489::setVolume(byte channel, byte volume)
{
	byte vol;
	vol = 0x90 | (channel << 5) | ((~volume) & 0xf);
	writeData(vol);
}

void SN76489::SetNoise(byte fb, byte f1, byte f0)
{
	byte data = 0xd0 | (fb << 2) | (f1 << 1) | f0;
	writeData(data);
}

void SN76489::modeWrite()
{
	digitalWrite(CEPin, LOW);
	digitalWrite(WEPin, LOW);
	delayMicroseconds(20);
}

void SN76489::modeInactive()
{
	digitalWrite(CEPin, HIGH);
	digitalWrite(WEPin, HIGH);
	delayMicroseconds(20);
}

void SN76489::writeData(byte data)
{
	modeWrite();
	digitalWrite(latchPin, LOW);
	shiftOut(dataPin, clockPin, LSBFIRST, data);
	digitalWrite(latchPin, HIGH);
	delayMicroseconds(100);
	modeInactive();
}

void SN76489::clear()
{
	setVolume(0, 0);
	setVolume(1, 0);
	setVolume(2, 0);
	setVolume(3, 0);
}
