#include "AY38910.h"
#include "arduino.h"
unsigned int tp_ay[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4050, 3822, 3608, 3405, 3214, 3034, 2863, 2703, 2551, 2408, 2273, 2145, 2025, 1911, 1804, 1703, 1607, 1517, 1432, 1351, 1276, 1204, 1136, 1073, 1012, 956, 902, 851, 804, 758, 716, 676, 638, 602, 568, 536, 506, 478, 451, 426, 402, 379, 358, 338, 319, 301, 284, 268, 253, 239, 225, 213, 201, 190, 179, 169, 159, 150, 142, 134, 127, 119, 113, 106, 100, 95, 89, 84, 80, 75, 71, 67, 63, 60, 56, 53, 50, 47, 45, 42, 40, 38, 36, 34, 32, 30, 28, 27, 25, 24, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 13, 12, 11, 11, 10
};

AY38910::AY38910(int latch, int clock, int data, int BC1, int BCDIR)
    : latchPin(latch)
    , clockPin(clock)
    , dataPin(data)
    , BC1Pin(BC1)
    , BCDIRPin(BCDIR)
{
	pinMode(latchPin, OUTPUT);
	pinMode(clockPin, OUTPUT);
	pinMode(dataPin, OUTPUT);
	pinMode(BC1Pin, OUTPUT);
	pinMode(BCDIRPin, OUTPUT);
}

void AY38910::setNote(byte channel, byte notenum)
{
	unsigned int t = tp_ay[notenum];
	writeData(0x00 + channel, t & 0xff);
	writeData(0x01 + channel, (t >> 8) & 0x0f);
}

void AY38910::setVolume(byte channel, byte volume)
{
	writeData(0x08 + channel, volume & 0x0f);
}

void AY38910::setEnvelope(byte mode)
{
	writeData(0x0d, 0x08 + mode & 0x0f);
}

void AY38910::setEnvelopeTime(byte envelopeTime)
{
	writeData(0x0b, envelopeTime & 0xff);
	writeData(0x0c, (envelopeTime >> 8) & 0xff);
}

void AY38910::modeLatch()
{
	digitalWrite(BC1Pin, HIGH);
	digitalWrite(BCDIRPin, HIGH);
}

void AY38910::modeWrite()
{
	digitalWrite(BC1Pin, LOW);
	digitalWrite(BCDIRPin, HIGH);
}

void AY38910::modeInactive()
{
	digitalWrite(BC1Pin, LOW);
	digitalWrite(BCDIRPin, LOW);
}

void AY38910::writeData(byte address, byte data)
{
	modeInactive();
	digitalWrite(latchPin, LOW);
	shiftOut(dataPin, clockPin, MSBFIRST, address);
	digitalWrite(latchPin, HIGH);
	modeLatch();
	modeInactive();
	modeWrite();
	digitalWrite(latchPin, LOW);
	shiftOut(dataPin, clockPin, MSBFIRST, data);
	digitalWrite(latchPin, HIGH);
	modeInactive();
}
