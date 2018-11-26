#include "pch.h"
#include "Game.h"
#include "Constants.h"
#include "GameState.h"

Game::Game()
{
	window.create(VideoMode(800, 600), "Warcaby");

	window.setFramerateLimit(60);
}


Game::~Game()
{
	while (!states.empty())
		popState();
}

void Game::pushState(GameState * state)
{
	states.push_back(state);
}

void Game::popState()
{
	states.back();
	delete states.back();
	states.pop_back();
}

GameState * Game::CurrentState()
{
	if (states.empty())
		return nullptr;
	else
		return states.back();
}

void Game::gameLoop()
{

	while (window.isOpen())
	{

		if (CurrentState() == nullptr)
			continue;

		CurrentState()->handleInput();
		CurrentState()->update();

		window.clear(Color(52, 52, 52));

		CurrentState()->draw();

		window.display();

	}


}
