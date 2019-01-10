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

	font.loadFromFile(FONT_PATH);
	
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
	

	sideText.setPosition(600, 50);

	turnText.setOrigin(sideText.getLocalBounds().width / 2.0f, sideText.getLocalBounds().height / 2.0f);
	turnText.setCharacterSize(25);
	turnText.setFillColor(Color::White);
	turnText.setFont(font);
	turnText.setString("Your Turn");
	turnText.setPosition(610, 150);

	surrenderButton = new Button("Surrender!", Vector2f(150, 50), Color::Black);
	surrenderButton->setPosition(660,350);

	clockText.setOrigin(clockText.getLocalBounds().width / 2.0f, clockText.getLocalBounds().height / 2.0f);
	clockText.setCharacterSize(25);
	clockText.setFillColor(Color::White);
	clockText.setFont(font);
	clockText.setString("1:00");
	clockText.setPosition(620, 220);

	selectedRectangle.setSize(Vector2f(64, 64));
	selectedRectangle.setOutlineColor(Color::Red);
	selectedRectangle.setOutlineThickness(3);
	selectedRectangle.setFillColor(Color(255, 255, 255, 0));
}


PlayState::~PlayState()
{
	delete surrenderButton;
}

void PlayState::draw()
{

	if (isTurn == side)
		game->window.draw(turnText);
	
	if (surrenderButton->isButtonPressed(&game->window) && isTurn == side)
		surrenderButton->setColor(Color::Red);
	else
		surrenderButton->setColor(Color::White);


	surrenderButton->draw(&game->window);

	game->window.draw(sideText);

	map.draw(&game->window);
	pawnMap.draw(&game->window);

	game->window.draw(clockText);

	if (drawRectangle)
		game->window.draw(selectedRectangle);
	
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

		if (sec < 10)
		{
			ss << min << ":0" << sec;
			clockText.setFillColor(Color::Red);
		}
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

				clockText.setFillColor(Color::White);
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

		switch (event.type) 
		{
		case sf::Event::KeyPressed:
			if (event.key.code == sf::Keyboard::Q)
			{
			
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

			if (pawnMap.getField(Mouse::getPosition(game->window).y / PAWN_WIDTH,
				Mouse::getPosition(game->window).x / PAWN_HEIGHT) == 1 && side == black &&
				Mouse::getPosition(game->window).y / PAWN_WIDTH < 8 && Mouse::getPosition(game->window).x / PAWN_HEIGHT < 8)
					king = true;
			
			
			if (pawnMap.getField(Mouse::getPosition(game->window).y / PAWN_WIDTH,
				Mouse::getPosition(game->window).x / PAWN_HEIGHT) == 4 && side == white &&
				Mouse::getPosition(game->window).y / PAWN_WIDTH < 8 && Mouse::getPosition(game->window).x / PAWN_HEIGHT < 8)
					king = true;
			
			

			if (isTurn == side && (pawnMap.getField(Mouse::getPosition(game->window).y / PAWN_WIDTH,
				Mouse::getPosition(game->window).x / PAWN_HEIGHT) == side || king) && !selected &&
				Mouse::getPosition(game->window).y / PAWN_WIDTH < 8 && Mouse::getPosition(game->window).x / PAWN_HEIGHT < 8)
			{
					

				sourceX = (Mouse::getPosition(game->window).y / PAWN_HEIGHT);
				sourceY = (Mouse::getPosition(game->window).x / PAWN_WIDTH);
				selected = true;
				king = false;

				selectedRectangle.setPosition(sourceY * 64, sourceX * 64);
				drawRectangle = true;

			}

			else if (selected && isTurn == side)
			{
				destX = (Mouse::getPosition(game->window).y / PAWN_HEIGHT);
				destY = (Mouse::getPosition(game->window).x / PAWN_WIDTH);

				drawRectangle = false;

				if (pawnMap.update(sourceX, sourceY, destX, destY, side))
				{

					if (!pawnMap.getNextMove())
					{
						
						pawnMap.updateKingPawn(side);

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
						clockText.setFillColor(Color::White);
					}
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
		game->pushState(new ServerErrorState(game, "Server connection error"));
	else if (status == sf::Socket::Error)
		game->pushState(new ServerErrorState(game, "Server connection error"));
	else
		game->pushState(new ServerErrorState(game, "Server connection error"));

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
			memset(buf, '*', 66);
			surrenderEnemy = true;
		}
		else if (buf[0] == 'E')
			enemyConnError = true;

		else if (buf[0] == 'T')
			gameOverWin = true;

	}
	else if (status == sf::Socket::Disconnected && !surrender)
		game->pushState(new ServerErrorState(game, "Server connection error"));
	else if (status == sf::Socket::Error && !surrender)
		game->pushState(new ServerErrorState(game, "Server connection error"));	
	else
	{
		if(!surrender)
			game->pushState(new ServerErrorState(game, "Server connection error"));
	}

}

