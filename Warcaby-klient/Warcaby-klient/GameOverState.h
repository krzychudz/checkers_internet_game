#pragma once
#include <SFML/Graphics.hpp>
#include "GameState.h"

using namespace sf;


/*
	Klasa obs�uguj�ca stan zako�czenie rozgrywki na sktuek wygranej lub przegranej gracza. Zajmuje si� wy�wietleniem informacji 
	o wynku gry oraz pozwala powr�t do menu g��wnego i rozpocz�cie zabawy od nowa. 
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

