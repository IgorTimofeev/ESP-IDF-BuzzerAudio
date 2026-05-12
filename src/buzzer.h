#pragma once

#include <driver/gpio.h>
#include <driver/ledc.h>
#include <soc/soc.h>

#include "sound.h"

namespace YOBA {
	class Buzzer {
		public:
			Buzzer(const gpio_num_t pin, const ledc_channel_t LEDCChannel, const ledc_timer_t LEDCTimer = LEDC_TIMER_0, const ledc_mode_t LEDCMode = LEDC_LOW_SPEED_MODE);
			void setup() const;
			void setState(const bool value) const;
			void setFrequency(const uint32_t value) const;

		private:
			constexpr static uint8_t _resolutionBits = 12;
			constexpr static uint32_t _dutyMax = (1 << _resolutionBits) / 2;
			constexpr static uint32_t _frequencyMax = APB_CLK_FREQ / (1 << _resolutionBits);

			gpio_num_t _pin;
			ledc_channel_t _LEDCChannel;
			ledc_timer_t _LEDCTimer;
			ledc_mode_t _LEDCMode;
	};
}