#ifndef AY38910_h
#define AY38910_h
#include "Arduino.h"

class AY38910
{
public:
	AY38910(int latch, int clock, int data, int BC1, int BCDIR);
	void setNote(byte channel, byte notenum);
	void setVolume(byte channel, byte volume);
	void setEnvelope(byte mode);
	void setEnvelopeTime(byte envelopeTime);
	void modeLatch();
	void modeWrite();
	void modeInactive();
	void writeData(byte address, byte data);

private:
	int latchPin, clockPin, dataPin, BC1Pin, BCDIRPin;
};

#endif
