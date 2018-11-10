#include <SFML/Graphics.hpp>
#include <iostream>
#include "Constants.h"
using namespace std;
using namespace sf;
#pragma once

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

