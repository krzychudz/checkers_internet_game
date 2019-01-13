#pragma once
#include "GameState.h"
#include "Constants.h"
#include "Map.h"
#include "Pawn.h"
#include "Button.h"


/*
	Klasa obs³uguj¹ca stan g³ównej gry. To tutaj jest rysowana gra, oraz obs³ugiwane jest wejœcie wyjœcie. To ta klasa pozwala na u¿ywanie
	myszki w grze, zajmuje siê wysy³aniem oraz odbieraniem danych, aktualizowaniem stanu rozgrywki.
*/

class PlayState : public GameState
{
public:
	PlayState(Game * game, int side);
	~PlayState();

	virtual void draw();
	virtual void update();
	virtual void handleInput();

private:
	void sendDataToServer();
	void receiveData();

	Button* surrenderButton;

	Text sideText;
	Text turnText;
	Text clockText;
	Font font;

	Clock clock;
	Time time;

	int timer = 60;

	char buf[66];
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
	bool enemyConnError = false;
	bool nextMove = false;
	bool king = false;

	int skipMoves = 0;

	RectangleShape selectedRectangle;
	bool drawRectangle = false;

};

