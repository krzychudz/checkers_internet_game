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
	bool update(int sX, int sY, int dX, int dY);

	Sprite getWhitePawnSprite();
	Sprite getBlackPawnSprite();

protected:
	Texture whitePawnTexture;			//Textura bia³ego piona
	Texture blackPawnTexture;			//Textura czarnego piona
	Sprite whitePawnSprite;				//Sprite bia³ego piona
	Sprite blackPawnSprite;				//Sprite czarnego piona

	int pawnMap[8][8] =					//Reprezantacja pionow na planszy
	{									// 0 - Pion czarny 1 - damka czarna 2 - Pole puste 3 - Pion bialy 4 - Damka biala
		{0,2,0,2,0,2,0,2},
		{2,0,2,0,2,0,2,0},
		{0,2,0,2,0,2,0,2},
		{2,2,2,2,2,2,2,2},
		{2,2,2,2,2,2,2,2},
		{3,2,3,2,3,2,3,2},
		{2,3,2,3,2,3,2,3},
		{3,2,3,2,3,2,3,2}
	};

	int map[8][8] =			// 0 - pole czarne 1 - pole biale
	{
		{1,0,1,0,1,0,1,0},
		{0,1,0,1,0,1,0,1},
		{1,0,1,0,1,0,1,0},
		{0,1,0,1,0,1,0,1},
		{1,0,1,0,1,0,1,0},
		{0,1,0,1,0,1,0,1},
		{1,0,1,0,1,0,1,0},
		{0,1,0,1,0,1,0,1}
	};


};

