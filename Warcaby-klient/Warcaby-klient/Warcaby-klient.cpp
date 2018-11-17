#include "pch.h"
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include "Map.h"
#include "Constants.h"
#include "Pawn.h"
#include <string>

using namespace sf;
using namespace std;

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
	socket.setBlocking(true);
	int a;
	cin >> a;

	sf::Socket::Status status = socket.connect(SERVER_IP, SERVER_PORT);
	socket.setBlocking(false);

	char buf[5];
	size_t t;


	int side = a;			//Zmienna określająca którą stroną gramy, nadawana przez server
	Turn isTurn = white;		//Zmienna określająca czyja jest tura, ustawiana przez server

	cout << "Side: " << side << endl;

	Map map;					//plansza
	Pawn pawnMap;				//piony

	bool leftMousePressed = false;

	Sprite whitePawnSprite = pawnMap.getWhitePawnSprite();
	Sprite blackPawnSprite = pawnMap.getBlackPawnSprite();

	int pawnNumber; // Numer aktualnie przenoszonego piona

	std::string moves;

	int sourceX = 0;
	int sourceY = 0;

	int destX = 0;
	int destY = 0;

	bool selected = false;
	bool sendData = false;

	while (wnd->isOpen())
	{
		sf::Event event;
		while (wnd->pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				wnd->close();

			if (event.type == Event::KeyReleased)
			{

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

				else if (selected && isTurn == side)
				{
					destX = (Mouse::getPosition(*wnd).y / PAWN_HEIGHT);
					destY = (Mouse::getPosition(*wnd).x / PAWN_WIDTH);

					if (pawnMap.update(sourceX, sourceY, destX, destY))
					{
						if(side == white)
							moves = "0" + to_string(sourceX) + to_string(sourceY) + to_string(destX) + to_string(destY) + "\n";
						else
							moves = "3" + to_string(sourceX) + to_string(sourceY) + to_string(destX) + to_string(destY) + "\n";


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



		if (sendData)
		{	
			if (socket.send(moves.c_str(), 5) == Socket::Done)
			{
				cout << "Socket sent a data" << endl;
				sendData = false;
			}

		}

		if (isTurn != side && !sendData)
		{
			if (socket.receive(buf, 5, t) != Socket::Done)
			{
				cout << buf[0] << endl;
				if (buf[0] == '0')
				{
					int sx = buf[1] - '0';
					int sy = buf[2] - '0';
					int dx = buf[3] - '0';
					int dy = buf[4] - '0';
					pawnMap.update(sx, sy, dx, dy);
					memset(buf, 9, 5);
					isTurn = black;
				}
				if (buf[0] == '3')
				{
					int sx = buf[1] - '0';
					int sy = buf[2] - '0';
					int dx = buf[3] - '0';
					int dy = buf[4] - '0';
					pawnMap.update(sx, sy, dx, dy);
					memset(buf, 9, 5);
					isTurn = white;
				}
			}
		}

		wnd->clear(Color(52, 52, 52));

		//rysowanie

		map.draw(wnd);
		pawnMap.draw(wnd);

		wnd->display();
	}
}