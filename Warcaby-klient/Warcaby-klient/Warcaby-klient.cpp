#include "pch.h"
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>

#include "Game.h"
#include "GameState.h"
#include "MenuState.h"
#include "PlayState.h"


using namespace sf;
using namespace std;

int main()
{
	Game game;
	
	game.pushState(new MenuState(&game));


	game.gameLoop();
}

