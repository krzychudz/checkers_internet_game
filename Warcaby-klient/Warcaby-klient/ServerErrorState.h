#pragma once
#include "GameState.h"
#include <SFML/Graphics.hpp>

/*
	Klasa obs³uguj¹ca stan prezentacji b³êdów. Ta klasa wyœweitli ekran informuj¹cy o np. zerwaniu po³¹czenia z serwerem.
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

