#include "pch.h"
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include "Map.h"
#include "Constants.h"
#include "Pawn.h"

using namespace sf;

void game(RenderWindow*);

int main()
{
	sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Szachy");
	game(&window);
}

void game(RenderWindow*wnd)
{
	//test
	TcpSocket socket;
	socket.setBlocking(false);

	sf::Socket::Status status = socket.connect(SERVER_IP, SERVER_PORT);
	//

	int side = white;			//Zmienna określająca którą stroną gramy, nadawana przez server
	Turn isTurn = white;		//Zmienna określająca czyja jest tura, ustawiana przez server

	Map map;					//plansza
	Pawn pawnMap;				//piony

	bool leftMousePressed = false;

	Sprite whitePawnSprite = pawnMap.getWhitePawnSprite();
	Sprite blackPawnSprite = pawnMap.getBlackPawnSprite();

	int pawnNumber; // Numer aktualnie przenoszonego piona

	int sourceX = 0;
	int sourceY = 0;

	bool selected = false;

	bool tmpp = true;

	while (wnd->isOpen())
	{
		sf::Event event;
		while (wnd->pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				wnd->close();

			if (event.type == Event::KeyReleased)
			{
				if (event.key.code == sf::Keyboard::I)
				{
					if (socket.getRemoteAddress() == IpAddress::None && socket.getRemotePort() == 0)
					{
						std::cout << "Connecting..." << std::endl;
					}
					else
					{
						std::cout << "Connected" << std::endl;
					}
				}
			}

			if (event.type == Event::MouseButtonReleased)
			{
				if (isTurn == side && pawnMap.getField(Mouse::getPosition(*wnd).y / PAWN_WIDTH,
					Mouse::getPosition(*wnd).x / PAWN_HEIGHT) == side && !selected)
				{
					sourceX = (Mouse::getPosition(*wnd).y / PAWN_HEIGHT);
					sourceY = (Mouse::getPosition(*wnd).x / PAWN_WIDTH);
					selected = true;

				}

				else if (selected)
				{
					int destX = (Mouse::getPosition(*wnd).y / PAWN_HEIGHT);
					int destY = (Mouse::getPosition(*wnd).x / PAWN_WIDTH);




					if (pawnMap.update(sourceX, sourceY, destX, destY) == true)
					{
						std::cout << "Udalo sie pomyslnie wykonac ruch" << std::endl;
					}
					else
					{
						std::cout << "Nie udalo sie wykonac ruchu" << std::endl;
					}

					selected = false;

					/*
				if (side == white)
					isTurn = black;
				else
					isTurn = white;
					*/

				}
			}

			/*
			std::cout << "Mouse PosX: " << Mouse::getPosition(*wnd).x / 64 << " Mouse PosY: " << Mouse::getPosition(*wnd).y / 64 << std::endl;
			if(pawnMap.getField(Mouse::getPosition(*wnd).y / 64, Mouse::getPosition(*wnd).x / 64) == 0)
				std::cout << "Czarny Pion" << std::endl;
			if (pawnMap.getField(Mouse::getPosition(*wnd).y / 64, Mouse::getPosition(*wnd).x / 64) == 1)
				std::cout << "Bialy Pion" << std::endl;
			if (pawnMap.getField(Mouse::getPosition(*wnd).y / 64, Mouse::getPosition(*wnd).x / 64) == 2)
				std::cout << "Puste Pole" << std::endl;
				*/

		}

		wnd->clear(Color(52, 52, 52));

		//rysowanie

		map.draw(wnd);
		pawnMap.draw(wnd);

		wnd->display();
	}
}