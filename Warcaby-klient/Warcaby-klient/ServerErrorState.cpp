#include "pch.h"
#include "ServerErrorState.h"
#include "MenuState.h"


ServerErrorState::ServerErrorState(Game* game, std::string text)
{
	this->game = game;

	font.loadFromFile("Fonts/arial.ttf");
	errorInfo.setOrigin(errorInfo.getLocalBounds().width / 2.0f, errorInfo.getLocalBounds().height / 2.0f);
	returnToMainMenu.setOrigin(returnToMainMenu.getLocalBounds().width / 2.0f, returnToMainMenu.getLocalBounds().height / 2.0f);

	errorInfo.setCharacterSize(32);
	errorInfo.setFillColor(Color::White);
	errorInfo.setFont(font);
	errorInfo.setString(text);
	errorInfo.setFillColor(Color::White);

	returnToMainMenu.setString("Menu");
	returnToMainMenu.setCharacterSize(25);
	returnToMainMenu.setFont(font);

	errorInfo.setPosition(WINDOW_WIDTH / 2.0f - errorInfo.getLocalBounds().width / 2.0f, 50);
	returnToMainMenu.setPosition(WINDOW_WIDTH / 2.0f - returnToMainMenu.getLocalBounds().width / 2.0f, 200);
}


ServerErrorState::~ServerErrorState()
{

}

void ServerErrorState::draw()
{
	game->window.draw(errorInfo);
	game->window.draw(returnToMainMenu);
}

void ServerErrorState::update()
{
}

void ServerErrorState::handleInput()
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

			else if (event.key.code == sf::Keyboard::Return)
			{
				game->pushState(new MenuState(game));
			}
			break;
		}
	}
}
