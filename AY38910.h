#pragma once

#include "PSG.h"

namespace m2d
{
namespace STM32
{
	namespace PSG
	{
		class AY38910 : Interface
		{
			enum Register : uint8_t
			{
				ChannelFineTune = 0x00,
				ChannelCoarseTune = 0x01,
				ChannelA_FineTune = 0x00,
				ChannelA_CoarseTune = 0x01,
				ChannelB_FineTune = 0x02,
				ChannelB_CoarseTune = 0x03,
				ChannelC_FineTune = 0x04,
				ChannelC_CoarseTune = 0x05,
				NoisePeriod = 0x06,
				MixerControl = 0x07,
				ChannelAmplitude = 0x08,
				ChannelA_Amplitude = 0x08,
				ChannelB_Amplitude = 0x09,
				ChannelC_Amplitude = 0x0a,
				EnvelopeFineTune = 0x0b,
				EnvelopeCoarseTune = 0x0c,
				EnvelopeShapeCycle = 0x0d,
				IOPortA_DataStore = 0x0e,
				IOPortB_DataStore = 0x0f,
			};

		private:
			uint8_t latchPin, clockPin, dataPin, BC1Pin, BCDIRPin, RSTPin;
			unsigned int tp_ay[128] = {
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4050, 3822, 3608, 3405, 3214, 3034, 2863, 2703, 2551, 2408, 2273, 2145, 2025, 1911, 1804, 1703, 1607, 1517, 1432, 1351, 1276, 1204, 1136, 1073, 1012, 956, 902, 851, 804, 758, 716, 676, 638, 602, 568, 536, 506, 478, 451, 426, 402, 379, 358, 338, 319, 301, 284, 268, 253, 239, 225, 213, 201, 190, 179, 169, 159, 150, 142, 134, 127, 119, 113, 106, 100, 95, 89, 84, 80, 75, 71, 67, 63, 60, 56, 53, 50, 47, 45, 42, 40, 38, 36, 34, 32, 30, 28, 27, 25, 24, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 13, 12, 11, 11, 10
			};

		public:
			enum EnvelopeMode : uint8_t
			{
				ModeA = 0x00,
				ModeB = 0x04,
				ModeC = 0x08,
				ModeD = 0x09,
				ModeE = 0x0a,
				ModeF = 0x0b,
				ModeG = 0x0c,
				ModeH = 0x0d,
				ModeI = 0x0e,
				ModeJ = 0x0f
			};

			enum SettingBit : uint8_t
			{
				ChannelA = 0x01,
				ChannelB = 0x02,
				ChannelC = 0x04,
			};

			AY38910(uint8_t latch, uint8_t clock, uint8_t data, uint8_t BC1, uint8_t BCDIR, uint8_t RST)
			    : latchPin(latch)
			    , clockPin(clock)
			    , dataPin(data)
			    , BC1Pin(BC1)
			    , BCDIRPin(BCDIR)
			    , RSTPin(RST)
			{
				pinMode(latchPin, OUTPUT);
				pinMode(clockPin, OUTPUT);
				pinMode(dataPin, OUTPUT);
				pinMode(BC1Pin, OUTPUT);
				pinMode(BCDIRPin, OUTPUT);
				pinMode(RSTPin, OUTPUT);

				digitalWrite(RSTPin, 0);
				delayMicroseconds(20);
				digitalWrite(RSTPin, 1);

				this->setNoise(0x00);
				this->setVolume(PSG::Channel::Channel1, 0);
				this->setVolume(PSG::Channel::Channel2, 0);
				this->setVolume(PSG::Channel::Channel3, 0);

				this->setMixerSetting(0, 0, (SettingBit::ChannelA | SettingBit::ChannelB | SettingBit::ChannelC));
			}

			bool validateChannel(uint8_t channel)
			{
				return channel < 3;
			}

			void setNote(PSG::Channel channel, uint8_t noteNumber)
			{
				unsigned int t = this->tp_ay[noteNumber];
				writeData(Register::ChannelFineTune + channel, t & 0xff);
				writeData(Register::ChannelCoarseTune + channel, (t >> 8) & 0x0f);
			}

			void setNoise(uint8_t noisePeriod)
			{
				writeData(Register::NoisePeriod, noisePeriod);
			}

			void setVolume(PSG::Channel channel, uint8_t volume)
			{
				uint8_t volume_bit = (uint8_t)map(volume, 0, 127, 0, 15);
				writeData(Register::ChannelAmplitude + channel, volume_bit);
			}

			void setEnvelope(PSG::Channel channel, AY38910::EnvelopeMode mode)
			{
				writeData(Register::EnvelopeShapeCycle, mode);
			}

			void setEnvelopeTime(uint16_t envelopeTime)
			{
				writeData(Register::EnvelopeFineTune, envelopeTime & 0xff);
				writeData(Register::EnvelopeCoarseTune, (envelopeTime >> 8) & 0xff);
			}

			void setMixerSetting(uint8_t inputEnabled, uint8_t noiseEnabled, uint8_t toneEnabled)
			{
				uint8_t data = ((~inputEnabled & 0x03) << 6) | ((~inputEnabled & 0x07) << 3) | (~toneEnabled & 0x07);
				writeData(Register::MixerControl, data);
			}

			void latchMode()
			{
				digitalWrite(BC1Pin, 1);
				delayMicroseconds(10);
				digitalWrite(BCDIRPin, 1);
				delayMicroseconds(10);
			}

			void writeMode()
			{
				digitalWrite(BC1Pin, 0);
				delayMicroseconds(10);
				digitalWrite(BCDIRPin, 1);
				delayMicroseconds(10);
			}

			void inactive()
			{
				digitalWrite(BC1Pin, 0);
				delayMicroseconds(10);
				digitalWrite(BCDIRPin, 0);
				delayMicroseconds(10);
			}

			void writeData(uint8_t address, uint8_t data)
			{
				inactive();
				digitalWrite(latchPin, 0);
				shiftOut(dataPin, clockPin, MSBFIRST, address);
				digitalWrite(latchPin, 1);
				latchMode();

				inactive();
				writeMode();
				digitalWrite(latchPin, 0);
				shiftOut(dataPin, clockPin, MSBFIRST, data);
				digitalWrite(latchPin, 1);
			}
		};
	}
}
}