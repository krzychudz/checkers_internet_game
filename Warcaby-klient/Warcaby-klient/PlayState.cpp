#include "pch.h"
#include "PlayState.h"
#include "MenuState.h"
#include "GameOverState.h"
#include "ServerErrorState.h"
#include <sstream>
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

	clockText.setOrigin(clockText.getLocalBounds().width / 2.0f, clockText.getLocalBounds().height / 2.0f);
	clockText.setCharacterSize(25);
	clockText.setFillColor(Color::White);
	clockText.setFont(font);
	clockText.setString("1:00");
	clockText.setPosition(650, 400);
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

	game->window.draw(clockText);
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

	if (surrender) // Surrender
	{
		moves = "S" + pawnMap.getMap() + "X";
		std::thread sendThread(&PlayState::sendDataToServer, this);
		sendThread.detach();
	}

	if (surrenderEnemy)
	{
		game->socket.disconnect();
		game->pushState(new GameOverState(game, true));
	}

	if (enemyConnError)
	{
		game->socket.disconnect();
		game->pushState(new GameOverState(game, true));
	}

	if (sendData && !gameOverLose && !surrender && !enemyConnError)
	{
		std::thread sendThread(&PlayState::sendDataToServer, this);
		sendThread.detach();

		sendData = false;
	}

	if (isTurn != side && receive && !gameOverWin && !gameOverLose && !surrender && !surrenderEnemy && !enemyConnError)
	{
		std::thread receiveThread(&PlayState::receiveData, this);
		receiveThread.detach();

		receive = false;
	}

	if (isTurn == side) // Obs³uga Timera 
	{
		stringstream ss;
		time = clock.getElapsedTime();

		int tmp = timer - time.asSeconds();
		int min = tmp / 60;
		int sec = tmp - (min*60);

		if(sec < 10)
			ss << min << ":0" << sec;
		else
			ss << min << ":" << sec;

		string str = ss.str();
		clockText.setString(str);

		if (tmp == 0)
		{
			clockText.setString("1:00");

			skipMoves++;
			if (skipMoves == 3)
			{
				moves = "T" + pawnMap.getMap() + "X";
				std::thread sendThread(&PlayState::sendDataToServer, this);
				sendThread.detach();
			}
			else
			{
				if (side == white)
				{
					moves = "b" + pawnMap.getMap() + "X";
					isTurn = black;
				}
				else
				{
					moves = "w" + pawnMap.getMap() + "X";
					isTurn = white;
				}

				sendData = true;
			}
		}
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

			if (surrenderButton->isButtonPressed(&game->window) && isTurn == side)
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

					if (!pawnMap.checkNextMove())
					{
						if (side == white)
						{
							if (pawnMap.isLose(black))
							{
								moves = "L" + pawnMap.getMap() + "X";
								gameOverWin = true;
							}
							else
								moves = "b" + pawnMap.getMap() + "X";

						}
						else
						{
							if (pawnMap.isLose(white))
							{
								moves = "L" + pawnMap.getMap() + "X";
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


						clockText.setString("1:00");
					}
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
		if (moves[0] == 'S')
		{
			cout << "SEND: " << moves << endl;
			game->socket.disconnect();
			game->pushState(new GameOverState(game, false));	
		}
		else if (moves[0] == 'T')
		{
			game->socket.disconnect();
			game->pushState(new GameOverState(game, false));
		}

		receive = true;
	}
	else if (status == sf::Socket::Disconnected)
		game->pushState(new ServerErrorState(game, "Server connect - ERROR1"));
	else if (status == sf::Socket::Error)
		game->pushState(new ServerErrorState(game, "Server connect - ERROR1"));
	else
		game->pushState(new ServerErrorState(game, "Server connect - ERROR1"));

}

void PlayState::receiveData()
{
	sf::Socket::Status status = game->socket.receive(buf, 66, t);

	if (status == sf::Socket::Done)
	{
		cout << "BUF: " << buf << endl;
		if (buf[0] == 'b')
		{
			clock.restart();
			pawnMap.setMap(buf);
			memset(buf, '*', 66);
			isTurn = black;
		}
		else if (buf[0] == 'w')
		{
			clock.restart();
			pawnMap.setMap(buf);
			memset(buf, '*', 66);
			isTurn = white;
		}
		else if (buf[0] == 'L')
		{
			pawnMap.setMap(buf);
			memset(buf, '*', 66);
			gameOverLose = true;
		}

		else if (buf[0] == 'S')
		{
			//pawnMap.setMap(buf);
			memset(buf, '*', 66);
			surrenderEnemy = true;
		}
		else if (buf[0] == 'E')
			enemyConnError = true;

		else if (buf[0] == 'T')
			gameOverWin = true;

	}
	else if (status == sf::Socket::Disconnected && !surrender)
		game->pushState(new ServerErrorState(game, "Server connect - ERROR2"));
	else if (status == sf::Socket::Error && !surrender)
		game->pushState(new ServerErrorState(game, "Server connect - ERROR2"));	
	else
	{
		if(!surrender)
			game->pushState(new ServerErrorState(game, "Server connect - ERROR2"));
	}

}

