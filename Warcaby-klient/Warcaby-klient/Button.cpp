#include "pch.h"
#include "Button.h"
#include "Constants.h"

Button::Button(std::string text, Vector2f size, Color c)
{
	font.loadFromFile(FONT_PATH);

	buttonLabel.setString(text);
	buttonLabel.setFont(font);
	buttonLabel.setCharacterSize(16);
	buttonLabel.setFillColor(Color::White);

	buttonBackground.setSize(size);
	buttonBackground.setFillColor(c);

	buttonLabel.setOrigin(buttonLabel.getLocalBounds().width / 2.0f, buttonLabel.getLocalBounds().height / 2.0f);
	buttonBackground.setOrigin(buttonBackground.getLocalBounds().width / 2.0f, buttonBackground.getLocalBounds().height / 2.0f);
}


Button::~Button()
{
}

/*
	Metoda sprawdzaj¹ca czy kursor myszy znajduje sie na przycisku
	Parametry: RenderWindow *wnd - wskaŸnik na okno g³ówne

	Return:
			   bool - True - kursor znajduje sie na przycisku
			   bool - False - kursor nie znajduje sie na przycisku
*/

bool Button::isButtonPressed(RenderWindow * wnd)
{
	Vector2f mousePosition;
	mousePosition.x = Mouse::getPosition(*wnd).x;
	mousePosition.y = Mouse::getPosition(*wnd).y;


	if (buttonBackground.getGlobalBounds().contains(mousePosition))
		return true;

	return false;
}

/*
	Metoda rysuj¹ca przycisk 
	Parametry: RenderWindow *wnd - wskaŸnik na okno g³ówne
*/

void Button::draw(RenderWindow * wnd)
{
	wnd->draw(buttonBackground);
	wnd->draw(buttonLabel);
}

/*
	Metoda ustawiaj¹ca przycisk na ekranie
	Parametry: 
		float x - pozycja x
		float y - pozycja y
*/

void Button::setPosition(float x, float y)
{
	buttonBackground.setPosition(x, y);
	buttonLabel.setPosition(x, y+5);
}

/*
	Metoda ustawiaj¹ca kolor napisu
	Parametry:
		Color color - kolor
*/

void Button::setColor(Color color)
{
	buttonLabel.setFillColor(color);
}
