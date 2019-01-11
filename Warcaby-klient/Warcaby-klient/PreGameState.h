#pragma once
#include "GameState.h"
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include "Constants.h"
#include <thread>
#include <vector>

using namespace sf;

/*
	Klasa obs�uguj�ca lobby gry. Tutaj gracz oczekuje na inne gracza. Klasa ta nawi�zuje pierwsze po��czenie z serwerem oraz odbiera
	informacj� o tym czy gracz gra pionami bia�ymi czy czarnymi.
*/


class PreGameState :
	public GameState
{
public:
	PreGameState(Game * game);

	virtual void draw();
	virtual void update();
	virtual void handleInput();

private:
	Text info;
	Font font;
	
	Socket::Status socketStatus;
	
	int side;

	bool connected;
	bool runThreadConnect;
	bool runThreadReceive;
	bool isDataReceive;


	void connect();
	void receiveData();

	char dataBuf[64];
	size_t dataSize;

	std::string IP_ADDRESS;



};

