#include "pch.h"
#include "Pawn.h"


Pawn::Pawn()
{
	if (!whitePawnTexture.loadFromFile("Image/whitePawn.png"))
	{
		cout << "Error: load image from file - Pawn " << endl;
	}
	else
	{
		whitePawnSprite.setTexture(whitePawnTexture);
	}

	if (!blackPawnTexture.loadFromFile("Image/blackPawn.png"))
	{
		cout << "Error: load image from file - Pawn " << endl;
	}
	else
	{
		blackPawnSprite.setTexture(blackPawnTexture);
	}


}


Pawn::~Pawn()
{
}

/*
	Metoda klasy Pawn rysuj�ca map� pion�w na podstawie tablicy pawnMap
	Parametry: RenderWindow *wnd - wska�nik na okno na kt�rym b�d� rysowanie piony
*/

void Pawn::draw(RenderWindow *wnd)
{

	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (pawnMap[i][j] == 0)
			{
				blackPawnSprite.setPosition(j * PAWN_WIDTH, i * PAWN_HEIGHT);
				wnd->draw(blackPawnSprite);
			}
			else if (pawnMap[i][j] == 1)
			{
				whitePawnSprite.setPosition(j * PAWN_WIDTH, i * PAWN_HEIGHT);
				wnd->draw(whitePawnSprite);
			}
			else
				continue;
			
		}
	}


}

/*
	Metoda klasy Pawn pobieraj�ca warto�� z tablicy getField spod podanego indeksu
	Parametry: int x - index x
			   int y - index y
*/

int Pawn::getField(int x, int y)
{
	return pawnMap[x][y];
}

/*
	Metoda klasy Pawn sprawdzaj�ca poprawno�� ruchu i aktualizuj�ca tablic� pion�w
	Parametry: int sX - koordynat x z kt�rego bierzemy piona
			   int sY - koordynat y z kt�rego bierzemy piona
			   int dX - koordynat x na kt�rego chcemy po�o�y� piona
			   int dY - koordynat y na kt�rego chcemy po�o�y� piona
*/

bool Pawn::update(int sX, int sY, int dX, int dY)
{
	int side = pawnMap[sX][sY];

	if (map[dX][dY] == side || pawnMap[dX][dY] != 2 || dX > 7 || dY > 7)
	{
		return false;
	}
	else
	{
		pawnMap[sX][sY] = 2;
		pawnMap[dX][dY] = 1;
		return true;
	}

}

Sprite Pawn::getWhitePawnSprite()
{
	return whitePawnSprite;
}

Sprite Pawn::getBlackPawnSprite()
{
	return blackPawnSprite;
}
