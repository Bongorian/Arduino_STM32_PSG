#ifndef SAA1099_h
#define SAA1099_h
#include "Arduino.h"

class SAA1099
{
public:
	SAA1099(int latch, int clock, int data, int WE, int CS, int AZ);
	void setNote(byte channel, byte notenum);
	void setVolume(byte channel, byte volume, byte side);
	void setFreq(byte channel, byte freq);
	void setOctave(byte channel, byte octave);
	void setFreqEnable(byte channelbit);
	void setNoiseEnable(byte channelbit);
	void setNoise(byte noiseChannel, byte mode);
	void setEnvelope(byte envelopeChannel, byte mode);
	void soundEnable();
	void mute();
	void reset();
	void modeWrite();
	void modeInactive();
	void writeData(byte address, byte data);

private:
	int latchPin, clockPin, dataPin, WEPin, CSPin, azPin;
};

#endif
