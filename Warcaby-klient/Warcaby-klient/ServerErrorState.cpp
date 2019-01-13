#include "pch.h"
#include "ServerErrorState.h"
#include "MenuState.h"


ServerErrorState::ServerErrorState(Game* game, std::string text)
{
	this->game = game;

	font.loadFromFile(FONT_PATH);
	errorInfo.setOrigin(errorInfo.getLocalBounds().width / 2.0f, errorInfo.getLocalBounds().height / 2.0f);
	returnToMainMenu.setOrigin(returnToMainMenu.getLocalBounds().width / 2.0f, returnToMainMenu.getLocalBounds().height / 2.0f);

	errorInfo.setCharacterSize(25);
	errorInfo.setFillColor(Color::White);
	errorInfo.setFont(font);
	errorInfo.setString(text);
	errorInfo.setFillColor(Color::White);

	returnToMainMenu.setString("Press enter to menu");
	returnToMainMenu.setCharacterSize(16);
	returnToMainMenu.setFont(font);

	errorInfo.setPosition(WINDOW_WIDTH / 2.0f - errorInfo.getLocalBounds().width / 2.0f, 50);
	returnToMainMenu.setPosition(WINDOW_WIDTH / 2.0f - returnToMainMenu.getLocalBounds().width / 2.0f, 200);
}


ServerErrorState::~ServerErrorState()
{

}

/*
	Metoda rysuj�ca ekran informuj�cy o b��dzie
*/

void ServerErrorState::draw()
{
	game->window.draw(errorInfo);
	game->window.draw(returnToMainMenu);
}

/*
	Metoda aktualizuj�ca ekran informuj�cy o b��dzie
*/

void ServerErrorState::update()
{
}

/*
	Metoda obs�uguj�ca wej/wyj ekranu informuj�cego o b��dzie
*/

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
