#include "pch.h"
#include "MenuState.h"
#include "GameState.h"
#include "PreGameState.h"


MenuState::MenuState(Game * game)
{
	this->game = game;

	font.loadFromFile("Fonts/arial.ttf");

	title.setOrigin(title.getLocalBounds().width / 2.0f, title.getLocalBounds().height / 2.0f);
	newGame.setOrigin(newGame.getLocalBounds().width / 2.0f, newGame.getLocalBounds().height / 2.0f);
	exit.setOrigin(exit.getLocalBounds().width / 2.0f, exit.getLocalBounds().height / 2.0f);

	title.setString("Warcaby");
	title.setCharacterSize(32);
	title.setFillColor(Color::White);
	title.setFont(font);

	newGame.setString("Nowa gra");
	newGame.setCharacterSize(32);
	newGame.setFillColor(Color::White);
	newGame.setFont(font);

	exit.setString("Koniec");
	exit.setCharacterSize(32);
	exit.setFillColor(Color::White);
	exit.setFont(font);

	title.setPosition(WINDOW_WIDTH / 2.0f - title.getLocalBounds().width / 2.0f, 50);
	newGame.setPosition(WINDOW_WIDTH / 2.0f - newGame.getLocalBounds().width / 2.0f, 200);
	exit.setPosition(WINDOW_WIDTH / 2.0f - exit.getLocalBounds().width / 2.0f, 300);


}

void MenuState::draw()
{

	game->window.draw(title);
	game->window.draw(newGame);
	game->window.draw(exit);

}

void MenuState::update()
{
	if (menuPointer == 0)
	{
		newGame.setFillColor(Color::Red);
		exit.setFillColor(Color::White);
	}
	else
	{
		newGame.setFillColor(Color::White);
		exit.setFillColor(Color::Red);
	}



}

void MenuState::handleInput()
{

	sf::Event event;

	while (game->window.pollEvent(event))
	{
		switch (event.type)
		{
			case sf::Event::Closed:
				game->window.close();
				break;

			case sf::Event::KeyPressed:
				if (event.key.code == sf::Keyboard::Escape)
					game->window.close();
				else if (event.key.code == sf::Keyboard::Up) {
					menuPointer++;
					if (menuPointer == 2)
						menuPointer = 0;
				}
				else if (event.key.code == sf::Keyboard::Down) {
					menuPointer--;
					if (menuPointer == -1)
						menuPointer = 1;
				}
				else if (event.key.code == sf::Keyboard::Return) {

					if (menuPointer == 0)
						loadgame();
					else
						game->window.close();

				}
			
				break;
		}
	}

}

void MenuState::loadgame()
{
	game->pushState(new PreGameState(game));
}


