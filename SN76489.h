#ifndef SN76489_h
#define SN76489_h
#include "Arduino.h"

class SN76489
{
public:
  SN76489(int latch, int clock, int data, int WE, int CE);
  void SetNote(byte channel, byte notenum);
  void SetVolume(byte channel, byte volume);
  void SetNoise(byte fb, byte f1, byte f0);
  void mode_write();
  void mode_inactive();
  void write_data(byte data);
  void clear();

private:
  int latchPin, clockPin, dataPin, WEPin, CEPin;
};

#endif
