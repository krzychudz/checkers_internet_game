#pragma once
#include "GameState.h"
#include <SFML/Graphics.hpp>

/*
	Klasa obs�uguj�ca stan prezentacji b��d�w. Ta klasa wy�weitli ekran informuj�cy o np. zerwaniu po��czenia z serwerem.
*/

class ServerErrorState :
	public GameState
{
public:
	ServerErrorState(Game* game, std::string text);
	~ServerErrorState();

	virtual void draw();
	virtual void update();
	virtual void handleInput();

private:
	Font font;
	Text errorInfo;
	Text returnToMainMenu;


};

