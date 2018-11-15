#ifndef SAA1099_h
#define SAA1099_h
#include "Arduino.h"

class SAA1099
{
public:
  SAA1099(int latch, int clock, int data, int WE, int CS, int AZ);
  void SetNote(byte channel, byte notenum);
  void SetVolume(byte channel, byte volume, byte side);
  void SetFreq(byte channel, byte freq);
  void SetOctave(byte channel, byte octave);
  void SetFreqEnable(byte channelbit);
  void SetNoiseEnable(byte channelbit);
  void SetNoise(byte noisechannel, byte mode);
  void SetEnvelope(byte envelopechannel, byte mode);
  void SoundEnable();
  void Mute();
  void Reset();
  void mode_write();
  void mode_inactive();
  void write_data(byte address, byte data);

private:
  int latchPin, clockPin, dataPin, WEPin, CSPin, azPin;
};

#endif
