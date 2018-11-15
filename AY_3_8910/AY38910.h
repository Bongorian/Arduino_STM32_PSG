#ifndef AY38910_h
#define AY38910_h
#include "Arduino.h"

class AY38910
{
public:
  AY38910(int latch, int clock, int data, int BC1, int BCDIR);
  void SetNote(byte channel, byte notenum);
  void SetVolume(byte channel, byte volume);
  void SetEnvelope(byte mode);
  void SetEnvelopeTime(byte envelopetime);
  void mode_latch();
  void mode_write();
  void mode_inactive();
  void write_data(byte address, byte data);

private:
  int latchPin, clockPin, dataPin, BC1Pin, BCDIRPin;
};

#endif
