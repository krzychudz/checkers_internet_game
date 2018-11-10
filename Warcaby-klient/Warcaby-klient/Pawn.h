#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include "Constants.h"

using namespace std;
using namespace sf;

class Pawn
{
public:
	Pawn();
	~Pawn();


	void draw(RenderWindow* wnd);

	int getField(int x, int y);
	int update(int sX, int sY, int dX, int dY);

protected:
	Texture whitePawnTexture;			//Textura bia�ego piona
	Texture blackPawnTexture;			//Textura czarnego piona
	Sprite whitePawnSprite;				//Sprite bia�ego piona
	Sprite blackPawnSprite;				//Sprite czarnego piona

	int pawnMap[8][8] =					//Reprezantacja pionow na planszy
	{									// 0 - Pion czarny 1 - Pion bialy 2 - Pole puste
		{0,2,0,2,0,2,0,2},
		{2,0,2,0,2,0,2,0},
		{0,2,0,2,0,2,0,2},
		{2,2,2,2,2,2,2,2},
		{2,2,2,2,2,2,2,2},
		{1,2,1,2,1,2,1,2},
		{2,1,2,1,2,1,2,1},
		{1,2,1,2,1,2,1,2},
	};


};
