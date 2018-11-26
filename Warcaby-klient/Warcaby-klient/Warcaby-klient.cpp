#include "pch.h"
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>

#include <iostream>
#include <string>

#include "Constants.h"
#include "Map.h"
#include "Pawn.h"


using namespace sf;
using namespace std;

void game(RenderWindow*);
void mainMenu(RenderWindow*);


int main()
{
	sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Warcaby");
	mainMenu(&window);
	game(&window);
}

void mainMenu()
{

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

	char buf[64];
	size_t t;


	int side = a;				//Zmienna określająca którą stroną gramy, nadawana przez server
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



		if (sendData)
		{	
			if (socket.send(moves.c_str(), 65,t) == Socket::Done)
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

		wnd->clear(Color(52, 52, 52));

		//rysowanie

		map.draw(wnd);
		pawnMap.draw(wnd);

		wnd->display();
	}
}