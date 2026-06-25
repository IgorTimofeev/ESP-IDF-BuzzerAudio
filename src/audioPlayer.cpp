#include <audioPlayer.h>

#include <esp_err.h>
#include <esp_timer.h>
#include <esp_log.h>

#include <sound.h>

namespace YOBA {
	AudioPlayer::AudioPlayer(Buzzer* buzzer): _buzzer(buzzer) {

	}

	void AudioPlayer::setup() {
		_buzzer->setup();

		xTaskCreate(
			[](void* arg) {
				static_cast<AudioPlayer*>(arg)->onStart();
			},
			"AudioPlayer",
			4 * 1024,
			this,
			1,
			nullptr
		);
	}
	
	void AudioPlayer::onStart() {
		_playSemaphore = xSemaphoreCreateBinary();

		while (true) {
			// Waiting for sound
			if (!_sound)
				xSemaphoreTake(_playSemaphore, portMAX_DELAY);
			
			if (!_sound) {
				ESP_LOGE("AudioPlayer", "sound is null");
				continue;
			}
			else if (esp_timer_get_time() < _playableDeadline) {
				taskYIELD();
				continue;
			}

			if (_playableIndex < _sound->getPlayablesLength()) {
				const auto playable = _sound->getPlayables()[_playableIndex];
				
				if (playable->isDelay()) {
					_buzzer->setState(false);
				}
				else {
					const auto note = reinterpret_cast<const Note*>(playable);
					
					_buzzer->setFrequency(note->getFrequency());
					_buzzer->setState(true);
				}
				
				_playableIndex = _playableIndex + 1;
				_playableDeadline = esp_timer_get_time() + playable->getDuration();
			}
			else {
				// Turning off speaker
				_sound = nullptr;
				_playableIndex = 0;
				_playableDeadline = 0;
				
				_buzzer->setState(false);
			}
			
			taskYIELD();
		}
	}
	
	void AudioPlayer::play(const Sound* sound) {
		_sound = sound;
		_playableIndex = 0;
		_playableDeadline = 0;
		
		xSemaphoreGive(_playSemaphore);
	}
}