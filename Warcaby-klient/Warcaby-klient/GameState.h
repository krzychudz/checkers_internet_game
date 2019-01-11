#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <iostream>
#include "Game.h"

class Game;

using namespace sf;
using namespace std;


/*
	Klasa po której dziedzicz¹ wszytkie inne stany w aplikacji.
*/

class GameState
{
public:
	GameState();
	~GameState();

	Game *game;

	virtual void draw() = 0;
	virtual void update() = 0;
	virtual void handleInput() = 0;

};


