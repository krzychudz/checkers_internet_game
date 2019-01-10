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

bool Button::isButtonPressed(RenderWindow * wnd)
{
	Vector2f mousePosition;
	mousePosition.x = Mouse::getPosition(*wnd).x;
	mousePosition.y = Mouse::getPosition(*wnd).y;


	if (buttonBackground.getGlobalBounds().contains(mousePosition))
		return true;

	return false;
}

void Button::draw(RenderWindow * wnd)
{
	wnd->draw(buttonBackground);
	wnd->draw(buttonLabel);
}

void Button::setPosition(float x, float y)
{
	buttonBackground.setPosition(x, y);
	buttonLabel.setPosition(x, y+5);
}

void Button::setColor(Color color)
{
	buttonLabel.setFillColor(color);
}
