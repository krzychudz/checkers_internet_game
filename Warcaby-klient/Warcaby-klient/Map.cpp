#include "pch.h"
#include "Map.h"


Map::Map()
{
	if (!fieldTextureWhite.loadFromFile("Image/fieldTexWhite.png"))
	{
		cout << "Error: Load image from file - Map tile" << endl;
	}

	if (!fieldTextureBlack.loadFromFile("Image/fieldTexBlack.png"))
	{
		cout << "Error: Load image from file - Map tile" << endl;
	}

}


Map::~Map()
{
}

/*
	Metoda klasy Map rysuj¹ca planszê
	Parametry: RenderWindow *wnd - wskaŸnik na okno na którym bêdzie rysowana plansza
*/

void Map::draw(RenderWindow * wnd)
{
	Texture textureHolder[2] =
	{
		{fieldTextureWhite},
		{fieldTextureBlack}
	};

	int pointer = 0;

	fieldSprite.setTexture(textureHolder[0]);

	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			fieldSprite.setPosition(j * FIELD_WIDTH, i * FIELD_HEIGHT);
			wnd->draw(fieldSprite);
			if(j*64 != 448)
				pointer = pointer + 1;
			if (pointer == 2)
				pointer = 0;
			fieldSprite.setTexture(textureHolder[pointer]);
		}
	}




}
