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
	Metoda dodaj¹ca nowy stan(scenê) do narysowania na ekranie
	Parametry:
		GameState * state - wskaŸnik na nowy stan
*/

void Game::pushState(GameState * state)
{
	states.push_back(state);
}

/*
	Metoda usuwaj¹ca stan
*/

void Game::popState()
{
	states.back();
	delete states.back();
	states.pop_back();
}

/*
	Metoda zwracaj¹ca aktualnie obs³ugiwany stan
	Return:
		GameState * - wskaŸnik na aktualny stan
*/

GameState * Game::CurrentState()
{
	if (states.empty())
		return nullptr;
	else
		return states.back();
}

/*
	Metoda obs³uguj¹ca g³ównê pêtle aplikacji

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
