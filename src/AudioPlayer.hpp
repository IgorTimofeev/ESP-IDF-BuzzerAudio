#pragma once

#include <stdint.h>

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include <Buzzer.hpp>
#include <Sound.hpp>

namespace YOBA {
	class AudioPlayer {
		public:
			AudioPlayer(Buzzer* buzzer);
			void setup();
			void play(const Sound* sound);

		private:
			Buzzer* _buzzer;

			SemaphoreHandle_t _playSemaphore = nullptr;
			const Sound* _sound = nullptr;
			size_t _playableIndex = 0;
			int64_t _playableDeadline = 0;

			[[noreturn]] void onStart();
	};
}