#include "pch.h"
#include "GameOverState.h"
#include "MenuState.h"


GameOverState::GameOverState(Game* game, bool resultInfo, std::string reason)
{
	this->game = game;

	font.loadFromFile(FONT_PATH);

	resultText.setCharacterSize(25);
	resultText.setFillColor(Color::White);
	resultText.setFont(font);


	if (resultInfo)
		resultText.setString("You win");
	else
		resultText.setString("You lose");


	reasonText.setString(reason);
	reasonText.setCharacterSize(16);
	resultText.setFillColor(Color::White);
	reasonText.setFont(font);


	returnToMainMenu.setString("Press enter to menu");
	returnToMainMenu.setCharacterSize(16);
	resultText.setFillColor(Color::White);
	returnToMainMenu.setFont(font);
	

	reasonText.setOrigin(reasonText.getLocalBounds().width / 2.0f, reasonText.getLocalBounds().height / 2.0f);
	resultText.setOrigin(resultText.getLocalBounds().width / 2.0f, resultText.getLocalBounds().height / 2.0f);
	returnToMainMenu.setOrigin(returnToMainMenu.getLocalBounds().width / 2.0f, returnToMainMenu.getLocalBounds().height / 2.0f);

	resultText.setPosition(WINDOW_WIDTH / 2.0f, 50);
	returnToMainMenu.setPosition(WINDOW_WIDTH / 2.0f, 230);
	reasonText.setPosition(WINDOW_WIDTH / 2.0f, 150);

}


GameOverState::~GameOverState()
{
}

/*
	Metoda rysuj¹ca scenê

*/

void GameOverState::draw()
{
	game->window.draw(resultText);
	game->window.draw(returnToMainMenu);
	game->window.draw(reasonText);
}

/*
	Metoda aktualizuj¹ca scenê
*/

void GameOverState::update()
{
}

/*
	Metoda ab³uguj¹ca wjeœcie/wyjœcie 
*/

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
