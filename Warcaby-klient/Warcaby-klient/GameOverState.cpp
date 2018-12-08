#include "pch.h"
#include "GameOverState.h"
#include "MenuState.h"


GameOverState::GameOverState(Game* game, bool resultInfo)
{
	this->game = game;

	font.loadFromFile("Fonts/arial.ttf");
	resultText.setOrigin(resultText.getLocalBounds().width / 2.0f, resultText.getLocalBounds().height / 2.0f);
	returnToMainMenu.setOrigin(returnToMainMenu.getLocalBounds().width / 2.0f, returnToMainMenu.getLocalBounds().height / 2.0f);

	resultText.setCharacterSize(32);
	resultText.setFillColor(Color::White);
	resultText.setFont(font);

	if (resultInfo)
		resultText.setString("Wygrana");
	else
		resultText.setString("Przegrana");


	returnToMainMenu.setString("Menu");
	returnToMainMenu.setCharacterSize(25);
	resultText.setFillColor(Color::White);
	returnToMainMenu.setFont(font);

	resultText.setPosition(WINDOW_WIDTH / 2.0f - resultText.getLocalBounds().width / 2.0f, 50);
	returnToMainMenu.setPosition(WINDOW_WIDTH / 2.0f - returnToMainMenu.getLocalBounds().width / 2.0f, 200);

}


GameOverState::~GameOverState()
{
}

void GameOverState::draw()
{
	game->window.draw(resultText);
	game->window.draw(returnToMainMenu);
}

void GameOverState::update()
{
}

void GameOverState::handleInput()
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
