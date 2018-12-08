#include "pch.h"
#include "PlayState.h"
#include "MenuState.h"
#include "GameOverState.h"
#include <Thread>

PlayState::PlayState(Game * game, int side)
{
	this->game = game;
	this->side = side;

	font.loadFromFile("Fonts/arial.ttf");
	
	sideText.setOrigin(sideText.getLocalBounds().width / 2.0f, sideText.getLocalBounds().height / 2.0f);
	sideText.setCharacterSize(25);
	sideText.setFillColor(Color::White);
	sideText.setFont(font);

	if (side == black)
	{
		sideText.setString("Black");
		receive = true;
	}
	else
		sideText.setString("White");

	turnText.setOrigin(sideText.getLocalBounds().width / 2.0f, sideText.getLocalBounds().height / 2.0f);
	turnText.setCharacterSize(25);
	turnText.setFillColor(Color::White);
	turnText.setFont(font);
	turnText.setString("Your Turn");
	turnText.setPosition(650, 150);
}


PlayState::~PlayState()
{
}

void PlayState::draw()
{

	if (isTurn == side)
		game->window.draw(turnText);
	
	sideText.setPosition(650, 100);
	game->window.draw(sideText);

	map.draw(&game->window);
	pawnMap.draw(&game->window);
}

void PlayState::update()
{
	if (gameOverWin && !sendData)	//Wygrana gracza
	{
		game->socket.disconnect();
		game->pushState(new GameOverState(game,true));
	}

	if (gameOverLose && !sendData) //Przegrana gracza
	{
		game->socket.disconnect();
		game->pushState(new GameOverState(game, false));
	}

	if (sendData)
	{
		std::thread sendThread(&PlayState::sendDataToServer, this);
		sendThread.detach();

		sendData = false;
	}

	if (isTurn != side && receive)
	{
		std::thread receiveThread(&PlayState::receiveData, this);
		receiveThread.detach();

		receive = false;
	}
}

void PlayState::handleInput()
{
	sf::Event event;
	
	while (game->window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
			game->window.close();

		if (event.type == Event::KeyReleased)
		{
			if (event.key.code == sf::Keyboard::Escape)
				pauseGame();
		}

		if (event.type == Event::MouseButtonReleased)
		{
			if (isTurn == side && pawnMap.getField(Mouse::getPosition(game->window).y / PAWN_WIDTH,
				Mouse::getPosition(game->window).x / PAWN_HEIGHT) == side && !selected)
			{
				sourceX = (Mouse::getPosition(game->window).y / PAWN_HEIGHT);
				sourceY = (Mouse::getPosition(game->window).x / PAWN_WIDTH);
				selected = true;

			}

			else if (selected && isTurn == side)
			{
				destX = (Mouse::getPosition(game->window).y / PAWN_HEIGHT);
				destY = (Mouse::getPosition(game->window).x / PAWN_WIDTH);



				if (pawnMap.update(sourceX, sourceY, destX, destY))
				{

					if (side == white)
					{
						if (pawnMap.isLose(black))
						{
							moves = "L" + pawnMap.getMap();
							gameOverWin = true;
						}
						else
							moves = "b" + pawnMap.getMap();
					}
					else
					{
						if (pawnMap.isLose(white))
						{
							moves = "L" + pawnMap.getMap();
							gameOverWin = true;
						}
						else
							moves = "w" + pawnMap.getMap();
					}


					sendData = true;

					if (side == white)
						isTurn = black;
					else
						isTurn = white;
				}
				else
				{
					cout << "Zly ruch" << endl;

				}

				selected = false;

			}
		}
	}
	
	
}

void PlayState::pauseGame()
{
	game->pushState(new MenuState(game));
}

void PlayState::sendDataToServer()
{
	sf::Socket::Status status = game->socket.send(moves.c_str(), 65, t);

	if (status == sf::Socket::Done)
	{
		receive = true;
	}

}

void PlayState::receiveData()
{
	
	sf::Socket::Status status = game->socket.receive(buf, 65, t);

	if (status == sf::Socket::Done)
	{
		if (buf[0] == 'b')
		{
			pawnMap.setMap(buf);
			memset(buf, 65, 5);
			isTurn = black;
		}
		if (buf[0] == 'w')
		{
			pawnMap.setMap(buf);
			memset(buf, 65, 5);
			isTurn = white;
		}
		if (buf[0] == 'L')
		{
			pawnMap.setMap(buf);
			memset(buf, 65, 5);
			gameOverLose = true;
		}
	}
	

}

