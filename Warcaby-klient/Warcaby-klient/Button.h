#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;

/*
	Klasa obsługująca przycisk.
*/

class Button
{
public:
	Button(std::string text, Vector2f size, Color c);
	~Button();

	bool isButtonPressed(RenderWindow * wnd);
	void draw(RenderWindow * wnd);
	void setPosition(float x, float y);
	void setColor(Color color);

private:
	RectangleShape buttonBackground;


	Font font;
	Text buttonLabel;



};

