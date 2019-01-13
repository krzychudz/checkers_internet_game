#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include "Constants.h"

using namespace std;
using namespace sf;

/*
	Klasa obs³uguj¹ca pionki na planszy. Tutaj za pomoc¹ wielu metod analizowane jest to czy gracz wykona³ poprawny ruch i czy mo¿na dany 
	ruch wys³aæ do serwera a nastêpnie do drugiego gracza czy nie.
*/

class Pawn
{
public:
	Pawn();
	~Pawn();


	void draw(RenderWindow* wnd);
	bool checkCapture(int side);
	bool checkCaptureKing(int side);

	int getField(int x, int y);
	Vector3i countEnemyPawn(int side, int sX, int sY, int dX, int dY);
	bool update(int sX, int sY, int dX, int dY, int side);
	void updateCapture(int sX, int sY, int delX, int delY, int dX, int dY, int side, bool d);
	void updateNormalMove(int sX, int sY, int dX, int dY, int side, bool d);
	std::string getMap();
	void setMap(char map[]);
	bool checkArrayCorrectness(int x1, int x2, int x3, int x4);
	bool checkSkew(int sX, int sY, int dX, int dY, int lenght);
	bool isLose(int side);

	bool checkNextMove(int side, int i, int j);
	bool checkNextMoveKing(int side, int i, int j);
	bool getNextMove();

	void updateKingPawn(int side);

	Sprite getWhitePawnSprite();
	Sprite getBlackPawnSprite();

protected:
	Texture whitePawnTexture;			//Textura bia³ego piona
	Texture blackPawnTexture;			//Textura czarnego piona
	Texture whitePawnTextureD;
	Texture blackPawnTextureD;
	Sprite whitePawnSprite;				//Sprite bia³ego piona
	Sprite blackPawnSprite;				//Sprite czarnego piona
	Sprite whitePawnSpriteD;
	Sprite blackPawnSpriteD;


	int pawnMap[8][8] =					//Reprezantacja pionow na planszy
	{									// 0 - Pion czarny 1 - damka czarna 2 - Pole puste 3 - Pion bialy 4 - Damka biala
		{2,0,2,0,2,0,2,0},
		{0,2,0,2,0,2,0,2},
		{2,0,2,0,2,0,2,0},
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

	bool nextMove = false;

};

