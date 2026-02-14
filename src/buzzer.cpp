#include "buzzer.h"

#include <algorithm>

#include <esp_err.h>

namespace YOBA {
	Buzzer::Buzzer(const gpio_num_t pin, const ledc_channel_t LEDCChannel, const ledc_timer_t LEDCTimer, const ledc_mode_t LEDCMode):
		_pin(pin),
		_LEDCChannel(LEDCChannel),
		_LEDCTimer(LEDCTimer),
		_LEDCMode(LEDCMode)
	{

	}

	void Buzzer::setup() const {
		ledc_timer_config_t timerConfig {};
		timerConfig.speed_mode = _LEDCMode;
		timerConfig.duty_resolution = static_cast<ledc_timer_bit_t>(_resolutionBits);
		timerConfig.timer_num = _LEDCTimer;
		timerConfig.freq_hz = 4000;
		timerConfig.clk_cfg = LEDC_AUTO_CLK;
		ESP_ERROR_CHECK(ledc_timer_config(&timerConfig));

		ledc_channel_config_t channelConfig {};
		channelConfig.gpio_num = _pin;
		channelConfig.speed_mode = _LEDCMode;
		channelConfig.channel = _LEDCChannel;
		channelConfig.intr_type = LEDC_INTR_DISABLE;
		channelConfig.timer_sel = _LEDCTimer;
		channelConfig.duty = 0;
		channelConfig.hpoint = 0;
		ESP_ERROR_CHECK(ledc_channel_config(&channelConfig));
	}

	void Buzzer::setState(const bool value) const {
		//				ESP_LOGI("Speaker", "setDuty: %d", value);

		ESP_ERROR_CHECK_WITHOUT_ABORT(ledc_set_duty(_LEDCMode, _LEDCChannel, value ? _dutyMax : 0));
		ESP_ERROR_CHECK_WITHOUT_ABORT(ledc_update_duty(_LEDCMode, _LEDCChannel));
	}

	void Buzzer::setFrequency(const uint32_t value) const {
		//				ESP_LOGI("Speaker", "setFrequency: %d", value);

		ESP_ERROR_CHECK_WITHOUT_ABORT(ledc_set_freq(
			_LEDCMode,
			_LEDCTimer,
			std::clamp<uint32_t>(value, 10, _frequencyMax)
		));
	}
}
