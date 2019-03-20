#include "arduino.h"
#include "SAA1099.h"

SAA1099::SAA1099(int latch, int clock, int data, int WE, int CS, int AZ)
    : latchPin(latch), clockPin(clock), dataPin(data), WEPin(WE), CSPin(CS), azPin(AZ)
{
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(WEPin, OUTPUT);
  pinMode(CSPin, OUTPUT);
  pinMode(azPin, OUTPUT);
  digitalWrite(CSPin, LOW);
  Reset();
}
void SAA1099::SetNote(byte channel, byte notenum)
{
  SetFreqEnable((1 << channel) & 0xff);
  int o = (notenum + 1) / 12 - 2;
  int f = ((notenum + 1) % 12) * 21.2;
  SetOctave(channel, o);
  SetFreq(channel, f);
}

void SAA1099::SetVolume(byte channel, byte volume, byte side)
{
  byte volL = 0xff & volume;
  byte volR = 0xff & (volume << 4);
  switch (side)
  {
  case 0:
    write_data(channel, volL);
    break;
  case 1:
    write_data(channel, volR);
    break;
  case 2:
    write_data(channel, volume);
    break;
  }
}

void SAA1099::SetFreq(byte channel, byte freq)
{
  write_data((0x08 | channel), freq);
}

void SAA1099::SetOctave(byte channel, byte octave)
{
  switch (channel)
  {
  case 0:
    write_data(0x10, octave);
    break;
  case 1:
    write_data(0x10, octave << 4 & 0xf0);
    break;
  case 2:
    write_data(0x11, octave);
    break;
  case 3:
    write_data(0x11, octave << 4 & 0xf0);
    break;
  case 4:
    write_data(0x12, octave);
    break;
  case 5:
    write_data(0x12, octave << 4 & 0xf0);
    break;
  }
}

void SAA1099::SetFreqEnable(byte channelbit)
{
  write_data(0x14, channelbit & 0xff);
}

void SAA1099::SetNoiseEnable(byte channelbit)
{
  write_data(0x15, channelbit & 0xff);
}

void SAA1099::SetNoise(byte noisechannel, byte mode)
{
  write_data(0x16, mode << ((noisechannel << 2) & 0x33));
}

void SAA1099::SetEnvelope(byte envelopechannel, byte mode)
{
  write_data(0x18 + envelopechannel, mode);
}

void SAA1099::SoundEnable()
{
  write_data(0x1C, 0x01);
}

void SAA1099::Mute()
{
  write_data(0x1C, 0x00);
}

void SAA1099::Reset()
{
  write_data(0x1C, 0x02);
}

void SAA1099::mode_write()
{
  digitalWrite(WEPin, LOW);
}

void SAA1099::mode_inactive()
{
  digitalWrite(WEPin, HIGH);
}

void SAA1099::write_data(byte address, byte data)
{
  digitalWrite(azPin, HIGH);
  mode_write();
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, address);
  digitalWrite(latchPin, HIGH);
  mode_inactive();
  digitalWrite(azPin, LOW);
  mode_write();
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, data);
  digitalWrite(latchPin, HIGH);
  mode_inactive();
}
