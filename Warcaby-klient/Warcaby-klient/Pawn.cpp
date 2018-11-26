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

}


Pawn::~Pawn()
{
}

/*
	Metoda klasy Pawn rysuj¹ca mapê pionów na podstawie tablicy pawnMap
	Parametry: RenderWindow *wnd - wskaŸnik na okno na którym bêd¹ rysowanie piony
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
			else
				continue;
			
		}
	}


}


/*
	Metoda klasy Pawn pobieraj¹ca wartoœæ z tablicy getField spod podanego indeksu
	Parametry: int x - index x
			   int y - index y
*/

int Pawn::getField(int x, int y)
{
	return pawnMap[x][y];
}

/*
	Metoda klasy Pawn sprawdzaj¹ca poprawnoœæ ruchu i aktualizuj¹ca tablicê pionów
	Parametry: int sX - koordynat x z którego bierzemy piona
			   int sY - koordynat y z którego bierzemy piona
			   int dX - koordynat x na którego chcemy po³o¿yæ piona
			   int dY - koordynat y na którego chcemy po³o¿yæ piona
	Return: 
			   bool - True - ruch poprawny
			   bool - False - ruch niepoprawny
*/

bool Pawn::update(int sX, int sY, int dX, int dY)
{
	int side = pawnMap[sX][sY];
	
	bool update = false;

	if (map[dX][dY] == 1 || !checkArrayCorrectness(dX, dY, 2, 2))
		return false;
	

	if (checkCapture(side))						// Ruch gdy trzeba wykonac bicie
	{
		cout << "Musisz wykonac bicie" << endl;
		
		if (side == white)
		{

			int midX = ceil(abs(sX + dX) / 2);
			int midY = ceil(abs(sY + dY) / 2);

			if (!checkSkew(sX, sY, dX, dY, 2))
				return false;


			if (pawnMap[midX][midY] == black && pawnMap[dX][dY] == 2)
			{	
				updateCapture(sX, sY, midX, midY, dX, dY, side);
				return true;
			}
			
		}

		else
		{
			int midX = ceil(fabs(sX + dX) / 2);
			int midY = ceil(fabs(sY + dY) / 2);

			if (!checkSkew(sX, sY, dX, dY, 2))
				return false;

			if (pawnMap[midX][midY] == white && pawnMap[dX][dY] == 2)
			{
				updateCapture(sX, sY, midX, midY, dX, dY, side);
				return true;
			}
		}
		
		return false;
	}
	else   // ruch gdy bicia nie ma
	{
		/*
		if (map[dX][dY] == 1 || pawnMap[dX][dY] != 2 || dX > 7 || dY > 7)
		{
			return false;
		}
		*/

		if (map[dX][dY] == 1 || pawnMap[dX][dY] != 2)
			return false;
		
		
		if (side == black)
		{
			/*
			if (sX - dX > 0 || abs(sY - dY) > 1 || abs(sX - dX) > 1)
			{
				return false;
			}
			*/

			if (sX - dX > 0 || !checkSkew(sX, sY, dX, dY, 1))
				return false;
			
		}

		if (side == white)
		{
			/*
			if (sX - dX < 0 || abs(sY - dY) > 1 || abs(sX - dX) > 1)
			{
				return false;
			}
			*/

			if (sX - dX < 0 || !checkSkew(sX, sY, dX, dY, 1))
				return false;
			
		}

		updateNormalMove(sX, sY, dX, dY, side);
		return true;

	}
	

}

/*
	Metoda klasy Pawn wykonjaca ruch w którym gracz bije piona przeciwnika
	Parametry: int sX - koordynat x z którego bierzemy piona
			   int sY - koordynat y z którego bierzemy piona
			   int dX - koordynat x na którego chcemy po³o¿yæ piona
			   int dY - koordynat y na którego chcemy po³o¿yæ piona
			   int side - strona, czarne, biale
*/

void Pawn::updateCapture(int sX, int sY,int delX, int delY, int dX, int dY, int side)
{
	cout << sX << " " << sY << " " << delX << " " << delY << " " << dX << " " << dY << endl;


	pawnMap[sX][sY] = 2;
	pawnMap[delX][delY] = 2;
	if (side == black)
		pawnMap[dX][dY] = 0;
	else if (side == white)
		pawnMap[dX][dY] = 3;

	for (int x = 0; x < 8; x++)//debug
	{
		for (int z = 0; z < 8; z++)
			cout << pawnMap[x][z] << " ";

		cout << endl;
	}


}

/*
	Metoda klasy Pawn wykonjaca zwyk³y ruch piona
	Parametry: int sX - koordynat x z którego bierzemy piona
			   int sY - koordynat y z którego bierzemy piona
			   int dX - koordynat x na którego chcemy po³o¿yæ piona
			   int dY - koordynat y na którego chcemy po³o¿yæ piona
			   int side - strona, czarne, biale
*/

void Pawn::updateNormalMove(int sX, int sY, int dX, int dY, int side)
{
	pawnMap[sX][sY] = 2;
	if (side == 0)
		pawnMap[dX][dY] = 0;
	else if (side == 3)
		pawnMap[dX][dY] = 3;


}

/*
	Metoda klasy Pawn konwertuj¹ca aktualny stan mapy do stringa
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
	Metoda klasy Pawn ustawiaj¹ca mapê 
	Parametry:
			char map[] - Wskaznik na tablicê char'ów zawieraj¹c¹ mapê do ustawienia
*/

void Pawn::setMap(char map[])
{
	int z = 1;
	int tmp;

	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			tmp = map[z] - '0';
			pawnMap[i][j] = tmp;
			z++;
		}
	}

}

/*
	Metoda klasy Pawn sprawdzaj¹ca czy nic nie odwo³uje siê poza zakres mapy
	Parametry:
			Zmienne do sprawdzenia
	Return:
		   bool - True - wszytko OK
		   bool - False - Któraœ zmienna odwo³uje siê poza tablicê
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
	Metoda klasy Pawn sprawdzaj¹ca czy podane 2 pola le¿¹ po skosie odpowiednio daleko od siebie
	Parametry:
			int sX - wspó³rzêdna X pierwszego pola
			int sY - wspó³rzêdna Y perwszego pola
			int dX - wspó³rzêdna X drugiego pola
			int dY - wspó³rzêdna Y drugiego pola
			int lenght - odleg³oœæ pól
	Return:
		   bool - True - Podane pola le¿¹ na skos od siebie w odleg³oœci length
		   bool - False - Podane pola nie le¿¹ na skos od siebie w odleg³oœci length
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
	Metoda klasy Pawn sprawdzaj¹ca czy gracz ma obowi¹zkowe bicia
	Parametry:
			int side - strona
	Return:
		   bool - True - Gracz ma obowi¹zkowe bicia
		   bool - False - Gracz nie ma obowi¹zkowych biæ
*/

bool Pawn::checkCapture(int side)
{
	for(int i = 0; i < 8; i++)
		for (int j = 0; j < 8; j++)
		{
			if (side == white && pawnMap[i][j] == white)
			{
				if (pawnMap[i - 1][j - 1] == black && pawnMap[i - 2][j - 2] == 2 
					&& checkArrayCorrectness(i - 1, j - 1, i - 2, j - 2))
				{
					return true;
				}
				if (pawnMap[i - 1][j + 1] == black && pawnMap[i - 2][j + 2] == 2 && i - 1 >= 0 
					&& checkArrayCorrectness(i - 1, j + 1, i - 2, j + 2))
				{
					return true;
				}
				if (pawnMap[i + 1][j + 1] == black && pawnMap[i + 2][j + 2] == 2 
					&& checkArrayCorrectness(i + 1, j + 1, i + 2, j + 2))
				{
					return true;
				}
				if (pawnMap[i + 1][j - 1] == black && pawnMap[i + 2][j - 2] == 2 
					&& checkArrayCorrectness(i + 1, j - 1, i + 2, j - 2))
				{
					return true;
				}
			}

			if (side == black && pawnMap[i][j] == black)
			{
			
					if (pawnMap[i - 1][j - 1] == white && pawnMap[i - 2][j - 2] == 2
						&& checkArrayCorrectness(i - 1, j - 1, i - 2, j - 2))
					{
						return true;
					}
					if (pawnMap[i - 1][j + 1] == white && pawnMap[i - 2][j + 2] == 2
						&& checkArrayCorrectness(i - 1, j + 1, i - 2, j + 2))
					{
						return true;
					}
					if (pawnMap[i + 1][j + 1] == white && pawnMap[i + 2][j + 2] == 2
						&& checkArrayCorrectness(i + 1, j + 1, i + 2, j + 2))
					{
						return true;
					}
					if (pawnMap[i + 1][j - 1] == white && pawnMap[i + 2][j - 2] == 2
						&& checkArrayCorrectness(i + 1, j - 1, i + 2, j - 2))
					{
						return true;
					}
		
			}

		}

	return false;

}

Sprite Pawn::getWhitePawnSprite()
{
	return whitePawnSprite;
}

Sprite Pawn::getBlackPawnSprite()
{
	return blackPawnSprite;
}
