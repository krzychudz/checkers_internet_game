#include "pch.h"
#include "PlayState.h"
#include "MenuState.h"
#include "GameOverState.h"
#include "ServerErrorState.h"
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

	surrenderButton = new Button("Poddaj sie!", Vector2f(150, 50), Color::Black);
	surrenderButton->setPosition(650,350);
}


PlayState::~PlayState()
{
	delete surrenderButton;
}

void PlayState::draw()
{

	if (isTurn == side)
		game->window.draw(turnText);
	
	surrenderButton->draw(&game->window);

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
		/*
		moves = "Q" + pawnMap.getMap();
		std::thread sendThread(&PlayState::sendDataToServer, this);
		sendThread.detach();
		*/

		game->socket.disconnect();
		game->pushState(new GameOverState(game, false));
	}

	if (surrender) // Surrender
	{
		moves = "S" + pawnMap.getMap() + "X";
		std::thread sendThread(&PlayState::sendDataToServer, this);
		sendThread.detach();
	}

	if (surrenderEnemy)
	{
		/*
		moves = 'Q' + pawnMap.getMap();
		std::thread sendThread(&PlayState::sendDataToServer, this);
		sendThread.detach();
		*/

		game->socket.disconnect();
		game->pushState(new GameOverState(game, true));
	}

	if (sendData && !gameOverLose && !surrender)
	{
		std::thread sendThread(&PlayState::sendDataToServer, this);
		sendThread.detach();

		sendData = false;
	}

	if (isTurn != side && receive && !gameOverWin && !gameOverLose && !surrender && !surrenderEnemy)
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

		switch (event.type) // test
		{
		case sf::Event::KeyPressed:
			if (event.key.code == sf::Keyboard::Q)
			{
				moves = "PTEST";
				cout << "send" << endl;
				std::thread sendThread(&PlayState::sendDataToServer, this);
				sendThread.detach();
			}
			break;
		}

		if (event.type == Event::KeyReleased)
		{
			if (event.key.code == sf::Keyboard::Escape)
				pauseGame();
		}

		if (event.type == Event::MouseButtonReleased)
		{

			if (surrenderButton->isButtonPressed(&game->window))
				surrender = true;

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
							moves = "b" + pawnMap.getMap() + "X";
						
					}
					else
					{
						if (pawnMap.isLose(white))
						{
							moves = "L" + pawnMap.getMap();
							gameOverWin = true;
						}
						else
							moves = "w" + pawnMap.getMap() + "X";
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
	sf::Socket::Status status = game->socket.send(moves.c_str(), 66, t);

	if (status == sf::Socket::Done)
	{
		cout << "SEND: " << moves << endl;
		if (moves[0] == 'S')
		{
			game->socket.disconnect();
			game->pushState(new GameOverState(game, false));	
		}
		receive = true;
	}
	else if (status == sf::Socket::Disconnected)
		game->pushState(new ServerErrorState(game, "Server connect - ERROR"));
	else if (status == sf::Socket::Error)
		game->pushState(new ServerErrorState(game, "Server connect - ERROR"));
	else
		game->pushState(new ServerErrorState(game, "Server connect - ERROR"));

}

void PlayState::receiveData()
{
	sf::Socket::Status status = game->socket.receive(buf, 66, t);

	if (status == sf::Socket::Done)
	{
		cout << "BUF: " << buf << endl;
		if (buf[0] == 'b')
		{
			pawnMap.setMap(buf);
			//memset(buf, '*', 66);
			isTurn = black;
		}
		if (buf[0] == 'w')
		{
			pawnMap.setMap(buf);
			//memset(buf, '*', 66);
			isTurn = white;
		}
		if (buf[0] == 'L')
		{
			pawnMap.setMap(buf);
			//memset(buf, '*', 66);
			gameOverLose = true;
		}

		if (buf[0] == 'S')
		{
			pawnMap.setMap(buf);
			//memset(buf, '*', 66);
			surrenderEnemy = true;
		}
	}
	else if (status == sf::Socket::Disconnected && !surrender)
		game->pushState(new ServerErrorState(game, "Server connect - ERROR"));
	else if (status == sf::Socket::Error && !surrender)
		game->pushState(new ServerErrorState(game, "Server connect - ERROR"));	
	else
	{
		if(!surrender)
			game->pushState(new ServerErrorState(game, "Server connect - ERROR"));
	}

}

