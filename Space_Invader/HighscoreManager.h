#pragma once
#include "raylib.h"
#include <string>
#include <vector>

namespace HighscoreManager {
	struct score {
		std::string name;
		int value;
	};
	const std::string HIGHSCORE_FILE = "highscore.txt";
	const int MAX_HIGHSCORES = 10;

	void initialize();
	void deinitialize();
	std::vector<score> getHighscores();
	int getHighestScore();			// returns the highest score from the highscore list
	void addScore(score newScore);	// adds a new score to the highscore list, if it is high enough
	void updateHighscores();	//saves the highscores to the file
}