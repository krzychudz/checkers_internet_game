#pragma once
#include <SFML/Graphics.hpp>
#include "GameState.h"

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
	

};

