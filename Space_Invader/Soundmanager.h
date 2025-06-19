#pragma once
#include "raylib.h"

namespace Soundmanager {
	struct soundSet {
		Wave explosion;
		Wave laserS;
		Wave laserM;
		Wave laserL;
	};
	void load();
	void unload();
	extern soundSet sounds;

	void initialize();		//must be run befor gameround start!
	void deinitialize();	//must be run after all
};
