#include <SFML/Graphics.hpp>
#include <iostream>
#include "Constants.h"
using namespace std;
using namespace sf;
#pragma once

/*
	Klasa ob�uguj�ca plansz� gry. Czarne i bia�e pola. S�uzy g��wnie do rysowania planszy.
*/

class Map
{
public:
	Map();
	~Map();

	void draw(RenderWindow* wnd);

protected:
	Texture fieldTextureWhite; //Textura pola czarnego
	Texture fieldTextureBlack; //Textura pola bia�ego
	Sprite fieldSprite;		   //Sprite pola

};

