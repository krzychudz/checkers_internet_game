#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "GameState.h"
#include "Constants.h"

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

	TcpSocket socket;

protected:
	std::vector<GameState*> states;
};

