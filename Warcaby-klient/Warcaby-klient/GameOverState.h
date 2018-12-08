#pragma once
#include <SFML/Graphics.hpp>
#include "GameState.h"

using namespace sf;

class GameOverState :
	public GameState
{
public:
	GameOverState(Game* game, bool resultInfo);
	~GameOverState();

	virtual void draw();
	virtual void update();
	virtual void handleInput();

private:
	Font font;
	Text resultText;
	Text returnToMainMenu;
};

