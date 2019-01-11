#include "pch.h"
#include "PreGameState.h"
#include "PlayState.h"
#include "ServerErrorState.h"
#include "MenuState.h"
#include <fstream>



PreGameState::PreGameState(Game * game)
{
	this->game = game;

	font.loadFromFile(FONT_PATH);
	info.setOrigin(info.getLocalBounds().width / 2.0f, info.getLocalBounds().height / 2.0f);

	info.setString("Waiting for players...");
	info.setCharacterSize(32);
	info.setFillColor(Color::White);
	info.setFont(font);



	connected = false;
	runThreadConnect = true;
	runThreadReceive = false;
	isDataReceive = false;

	std::ifstream infile("ip_address.txt");
	infile >> IP_ADDRESS;
	infile.close();

}

/*
	Metoda rysuj¹ca lobby
*/

void PreGameState::draw()
{

	info.setPosition(WINDOW_WIDTH / 2.0f - info.getLocalBounds().width / 2.0f, 50);
	game->window.draw(info);

}

/*
	Metoda aktualizuj¹ca stan lobby
*/

void PreGameState::update()
{
	if (runThreadConnect)
	{
		std::thread connectThread(&PreGameState::connect, this);
		connectThread.detach();
		runThreadConnect = false;
	}

	if (connected)
	{
		if (runThreadReceive)
		{
			std::thread receiveThread(&PreGameState::receiveData, this);
			receiveThread.detach();
			runThreadReceive = false;
		}

		if (isDataReceive)
		{
			if (dataBuf[0] == 'w')
			{
				side = white;
				game->pushState(new PlayState(game, side));
			}
			else if (dataBuf[0] == 'b')
			{
				side = black;
				game->pushState(new PlayState(game, side));
			}
			else if (dataBuf[0] == 'E')
			{
				game->socket.disconnect();
				game->pushState(new ServerErrorState(game, "Your opponent has lefy the game"));
			}
			else
				cout << "Error" << endl;

			isDataReceive = false;
		}

	}

}

/*
	Metoda obs³uguj¹ca wje/wyj lobby
*/

void PreGameState::handleInput()
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

			if (event.key.code == sf::Keyboard::Return)
			{	
			}
			break;
		}
	}

}

/*
	Metoda pozwalaj¹ca na po³¹czenie siê z serwerem
*/

void PreGameState::connect()
{	
	sf::Socket::Status status = game->socket.connect(IP_ADDRESS, SERVER_PORT);

	if (status != sf::Socket::Done)
		game->pushState(new ServerErrorState(game, "ERROR - server is not available"));
	
	else
	{
		connected = true;
		runThreadReceive = true;
	}
	
}

/*
	Metoda pozwalaj¹ca odebranie danych od serwera
*/

void PreGameState::receiveData()
{

	sf::Socket::Status status = game->socket.receive(dataBuf, 2, dataSize);

	if (status != sf::Socket::Done)
		game->pushState(new ServerErrorState(game, "Server connection error"));
	else
		isDataReceive = true;
}

