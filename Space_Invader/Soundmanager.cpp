#include "Soundmanager.h"

namespace Soundmanager {
	soundSet sounds;

	void load() {
		sounds.explosion = LoadSound("sounds\\exploding.mp3");
		sounds.laserS = LoadSound("sounds\\lightlaser.mp3");
		sounds.laserM = LoadSound("sounds\\mediumlaser.mp3");
		sounds.laserL = LoadSound("sounds\\heavylaser.mp3");
	}

	void unload() {
		UnloadSound(sounds.explosion);
		UnloadSound(sounds.laserS);
		UnloadSound(sounds.laserM);
		UnloadSound(sounds.laserL);
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