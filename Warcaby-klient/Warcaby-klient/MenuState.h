#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include "GameState.h"


/*
	Klasa obs�uguj�ca stan menu. St�d mo�emy wyj�� z gry lub przej�� do lobby.
*/

class MenuState : public GameState
{
public:
	MenuState(Game* game);
	
	virtual void draw();
	virtual void update();
	virtual void handleInput();

private:
	void loadgame();

	Font font;

	Text title;
	Text newGame;
	Text exit;
	
	int menuPointer = 0; // Wskaznik pozycji w menu

};

