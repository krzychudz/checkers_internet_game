#include "pch.h"
#include "Pawn.h"
#include <math.h>
#include <sstream>


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

	if (!blackPawnTextureD.loadFromFile("Image/blackPawnD.png"))
	{
		cout << "Error: load image from file - PawnD " << endl;
	}
	else
	{
		blackPawnSpriteD.setTexture(blackPawnTextureD);
	}

	if (!whitePawnTextureD.loadFromFile("Image/whitePawnD.png"))
	{
		cout << "Error: load image from file - PawnD " << endl;
	}
	else
	{
		whitePawnSpriteD.setTexture(whitePawnTextureD);
	}



}


Pawn::~Pawn()
{
}

/*
	Metoda rysuj�ca map� pion�w na podstawie tablicy pawnMap
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
			else if (pawnMap[i][j] == 3)
			{
				whitePawnSprite.setPosition(j * PAWN_WIDTH, i * PAWN_HEIGHT);
				wnd->draw(whitePawnSprite);
			}
			else if (pawnMap[i][j] == 1)
			{
				blackPawnSpriteD.setPosition(j * PAWN_WIDTH, i * PAWN_HEIGHT);
				wnd->draw(blackPawnSpriteD);
			}
			else if (pawnMap[i][j] == 4)
			{
				whitePawnSpriteD.setPosition(j * PAWN_WIDTH, i * PAWN_HEIGHT);
				wnd->draw(whitePawnSpriteD);
			}
			else
				continue;
			
		}
	}


}


/*
	Metoda pobieraj�ca warto�� z tablicy getField spod podanego indeksu
	Parametry: int x - index x
			   int y - index y
*/

int Pawn::getField(int x, int y)
{
	return pawnMap[x][y];
}

/*
	Metoda licz�ca ilo�� przeciwnych pion�w pomi�dzy dwoma podanymi polami w ukosie
	Parametry: int side - strona
			   int sX - index x pola pierwszego
			   int sY - index y pola pierwszego
			   int dX - index x pola drugiego
			   int dY - index y pola drugiego
	Return:
				Vector3i - Vector zawieraj�cy informacje:
					Vector3i.x = -2 - je�eli podane pola nie s� po ukosie lub napotkano pion gracza
					Vector3i.x > 0 - ilo�� pionow przeciwnika w lini 
					Vector3i.y - pozycja x wrogiego piona 
					Vector3i.z - pozcyja y wrogiego piona
					
*/
Vector3i Pawn::countEnemyPawn(int side, int sX, int sY, int dX, int dY)
{
	Vector3i data;
	data.x = data.y = data.z = 0;

	int counter = 0;
	int iX, iY;
	iX = 0;
	iY = 0;

	if (sX > dX)
		iX = -1;
	if (sX < dX)
		iX = 1;

	if (sY > dY)
		iY = -1;
	if (sY < dY)
		iY = 1;

	if (sX == dX)
	{
		data.x = -2;
		return data;
	}

	if(sY == dY)
	{
		data.x = -2;
		return data;
	}


	for (int i = 0; i < 8; i++)
	{
		sX = sX + iX;
		sY = sY + iY;

		if (sX == dX && sY == dY)
			return data;

			if (side == white)
			{
				if (pawnMap[sX][sY] == 0 || pawnMap[sX][sY] == 1)
				{
					counter++;
					data.x = counter;
					data.y = sX;
					data.z = sY;
				}
				if (pawnMap[sX][sY] == 3 || pawnMap[sX][sY] == 4)
				{
					data.x = -2;
					return data;
				}
			}
			if (side == black)
			{
				if (pawnMap[sX][sY] == 3 || pawnMap[sX][sY] == 4)
				{
					counter++;
					data.x = counter;
					data.y = sX;
					data.z = sY;
				}
				if (pawnMap[sX][sY] == 0 || pawnMap[sX][sY] == 1)
				{
					data.x = -2;
					return data;
				}
			}
		
		
	}

	data.x = -1;

	return data;
}

/*
	Metoda sprawdzaj�ca poprawno�� ruchu i aktualizuj�ca tablic� pion�w
	Parametry: int sX - koordynat x z kt�rego bierzemy piona
			   int sY - koordynat y z kt�rego bierzemy piona
			   int dX - koordynat x na kt�rego chcemy po�o�y� piona
			   int dY - koordynat y na kt�rego chcemy po�o�y� piona
	Return: 
			   bool - True - ruch poprawny
			   bool - False - ruch niepoprawny
*/

bool Pawn::update(int sX, int sY, int dX, int dY, int side)
{
	nextMove = false;

	if (sX == dX && sY == dY)
		return false;

	bool update = false;

	if (map[dX][dY] == 1 || !checkArrayCorrectness(dX, dY, 2, 2))
		return false;

	if (pawnMap[sX][sY] == 2)
		return false;

	if (checkCapture(side) || checkCaptureKing(side))						// Ruch gdy trzeba wykonac bicie
	{

		if ((pawnMap[sX][sY] == 0 || pawnMap[sX][sY] == 3)) // Zwyk�e piony
		{
			if (side == white)
			{
				int midX = ceil(abs(sX + dX) / 2);
				int midY = ceil(abs(sY + dY) / 2);

				if (!checkSkew(sX, sY, dX, dY, 2))
					return false;

				if ((pawnMap[midX][midY] == black || pawnMap[midX][midY] == 1) && pawnMap[dX][dY] == 2)
				{
					updateCapture(sX, sY, midX, midY, dX, dY, side, false);

					/*
					if (checkCapture(side))
						nextMove = true;*/

					if (checkNextMove(side, dX, dY))
						nextMove = true;

					return true;
				}

			}
			else
			{
				int midX = ceil(fabs(sX + dX) / 2);
				int midY = ceil(fabs(sY + dY) / 2);

				if (!checkSkew(sX, sY, dX, dY, 2))
					return false;

				if ((pawnMap[midX][midY] == white || pawnMap[midX][midY] == 4) && pawnMap[dX][dY] == 2)
				{
					updateCapture(sX, sY, midX, midY, dX, dY, side, false);

					/*
					if (checkCapture(side))
						nextMove = true;*/

					if (checkNextMove(side, dX, dY))
						nextMove = true;

					return true;
				}
			}

			return false;
		}

		else if ((pawnMap[sX][sY] == 1 || pawnMap[sX][sY] == 4))
		{
			//damki

			if (side == white)
			{

				if (countEnemyPawn(white, sX, sY, dX, dY).x == -1) // czy s� po skosie
					return false;

				if (countEnemyPawn(white, sX, sY, dX, dY).x == -2) 
					return false;

				if (countEnemyPawn(white, sX, sY, dX, dY).x == 1 && pawnMap[dX][dY] == 2)
				{
					int midX = countEnemyPawn(white, sX, sY, dX, dY).y;
					int midY = countEnemyPawn(white, sX, sY, dX, dY).z;

					updateCapture(sX, sY, midX, midY, dX, dY, side, true);

					if (checkNextMoveKing(side, dX, dY))
						nextMove = true;

					return true;
				}

			}
			else
			{
				if (countEnemyPawn(black, sX, sY, dX, dY).x == -1)
					return false;

				if (countEnemyPawn(black, sX, sY, dX, dY).x == -2)
					return false;

				if (countEnemyPawn(black, sX, sY, dX, dY).x == 1 && pawnMap[dX][dY] == 2)
				{

					int midX = countEnemyPawn(black, sX, sY, dX, dY).y;
					int midY = countEnemyPawn(black, sX, sY, dX, dY).z;

					updateCapture(sX, sY, midX, midY, dX, dY, side, true);

					if (checkNextMoveKing(side, dX, dY))
						nextMove = true;

					return true;
				}

			}

			return false;
		}
	}

	else   // ruch gdy bicia nie ma
	{
			if (map[dX][dY] == 1 || pawnMap[dX][dY] != 2)
				return false;

			if (pawnMap[sX][sY] == 0 || pawnMap[sX][sY] == 3) // zykly pion
			{
				if (side == black)
				{
					if (sX - dX > 0 || !checkSkew(sX, sY, dX, dY, 1))
						return false;
				}

				if (side == white)
				{
					if (sX - dX < 0 || !checkSkew(sX, sY, dX, dY, 1))
						return false;
				}

				updateNormalMove(sX, sY, dX, dY, side, false);
			} 

			else if (pawnMap[sX][sY] == 1 || pawnMap[sX][sY] == 4) // DAMKA
			{
				if (countEnemyPawn(side, sX, sY, dX, dY).x == 0)
					updateNormalMove(sX, sY, dX, dY, side, true);
				else
					return false;
			}

			return true;

	}

}

/*
	Metoda wykonjaca ruch w kt�rym gracz bije piona przeciwnika
	Parametry: int sX - koordynat x z kt�rego bierzemy piona
			   int sY - koordynat y z kt�rego bierzemy piona
			   int dX - koordynat x na kt�rego chcemy po�o�y� piona
			   int dY - koordynat y na kt�rego chcemy po�o�y� piona
			   int side - strona, czarne, biale
*/

void Pawn::updateCapture(int sX, int sY,int delX, int delY, int dX, int dY, int side, bool d)
{

	if (!d)
	{
		pawnMap[sX][sY] = 2;
		pawnMap[delX][delY] = 2;
		if (side == black)
			pawnMap[dX][dY] = 0;
		else if (side == white)
			pawnMap[dX][dY] = 3;
	}
	else
	{
		pawnMap[sX][sY] = 2;
		pawnMap[delX][delY] = 2;
		if (side == black)
			pawnMap[dX][dY] = 1;
		else if (side == white)
			pawnMap[dX][dY] = 4;
	}

}

/*
	Metoda wykonjaca zwyk�y ruch piona lub damki
	Parametry: int sX - koordynat x z kt�rego bierzemy piona
			   int sY - koordynat y z kt�rego bierzemy piona
			   int dX - koordynat x na kt�rego chcemy po�o�y� piona
			   int dY - koordynat y na kt�rego chcemy po�o�y� piona
			   int side - strona, czarne, biale
*/

void Pawn::updateNormalMove(int sX, int sY, int dX, int dY, int side, bool d)
{
	if (!d)
	{
		pawnMap[sX][sY] = 2;
		if (side == 0)
			pawnMap[dX][dY] = 0;
		else if (side == 3)
			pawnMap[dX][dY] = 3;
	}
	else
	{
		pawnMap[sX][sY] = 2;
		if (side == 0)
			pawnMap[dX][dY] = 1;
		else if (side == 3)
			pawnMap[dX][dY] = 4;
	}

}

/*
	Metoda konwertuj�ca aktualny stan mapy do stringa
	Return: 
			std::string - mapa w postaci stringa
*/

std::string Pawn::getMap()
{
	std::string str = "";
	std::ostringstream ss;

	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			ss.str("");
			ss << pawnMap[i][j];
			str = str + ss.str();
		}
	}

	return str;
}

/*
	Metoda ustawiaj�ca map� 
	Parametry:
			char map[] - Wskaznik na tablic� char'�w zawieraj�c� map� do ustawienia
*/

void Pawn::setMap(char map[])
{
	int z = 1;
	int tmp;
	
	cout << "MAP UPDATE: " << endl;

	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			tmp = map[z] - '0';
			pawnMap[i][j] = tmp;
			cout << tmp;
			z++;
		}
		cout << endl;
	}

	cout << "MAP UPDATE END" << endl;

}

/*
	Metoda sprawdzaj�ca czy nic nie odwo�uje si� poza zakres mapy
	Parametry:
			Zmienne do sprawdzenia
	Return:
		   bool - True - wszytko OK
		   bool - False - Kt�ra� zmienna odwo�uje si� poza tablic�
*/

bool Pawn::checkArrayCorrectness(int x1, int x2, int x3, int x4)
{
	if (x1 < 0 || x1 >= 8)
		return false;
	if (x2 < 0 || x2 >= 8)
		return false;
	if (x3 < 0 || x3 >= 8)
		return false;
	if (x4 < 0 || x4 >= 8)
		return false;

	return true;



}

/*
	Metoda sprawdzaj�ca czy podane 2 pola le�� po skosie odpowiednio daleko od siebie
	Parametry:
			int sX - wsp�rz�dna X pierwszego pola
			int sY - wsp�rz�dna Y perwszego pola
			int dX - wsp�rz�dna X drugiego pola
			int dY - wsp�rz�dna Y drugiego pola
			int lenght - odleg�o�� p�l
	Return:
		   bool - True - Podane pola le�� na skos od siebie w odleg�o�ci length
		   bool - False - Podane pola nie le�� na skos od siebie w odleg�o�ci length
*/

bool Pawn::checkSkew(int sX, int sY, int dX, int dY, int lenght)
{
	if (sX - lenght == dX && sY - lenght == dY)
		return true;
	if (sX + lenght == dX && sY - lenght == dY)
		return true;
	if (sX + lenght == dX && sY + lenght == dY)
		return true;
	if (sX - lenght == dX && sY + lenght == dY)
		return true;

	return false;
}

/*
	Metoda sprawdzaj�ca czy przeciwnik przegral
	Parametry:
			int side - strona
	Return:
		   bool - True - Gracz przegral
		   bool - False - Gracz ciagle moze grac
*/


bool Pawn::isLose(int side)
{
	int pawnCounter = 0;

	if (side == white)
	{
		for (int i = 0; i < 8; i++)
		{
			for (int j = 0; j < 8; j++)
			{
				if (pawnMap[i][j] == 3 || pawnMap[i][j] == 4)
					pawnCounter++;
			}
		}
	}
	else
	{
		for (int i = 0; i < 8; i++)
		{
			for (int j = 0; j < 8; j++)
			{
				if (pawnMap[i][j] == 0 || pawnMap[i][j] == 1)
					pawnCounter++;
			}
		}
	}

	if (pawnCounter == 0)
		return true;
	else
		return false;
}

/*
	Metoda sprawdzaj�ca czy z danego pola mo�na wykona� jakie� bicie dla zwyk�ego piona
	Parametry:
			int side - strona
			int i - pole X
			int j - pole Y
	Return:
		   bool - True - z pola o wsp�rz�dnych i,j mo�na wykona� bicie
		   bool - False - z pola o wsp�rz�dnych i,j nie mo�na wykona� bicie
*/

bool Pawn::checkNextMove(int side, int i, int j)
{
	if (side == white && pawnMap[i][j] == white)
	{
		if ((pawnMap[i - 1][j - 1] == black || pawnMap[i - 1][j - 1] == 1) && pawnMap[i - 2][j - 2] == 2
			&& checkArrayCorrectness(i - 1, j - 1, i - 2, j - 2))
		{
			return true;
		}
		if ((pawnMap[i - 1][j + 1] == black || pawnMap[i - 1][j + 1] == 1) && pawnMap[i - 2][j + 2] == 2 && i - 1 >= 0
			&& checkArrayCorrectness(i - 1, j + 1, i - 2, j + 2))
		{
			return true;
		}
		if ((pawnMap[i + 1][j + 1] == black || pawnMap[i + 1][j + 1] == 1) && pawnMap[i + 2][j + 2] == 2
			&& checkArrayCorrectness(i + 1, j + 1, i + 2, j + 2))
		{
			return true;
		}
		if ((pawnMap[i + 1][j - 1] == black || pawnMap[i + 1][j - 1] == 1) && pawnMap[i + 2][j - 2] == 2
			&& checkArrayCorrectness(i + 1, j - 1, i + 2, j - 2))
		{
			return true;
		}
	}

	if (side == black && pawnMap[i][j] == black)
	{

		if ((pawnMap[i - 1][j - 1] == white || pawnMap[i - 1][j - 1] == 4) && pawnMap[i - 2][j - 2] == 2
			&& checkArrayCorrectness(i - 1, j - 1, i - 2, j - 2))
		{
			return true;
		}
		if ((pawnMap[i - 1][j + 1] == white || pawnMap[i - 1][j + 1] == 4) && pawnMap[i - 2][j + 2] == 2
			&& checkArrayCorrectness(i - 1, j + 1, i - 2, j + 2))
		{
			return true;
		}
		if ((pawnMap[i + 1][j + 1] == white || pawnMap[i + 1][j + 1] == 4) && pawnMap[i + 2][j + 2] == 2
			&& checkArrayCorrectness(i + 1, j + 1, i + 2, j + 2))
		{
			return true;
		}
		if ((pawnMap[i + 1][j - 1] == white || pawnMap[i + 1][j - 1] == 4) && pawnMap[i + 2][j - 2] == 2
			&& checkArrayCorrectness(i + 1, j - 1, i + 2, j - 2))
		{
			return true;
		}

	}

	return false;
}

/*
	Metoda sprawdzaj�ca czy z danego pola mo�na wykona� jakie� bicie dla damki
	Parametry:
			int side - strona
			int i - pole X
			int j - pole Y
	Return:
		   bool - True - z pola o wsp�rz�dnych i,j mo�na wykona� bicie
		   bool - False - z pola o wsp�rz�dnych i,j nie mo�na wykona� bicie
*/

bool Pawn::checkNextMoveKing(int side, int i, int j)
{
	if (side == white)
	{
		if (pawnMap[i][j] == 4)
		{
			for (int x = 1, y = 1; x < 8; x++, y++)
			{
				if (countEnemyPawn(white, i, j, i - x, j - y).x == 1 && pawnMap[i - x][j - y] == 2 && countEnemyPawn(white, i, j, i - x, j - y).x != -2
					&& checkArrayCorrectness(i - x, j - y, 2, 2))
				{
					return true;
				}

				else if (countEnemyPawn(white, i, j, i - x, j + y).x == 1 && pawnMap[i - x][j + y] == 2 && countEnemyPawn(white, i, j, i - x, j + y).x != -2
					&& checkArrayCorrectness(i - x, j + y, 2, 2))
				{
					return true;
				}

				else if (countEnemyPawn(white, i, j, i + x, j + y).x == 1 && pawnMap[i + x][j + y] == 2 && countEnemyPawn(white, i, j, i + x, j + y).x != -2
					&& checkArrayCorrectness(i + x, j + y, 2, 2))
				{
					return true;
				}

				else if (countEnemyPawn(white, i, j, i + x, j - y).x == 1 && pawnMap[i + x][j - y] == 2 && countEnemyPawn(white, i, j, i + x, j - y).x != -2
					&& checkArrayCorrectness(i + x, j - y, 2, 2))
				{
					return true;
				}

			}
		}
	}

	else
	{
		if (pawnMap[i][j] == 1)
		{

			for (int x = 1, y = 1; x < 8; x++, y++)
			{

				if (countEnemyPawn(black, i, j, i - x, j - y).x == 1 && pawnMap[i - x][j - y] == 2 && countEnemyPawn(black, i, j, i - x, j - y).x != -2
					&& checkArrayCorrectness(i - x, j - y, 2, 2))
				{
					return true;
				}

				else if (countEnemyPawn(black, i, j, i - x, j + y).x == 1 && pawnMap[i - x][j + y] == 2 && countEnemyPawn(black, i, j, i - x, j + y).x != -2
					&& checkArrayCorrectness(i - x, j + y, 2, 2))
				{
					return true;
				}

				else if (countEnemyPawn(black, i, j, i + x, j + y).x == 1 && pawnMap[i + x][j + y] == 2 && countEnemyPawn(black, i, j, i + x, j + y).x != -2
					&& checkArrayCorrectness(i + x, j + y, 2, 2))
				{
					return true;
				}

				else if (countEnemyPawn(black, i, j, i + x, j - y).x == 1 && pawnMap[i + x][j - y] == 2 && countEnemyPawn(black, i, j, i + x, j - y).x != -2
					&& checkArrayCorrectness(i + x, j - y, 2, 2))
				{
					return true;
				}
			}
		}
	}

	return false;

}

/*
	Metoda zwracaj�ca infomacj� o tym czy dany gracz posiada kolejny ruch

	Return:
		   bool - True - gracz posiada dodatkowy ruch
		   bool - False -gracz nie posiada dodatkowego ruchu
*/

bool Pawn::getNextMove()
{
	return nextMove;
}


/*
	Metoda sprawdzaj�ca czy zwyk�y pionek nie powinien sta� si� damk�
	Parametry:
		   int side - strona
*/

void Pawn::updateKingPawn(int side)
{
	if (side == white)
	{
		for (int i = 0; i < 8; i++)
			if (pawnMap[0][i] == white)
				pawnMap[0][i] = 4;
	}
	else
	{
		for (int i = 0; i < 8; i++)
			if (pawnMap[7][i] == black)
				pawnMap[7][i] = 1;
	}

}

/*
	Metoda sprawdzaj�ca czy gracz ma obowi�zkowe bicia zwyk�ym pionem
	Parametry:
			int side - strona
	Return:
		   bool - True - Gracz ma obowi�zkowe bicia
		   bool - False - Gracz nie ma obowi�zkowych bi�
*/

bool Pawn::checkCapture(int side)
{
	for(int i = 0; i < 8; i++)
		for (int j = 0; j < 8; j++)
		{
			if (side == white && pawnMap[i][j] == white)
			{
				if ((pawnMap[i - 1][j - 1] == black || pawnMap[i - 1][j - 1] == 1) && pawnMap[i - 2][j - 2] == 2 
					&& checkArrayCorrectness(i - 1, j - 1, i - 2, j - 2))
				{
					return true;
				}
				if ((pawnMap[i - 1][j + 1] == black || pawnMap[i -1 ][j + 1] == 1) && pawnMap[i - 2][j + 2] == 2 && i - 1 >= 0 
					&& checkArrayCorrectness(i - 1, j + 1, i - 2, j + 2))
				{
					return true;
				}
				if ((pawnMap[i + 1][j + 1] == black || pawnMap[i + 1][j + 1] == 1) && pawnMap[i + 2][j + 2] == 2 
					&& checkArrayCorrectness(i + 1, j + 1, i + 2, j + 2))
				{
					return true;
				}
				if ((pawnMap[i + 1][j - 1] == black || pawnMap[i + 1][j - 1] == 1) && pawnMap[i + 2][j - 2] == 2 
					&& checkArrayCorrectness(i + 1, j - 1, i + 2, j - 2))
				{
					return true;
				}
			}

			if (side == black && pawnMap[i][j] == black)
			{
			
					if ((pawnMap[i - 1][j - 1] == white || pawnMap[i - 1][j - 1] == 4) && pawnMap[i - 2][j - 2] == 2
						&& checkArrayCorrectness(i - 1, j - 1, i - 2, j - 2))
					{
						return true;
					}
					if ((pawnMap[i - 1][j + 1] == white || pawnMap[i - 1][j + 1] == 4) && pawnMap[i - 2][j + 2] == 2
						&& checkArrayCorrectness(i - 1, j + 1, i - 2, j + 2))
					{
						return true;
					}
					if ((pawnMap[i + 1][j + 1] == white || pawnMap[i + 1][j + 1] == 4) && pawnMap[i + 2][j + 2] == 2
						&& checkArrayCorrectness(i + 1, j + 1, i + 2, j + 2))
					{
						return true;
					}
					if ((pawnMap[i + 1][j - 1] == white || pawnMap[i + 1][j - 1] == 4) && pawnMap[i + 2][j - 2] == 2
						&& checkArrayCorrectness(i + 1, j - 1, i + 2, j - 2))
					{
						return true;
					}
		
			}

		}

	return false;

}

/*
	Metoda sprawdzaj�ca czy gracz ma obowi�zkowe bicia damk�
	Parametry:
			int side - strona
	Return:
		   bool - True - Gracz ma obowi�zkowe bicia
		   bool - False - Gracz nie ma obowi�zkowych bi�
*/

bool Pawn::checkCaptureKing(int side)
{
	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 8; j++)
		{
			if (side == white)
			{
				if (pawnMap[i][j] == 4)
				{
					for (int x = 1, y = 1; x < 8; x++, y++)
					{
						if (countEnemyPawn(white, i, j, i - x, j - y).x == 1 && pawnMap[i - x][j - y] == 2 && countEnemyPawn(white, i, j, i - x, j - y).x != -2
							&& checkArrayCorrectness(i - x, j - y, 2, 2))
						{
							return true;
						}

						else if (countEnemyPawn(white, i, j, i - x, j + y).x == 1 && pawnMap[i - x][j + y] == 2 && countEnemyPawn(white, i, j, i - x, j + y).x != -2
							&& checkArrayCorrectness(i - x, j + y, 2, 2))
						{
							return true;
						}

						else if (countEnemyPawn(white, i, j, i + x, j + y).x == 1 && pawnMap[i + x][j + y] == 2 && countEnemyPawn(white, i, j, i + x, j + y).x != -2
							&& checkArrayCorrectness(i + x, j + y, 2, 2))
						{
							return true;
						}

						else if (countEnemyPawn(white, i, j, i + x, j - y).x == 1 && pawnMap[i + x][j - y] == 2 && countEnemyPawn(white, i, j, i + x, j - y).x != -2
							&& checkArrayCorrectness(i + x, j - y, 2, 2))
						{
							return true;
						}

					}
				}
			}

			else
			{
				if (pawnMap[i][j] == 1)
				{

					for (int x = 1, y = 1; x < 8; x++, y++)
					{

						if (countEnemyPawn(black, i, j, i - x, j - y).x == 1 && pawnMap[i - x][j - y] == 2 && countEnemyPawn(black, i, j, i - x, j - y).x != -2
							&& checkArrayCorrectness(i - x, j - y, 2, 2))
						{
							return true;
						}

						else if (countEnemyPawn(black, i, j, i - x, j + y).x == 1 && pawnMap[i - x][j + y] == 2 && countEnemyPawn(black, i, j, i - x, j + y).x != -2
							&& checkArrayCorrectness(i - x, j + y, 2, 2))
						{
							return true;
						}

						else if (countEnemyPawn(black, i, j, i + x, j + y).x == 1 && pawnMap[i + x][j + y] == 2 && countEnemyPawn(black, i, j, i + x, j + y).x != -2
							&& checkArrayCorrectness(i + x, j + y, 2, 2))
						{
							return true;
						}

						else if (countEnemyPawn(black, i, j, i + x, j - y).x == 1 && pawnMap[i + x][j - y] == 2 && countEnemyPawn(black, i, j, i + x, j - y).x != -2
							&& checkArrayCorrectness(i + x, j - y, 2, 2))
						{
							return true;
						}
					}
				}
			}

		}

	return false;
}

/*
	Metoda zwracaj�ca sprite'a bia�ego piona
	Return:
		   Sprite - sprite 
*/

Sprite Pawn::getWhitePawnSprite()
{
	return whitePawnSprite;
}

/*
	Metoda zwracaj�ca sprite'a czarnego piona
	Return:
		   Sprite - sprite
*/

Sprite Pawn::getBlackPawnSprite()
{
	return blackPawnSprite;
}
