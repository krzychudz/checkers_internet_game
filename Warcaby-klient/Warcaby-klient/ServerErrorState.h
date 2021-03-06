#pragma once
#include "GameState.h"
#include <SFML/Graphics.hpp>

/*
	Klasa obsługująca stan prezentacji błędów. Ta klasa wyśweitli ekran informujący o np. zerwaniu połączenia z serwerem.
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

