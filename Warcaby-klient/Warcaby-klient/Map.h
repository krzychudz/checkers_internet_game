#include <SFML/Graphics.hpp>
#include <iostream>
#include "Constants.h"
using namespace std;
using namespace sf;
#pragma once

/*
	Klasa ob³uguj¹ca planszê gry. Czarne i bia³e pola. S³uzy g³ównie do rysowania planszy.
*/

class Map
{
public:
	Map();
	~Map();

	void draw(RenderWindow* wnd);

protected:
	Texture fieldTextureWhite; //Textura pola czarnego
	Texture fieldTextureBlack; //Textura pola bia³ego
	Sprite fieldSprite;		   //Sprite pola

};

