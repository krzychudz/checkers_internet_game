#include "pch.h"
#include "PlayState.h"
#include "MenuState.h"

PlayState::PlayState(Game * game)
{
	this->game = game;
}


PlayState::~PlayState()
{
}

void PlayState::draw()
{
	map.draw(&game->window);
	pawnMap.draw(&game->window);
}

void PlayState::update()
{

	if (sendData)
	{
		if (socket.send(moves.c_str(), 65, t) == Socket::Done)
		{
			cout << "Socket sent a data: " << t << endl;
			cout << pawnMap.getMap() << endl;
			sendData = false;
		}

	}

	if (isTurn != side && !sendData)
	{
		if (socket.receive(buf, 65, t) != Socket::Done)
		{
			cout << buf[0] << endl;
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

		if (event.type == Event::KeyReleased)
		{
			if (event.key.code == sf::Keyboard::Escape)
				PauseGame();
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
						moves = "b" + pawnMap.getMap();
					else
						moves = "w" + pawnMap.getMap();


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

void PlayState::PauseGame()
{
	game->pushState(new MenuState(game));
}
