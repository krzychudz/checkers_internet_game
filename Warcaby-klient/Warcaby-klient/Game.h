#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "GameState.h"
#include "Constants.h"

using namespace sf;

class GameState;

/*
	Klasa ob³uguj¹ca ca³¹ oplikacjê. Tutaj wykonywana jest pêtla g³ówna która rysuje, aktualizuje aktualny stan gry.
*/


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

