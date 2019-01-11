#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include "GameState.h"


/*
	Klasa obs³uguj¹ca stan menu. St¹d mo¿emy wyjœæ z gry lub przejœæ do lobby.
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

