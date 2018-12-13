#pragma once
#include "GameState.h"
#include "Constants.h"
#include "Map.h"
#include "Pawn.h"
#include "Button.h"

class PlayState : public GameState
{
public:
	PlayState(Game * game, int side);
	~PlayState();

	virtual void draw();
	virtual void update();
	virtual void handleInput();

private:
	void pauseGame();
	void sendDataToServer();
	void receiveData();

	Button* surrenderButton;

	Text sideText;
	Text turnText;
	Font font;

	char buf[64];
	size_t t;

	int side = 3;				//Zmienna okreœlaj¹ca któr¹ stron¹ gramy, nadawana przez server
	Turn isTurn = white;		//Zmienna okreœlaj¹ca czyja jest tura, ustawiana przez server

	Map map;					//plansza
	Pawn pawnMap;				//piony

	bool leftMousePressed = false;

	Sprite whitePawnSprite = pawnMap.getWhitePawnSprite();
	Sprite blackPawnSprite = pawnMap.getBlackPawnSprite();

	int pawnNumber;				// Numer aktualnie przenoszonego piona

	std::string moves;

	int sourceX = 0;
	int sourceY = 0;

	int destX = 0;
	int destY = 0;

	bool selected = false;
	bool sendData = false;
	bool receive = false;
	bool gameOverLose = false;
	bool gameOverWin = false;
	bool surrender = false;
	bool surrenderEnemy = false;

};

