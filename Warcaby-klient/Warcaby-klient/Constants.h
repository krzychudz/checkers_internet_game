#pragma once
#include <SFML/Network.hpp>

const int WINDOW_WIDTH = 800;		
const int WINDOW_HEIGHT = 512;		

const int FIELD_WIDTH = 64;
const int FIELD_HEIGHT = 64;

const int PAWN_WIDTH = 64;
const int PAWN_HEIGHT = 64;

const std::string SERVER_IP = "192.168.0.11";	//Adres serwera gry
const int SERVER_PORT = 1234;					//Port pod którym nas³uchuje serwer


enum Turn	
{
	white = 3,
	black = 0
};
