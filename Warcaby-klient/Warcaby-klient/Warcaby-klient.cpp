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
	TcpSocket socket;
	sf::Socket::Status status = socket.connect(SERVER_IP, SERVER_PORT);
	if (status != sf::Socket::Done)
	{
		cout << "ERROR: Tcp Socket - connect" << endl;
	}


	int side = white;			//Zmienna określająca którą stroną gramy, nadawana przez server
	Turn isTurn = white;		//Zmienna określająca czyja jest tura, ustawiana przez server

	Map map;					//plansza
	Pawn pawnMap;				//piony

	while (wnd->isOpen())
	{
		sf::Event event;
		while (wnd->pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				wnd->close();

		}

		if (isTurn == side)		//Obsługa podnoszenia i przenoszenie pionów
		{
			

			if (side == white)
				isTurn = black;
			else
				isTurn = white;
		}




		wnd->clear(Color(52, 52, 52));

		//rysowanie
		map.draw(wnd);
		pawnMap.draw(wnd);

		wnd->display();
	}
}