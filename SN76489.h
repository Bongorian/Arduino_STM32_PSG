#ifndef SN76489_h
#define SN76489_h
#include "Arduino.h"

class SN76489
{
public:
	SN76489(int latch, int clock, int data, int WE, int CE);
	void setNote(byte channel, byte notenum);
	void setVolume(byte channel, byte volume);
	void setNoise(byte fb, byte f1, byte f0);
	void modeWrite();
	void modeInactive();
	void writeData(byte data);
	void clear();

private:
	int latchPin, clockPin, dataPin, WEPin, CEPin;
};

#endif
