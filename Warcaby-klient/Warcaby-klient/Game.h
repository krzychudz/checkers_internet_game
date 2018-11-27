#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "GameState.h"

using namespace sf;

class GameState;

class Game
{
public:
	Game();
	~Game();

	void pushState(GameState* state);
	void popState();

	GameState* CurrentState();

	void gameLoop();

	RenderWindow window;

protected:
	std::vector<GameState*> states;
};

