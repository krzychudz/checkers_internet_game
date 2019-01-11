#pragma once
#include <SFML/Graphics.hpp>
#include "GameState.h"

using namespace sf;


/*
	Klasa obs³uguj¹ca stan zakoñczenie rozgrywki na sktuek wygranej lub przegranej gracza. Zajmuje siê wyœwietleniem informacji 
	o wynku gry oraz pozwala powrót do menu g³ównego i rozpoczêcie zabawy od nowa. 
*/

class GameOverState :
	public GameState
{
public:
	GameOverState(Game* game, bool resultInfo, std::string reason);
	~GameOverState();

	virtual void draw();
	virtual void update();
	virtual void handleInput();

private:
	Font font;
	Text resultText;
	Text returnToMainMenu;
	Text reasonText;
};

