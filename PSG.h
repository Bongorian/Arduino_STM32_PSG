#pragma once

namespace m2d
{
namespace STM32
{
	namespace PSG
	{
		enum Channel : uint8_t
		{
			Channel1 = 0,
			Channel2 = 1,
			Channel3 = 2,
			Channel4 = 3,
			Channel5 = 4,
			Channel6 = 5
		};

		class Interface
		{
		protected:
			virtual bool validateChannel(uint8_t channel) = 0;
		};
	}
}
}