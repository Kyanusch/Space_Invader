#include "Soundmanager.h"

namespace Soundmanager {
	soundSet sounds;

	void load() {
		sounds.explosion = LoadWave("sounds\\exploding.mp3");
		sounds.laserS = LoadWave("sounds\\lightlaser.mp3");
		sounds.laserM = LoadWave("sounds\\mediumlaser.mp3");
		sounds.laserL = LoadWave("sounds\\heavylaser.mp3");
	}

	void unload() {
		UnloadWave(sounds.explosion);
		UnloadWave(sounds.laserS);
		UnloadWave(sounds.laserM);
		UnloadWave(sounds.laserL);
	}
	void initialize() {		//must be run befor gameround start!
		while (!IsAudioDeviceReady()) {
			InitAudioDevice();
		}
		load();
	}

	void deinitialize() {	//must be run after all
		unload();
		CloseAudioDevice();
	}
}