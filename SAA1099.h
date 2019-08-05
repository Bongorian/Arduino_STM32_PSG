#pragma once

#include "PSG.h"

namespace m2d
{
namespace STM32
{
	namespace PSG
	{
		class SAA1099 : PSG::Interface
		{
		private:
			uint8_t latchPin, clockPin, dataPin, WEPin, CSPin, azPin;

		public:
			// See http://www.onasticksoftware.co.uk/dragon/saa1099.pdf, page 14
			enum EnvelopeWaveform : uint8_t
			{
				WaveA = 0b0000,
				WaveB = 0b0010,
				WaveC = 0b0100,
				WaveD = 0b0110,
				WaveE = 0b1000,
				WaveF = 0b1010,
				WaveG = 0b1100,
				WaveH = 0b1110,
				WaveI = 0b1111
			};

			enum OutputSide : uint8_t
			{
				Left = 0,
				Right,
				Both
			};

			SAA1099(uint8_t latch, uint8_t clock, uint8_t data, uint8_t WE, uint8_t CS, uint8_t AZ)
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
				digitalWrite(CSPin, 0);
				reset();
			}

			bool validateChannel(uint8_t channel)
			{
				return channel < 6;
			}

			void setNote(PSG::Channel channel, uint8_t noteNumber)
			{
				// Reference: https://github.com/Bobcatmodder/SAATunes
				uint8_t o = (noteNumber / 12) - 1;
				uint8_t note_val = noteNumber - ((o + 1) * 12);
				uint8_t note_addr[] = { 5, 32, 60, 85, 110, 132, 153, 173, 192, 210, 227, 243 };
				setOctave(channel, o);
				setFreq(channel, note_addr[note_val]);
			}

			void setVolume(PSG::Channel channel, SAA1099::OutputSide side, uint8_t volume)
			{
				uint8_t volL = 0xff & volume;
				uint8_t volR = 0xff & (volume << 4);
				switch (side) {
					case Left:
						writeData(channel, volL);
					case Right:
						writeData(channel, volR);
					case Both:
						writeData(channel, volume);
				}
			}

			void setFreq(PSG::Channel channel, uint8_t freq)
			{
				writeData((0x08 | channel), freq);
			}

			void setOctave(PSG::Channel channel, uint8_t octave)
			{
				switch (channel) {
					case Channel1:
						writeData(0x10, octave);
						break;
					case Channel2:
						writeData(0x10, octave << 4 & 0xf0);
						break;
					case Channel3:
						writeData(0x11, octave);
						break;
					case Channel4:
						writeData(0x11, octave << 4 & 0xf0);
						break;
					case Channel5:
						writeData(0x12, octave);
						break;
					case Channel6:
						writeData(0x12, octave << 4 & 0xf0);
						break;
				}
			}

			void setFreqEnable(uint8_t channelbit)
			{
				writeData(0x14, channelbit & 0xff);
			}

			void setNoiseEnable(uint8_t channelbit)
			{
				writeData(0x15, channelbit & 0xff);
			}

			void setNoise(PSG::Channel noiseChannel, uint8_t mode)
			{
				writeData(0x16, mode << ((noiseChannel << 2) & 0x33));
			}

			void setEnvelope(PSG::Channel envelopeChannel, SAA1099::EnvelopeWaveform form)
			{
				writeData(0x18 + envelopeChannel, form);
			}

			void soundEnable()
			{
				writeData(0x1C, 0x01);
			}

			void mute()
			{
				writeData(0x1C, 0x00);
			}

			void reset()
			{
				writeData(0x1C, 0x02);
			}

			void modeWrite()
			{
				digitalWrite(WEPin, 0);
			}

			void modeInactive()
			{
				digitalWrite(WEPin, 1);
			}

			void writeData(uint8_t address, uint8_t data)
			{
				digitalWrite(azPin, 1);
				modeWrite();
				digitalWrite(latchPin, 0);
				shiftOut(dataPin, clockPin, MSBFIRST, address);
				digitalWrite(latchPin, 1);
				modeInactive();
				digitalWrite(azPin, 0);
				modeWrite();
				digitalWrite(latchPin, 0);
				shiftOut(dataPin, clockPin, MSBFIRST, data);
				digitalWrite(latchPin, 1);
				modeInactive();
			}
		};
	}
}
}