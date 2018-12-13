#include "pch.h"
#include "PreGameState.h"
#include "PlayState.h"
#include "ServerErrorState.h"
#include "MenuState.h"



PreGameState::PreGameState(Game * game)
{
	this->game = game;

	font.loadFromFile("Fonts/arial.ttf");
	info.setOrigin(info.getLocalBounds().width / 2.0f, info.getLocalBounds().height / 2.0f);

	info.setString("Oczekiwanie na przeciwnika...");
	info.setCharacterSize(32);
	info.setFillColor(Color::White);
	info.setFont(font);



	connected = false;
	runThreadConnect = true;
	runThreadReceive = false;
	isDataReceive = false;

}

void PreGameState::draw()
{

	info.setPosition(WINDOW_WIDTH / 2.0f - info.getLocalBounds().width / 2.0f, 50);
	game->window.draw(info);

}

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
			cout << "recived" << endl;
			cout << dataBuf[0] << endl;

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
			else
				cout << "Error" << endl;

			isDataReceive = false;
		}

	}

}

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

void PreGameState::connect()
{	
	sf::Socket::Status status = game->socket.connect(SERVER_IP, SERVER_PORT);

	if (status != sf::Socket::Done)
	{
		cout << "ERROR: Tcp Socket - connect" << endl;
		game->pushState(new ServerErrorState(game, "Server connect - ERROR"));

	}
	else
	{
		connected = true;
		runThreadReceive = true;
		cout << "Connect to server - success" << endl;
	}
	
}

void PreGameState::receiveData()
{

	sf::Socket::Status status = game->socket.receive(dataBuf, 2, dataSize);

	if (status != sf::Socket::Done)
		game->pushState(new ServerErrorState(game, "Server connect - ERROR"));
	else
		isDataReceive = true;
}

