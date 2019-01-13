#include "pch.h"
#include "Game.h"
#include "GameState.h"

Game::Game()
{
	window.create(VideoMode(800, 512), "Checkers");

	window.setFramerateLimit(60);
}


Game::~Game()
{
	while (!states.empty())
		popState();
}

/*
	Metoda dodaj�ca nowy stan(scen�) do narysowania na ekranie
	Parametry:
		GameState * state - wska�nik na nowy stan
*/

void Game::pushState(GameState * state)
{
	states.push_back(state);
}

/*
	Metoda usuwaj�ca stan
*/

void Game::popState()
{
	states.back();
	delete states.back();
	states.pop_back();
}

/*
	Metoda zwracaj�ca aktualnie obs�ugiwany stan
	Return:
		GameState * - wska�nik na aktualny stan
*/

GameState * Game::CurrentState()
{
	if (states.empty())
		return nullptr;
	else
		return states.back();
}

/*
	Metoda obs�uguj�ca g��wn� p�tle aplikacji

*/

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
