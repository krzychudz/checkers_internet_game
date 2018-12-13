#pragma once
#include "GameState.h"
#include <SFML/Graphics.hpp>
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

