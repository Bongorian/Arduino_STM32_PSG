#include <Arduino_STM32_PSG.h>

using namespace m2d::STM32::PSG;

auto psg = SN76489(1, 2, 3, 4, 5);
// auto psg = SAA1099(GPIO_NUM_19, GPIO_NUM_18, GPIO_NUM_23, GPIO_NUM_16, GPIO_NUM_17, GPIO_NUM_32);
// auto psg = AY38910(GPIO_NUM_19, GPIO_NUM_18, GPIO_NUM_23, GPIO_NUM_16, GPIO_NUM_17, GPIO_NUM_32);

void setup()
{
	// put your setup code here, to run once:
	Serial.begin(9600);
}

void loop()
{
	// put your main code here, to run repeatedly:
	Serial.println("54");
	psg.setNote(0, 54);
	delay(1000);
	Serial.println("51");
	psg.setNote(0, 51);
	delay(1000);
}