#include "pch.h"
#include "PlayState.h"
#include "MenuState.h"
#include "GameOverState.h"
#include "ServerErrorState.h"
#include <sstream>
#include <Thread>

PlayState::PlayState(Game * game, int side)
{
	this->game = game;
	this->side = side;

	font.loadFromFile(FONT_PATH);
	
	sideText.setOrigin(sideText.getLocalBounds().width / 2.0f, sideText.getLocalBounds().height / 2.0f);
	sideText.setCharacterSize(25);
	sideText.setFillColor(Color::White);
	sideText.setFont(font);

	if (side == black)
	{
		sideText.setString("Black");
		receive = true;
	}
	else
		sideText.setString("White");
	

	sideText.setPosition(600, 50);

	turnText.setOrigin(sideText.getLocalBounds().width / 2.0f, sideText.getLocalBounds().height / 2.0f);
	turnText.setCharacterSize(25);
	turnText.setFillColor(Color::White);
	turnText.setFont(font);
	turnText.setString("Your Turn");
	turnText.setPosition(610, 150);

	surrenderButton = new Button("Surrender!", Vector2f(150, 50), Color::Black);
	surrenderButton->setPosition(660,350);

	clockText.setOrigin(clockText.getLocalBounds().width / 2.0f, clockText.getLocalBounds().height / 2.0f);
	clockText.setCharacterSize(25);
	clockText.setFillColor(Color::White);
	clockText.setFont(font);
	clockText.setString("1:00");
	clockText.setPosition(620, 220);

	selectedRectangle.setSize(Vector2f(64, 64));
	selectedRectangle.setOutlineColor(Color::Red);
	selectedRectangle.setOutlineThickness(3);
	selectedRectangle.setFillColor(Color(255, 255, 255, 0));
}


PlayState::~PlayState()
{
	delete surrenderButton;
}

/*
	Metoda rysuj�ca gr�
*/

void PlayState::draw()
{

	if (isTurn == side)
		game->window.draw(turnText);
	
	if (surrenderButton->isButtonPressed(&game->window) && isTurn == side)
		surrenderButton->setColor(Color::Red);
	else
		surrenderButton->setColor(Color::White);


	surrenderButton->draw(&game->window);

	game->window.draw(sideText);

	map.draw(&game->window);
	pawnMap.draw(&game->window);

	game->window.draw(clockText);

	if (drawRectangle)
		game->window.draw(selectedRectangle);
	
}

/*
	Metoda aktualizuj�ca gr�
*/

void PlayState::update()
{
	if (gameOverWin && !sendData)	//Je�eli gracz wygra�
	{
		game->socket.disconnect();							// Od��czenie si� 
		game->pushState(new GameOverState(game,true,""));   // Wy�wietlenie stanu koniec gry z informacj� o wygranej
	}

	if (gameOverLose && !sendData) //Je�eli gracz przegra�
	{
		game->socket.disconnect();
		game->pushState(new GameOverState(game, false,""));
	}

	if (surrender) // Je�eli gracz si� podda� 
	{
		moves = "S" + pawnMap.getMap() + "X";						 // Ustawienie odpowiednej informacji do wy�ania
		std::thread sendThread(&PlayState::sendDataToServer, this);  // Uruchomienie w�tku kt�ry wy�le dan� informacj� do serwera
		sendThread.detach();
	}

	if (surrenderEnemy) // Wykycie sytuacji w kt�rej przeciwnik si� podda�
	{
		game->socket.disconnect();
		game->pushState(new GameOverState(game, true, "Your opponent has surrender"));	// Wy�wietlenie stanu koniec gry z informacj� o wygranej z odpowiednim powodem
	}

	if (enemyConnError)
	{
		game->socket.disconnect();
		game->pushState(new GameOverState(game, true, "Your opponent has disconnected"));
	}

	if (sendData && !gameOverLose && !surrender && !enemyConnError)		//Je�eli aplikacja powinna wys�a� dane
	{
		std::thread sendThread(&PlayState::sendDataToServer, this);		//Uruchomienie w�tku kt�ry wy�le dane do serwera
		sendThread.detach();

		sendData = false;
	}

	if (isTurn != side && receive && !gameOverWin && !gameOverLose && !surrender && !surrenderEnemy && !enemyConnError) //Je�eli aplikacja powinna oczekiwa� na dane
	{
		std::thread receiveThread(&PlayState::receiveData, this);   //Uruchomienie w�tku kt�ry b�dzie oczekiwa� na dane
		receiveThread.detach();

		receive = false;
	}

	if (isTurn == side) // Obs�uga Timera 
	{
		stringstream ss;
		time = clock.getElapsedTime();

		int tmp = timer - time.asSeconds();
		int min = tmp / 60;
		int sec = tmp - (min*60);

		if (sec < 10)
		{
			ss << min << ":0" << sec;
			clockText.setFillColor(Color::Red);
		}
		else
			ss << min << ":" << sec;

		string str = ss.str();
		clockText.setString(str);

		if (tmp == 0)
		{
			clockText.setString("1:00");

			skipMoves++;
			if (skipMoves == 3)		// Je�eli po raz trzeci gracz jest pasywny
			{
				moves = "T" + pawnMap.getMap() + "X";						
				std::thread sendThread(&PlayState::sendDataToServer, this); // Wys�anie informacji do serwera
				sendThread.detach();
			}
			else
			{
				if (side == white)
				{
					moves = "b" + pawnMap.getMap() + "X";
					isTurn = black;
				}
				else
				{
					moves = "w" + pawnMap.getMap() + "X";
					isTurn = white;
				}

				clockText.setFillColor(Color::White);
				sendData = true;
			}
		}
	}
}

/*
	Metoda obs�uguj�ca wej�cie wyj�cie w grze
*/

void PlayState::handleInput()
{
	sf::Event event;
	
	while (game->window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
			game->window.close();	

		switch (event.type) 
		{
		case sf::Event::KeyPressed:
			if (event.key.code == sf::Keyboard::Q)
			{
			
			}
			break;
		}


		if (event.type == Event::MouseButtonReleased)								// Je�eli zwolniono przycisk myszy
		{

			if (surrenderButton->isButtonPressed(&game->window) && isTurn == side)	// Nacisni�to przycisk poddaj si�
				surrender = true;

			if (pawnMap.getField(Mouse::getPosition(game->window).y / PAWN_WIDTH,
				Mouse::getPosition(game->window).x / PAWN_HEIGHT) == 1 && side == black &&
				Mouse::getPosition(game->window).y / PAWN_WIDTH < 8 && Mouse::getPosition(game->window).x / PAWN_HEIGHT < 8) // Czy w polu na kt�re klikamy jest damka
					king = true;
			
			
			if (pawnMap.getField(Mouse::getPosition(game->window).y / PAWN_WIDTH,
				Mouse::getPosition(game->window).x / PAWN_HEIGHT) == 4 && side == white &&
				Mouse::getPosition(game->window).y / PAWN_WIDTH < 8 && Mouse::getPosition(game->window).x / PAWN_HEIGHT < 8) // Czy w polu na kt�re klikamy jest damka
					king = true;
			
			

			if (isTurn == side && (pawnMap.getField(Mouse::getPosition(game->window).y / PAWN_WIDTH,
				Mouse::getPosition(game->window).x / PAWN_HEIGHT) == side || king) && !selected &&
				Mouse::getPosition(game->window).y / PAWN_WIDTH < 8 && Mouse::getPosition(game->window).x / PAWN_HEIGHT < 8) // Czy w polu na kt�re klikamy jest pionek lub damka
			{
					

				sourceX = (Mouse::getPosition(game->window).y / PAWN_HEIGHT);		// Wsp�rz�dne w tablicy pion�w (zaznacznoego pionka)
				sourceY = (Mouse::getPosition(game->window).x / PAWN_WIDTH);
				selected = true;
				king = false;

				selectedRectangle.setPosition(sourceY * 64, sourceX * 64);
				drawRectangle = true;

			}

			else if (selected && isTurn == side)
			{
				destX = (Mouse::getPosition(game->window).y / PAWN_HEIGHT);		// Wsp�rz�dne w talbicy pionk�w (pola na kt�ry chcemy piona przenie��)
				destY = (Mouse::getPosition(game->window).x / PAWN_WIDTH);

				drawRectangle = false;

				if (pawnMap.update(sourceX, sourceY, destX, destY, side))		// Sprawdzenia czy mo�na wykona� dany ruch przenosz�c piona z pola (sourceX, sourceY) na pole (destX, destY)
				{

					if (!pawnMap.getNextMove())				// Sprawdzenie czy gracz nie musi wykona� jescze jedno bicia, w przypadku bicia wielokrotnego
					{
						
						pawnMap.updateKingPawn(side);		// Sprawdzenie czy kt�ry� z naszych pionk�w nie powienien zmieni� si� w damk�

						if (side == white)					// Przygotowanie danych do wys�ania w zale�no�ci od strony
						{
							if (pawnMap.isLose(black))
							{
								moves = "L" + pawnMap.getMap() + "X";	// Je�eli wyktyto �e przeciwnik przegra�
								gameOverWin = true;
							}
							else
								moves = "b" + pawnMap.getMap() + "X";	// Normalny ruch

						}
						else
						{
							if (pawnMap.isLose(white))
							{
								moves = "L" + pawnMap.getMap() + "X";
								gameOverWin = true;
							}
							else
								moves = "w" + pawnMap.getMap() + "X";
						}


						sendData = true;

						if (side == white)		// Zmiana tury na tur� przeciwnika
							isTurn = black;
						else
							isTurn = white;


						clockText.setString("1:00");			// Reset timera
						clockText.setFillColor(Color::White);
					}
				}
				
		
				selected = false;

			}
		}
	}
	
	
}

/*
	Metoda pozwalaj�ca wys�a� dane do serwera
*/

void PlayState::sendDataToServer()
{
	sf::Socket::Status status = game->socket.send(moves.c_str(), 66, t);		// Wys�anie aktualnych danych do serwera

	if (status == sf::Socket::Done)			// Je�eli wszystkie dane zosta�y wys�ane
	{
		if (moves[0] == 'S')				// Je�eli wysy�ali�my informacj� o tym �e si� poddajemy
		{
			game->socket.disconnect();
			game->pushState(new GameOverState(game, false,""));		// Wy�wietlamy stan w kt�rym informujemy o pora�ce
		}
		else if (moves[0] == 'T')									// Je�eli wysy�ali�my informacj� o tym �e byli�my pasywni przez 3 rundy
		{
			game->socket.disconnect();
			game->pushState(new GameOverState(game, false,"You must play!")); // Wy�wietlamy stan w kt�rym informujemy o pora�ce z powodem
		}

		receive = true;
	}
	else if (status == sf::Socket::Disconnected)									// W przypadku b��d�w z wys�aniem
		game->pushState(new ServerErrorState(game, "Server connection error"));
	else if (status == sf::Socket::Error)
		game->pushState(new ServerErrorState(game, "Server connection error"));
	else
		game->pushState(new ServerErrorState(game, "Server connection error"));

}

/*
	Metoda pozwalaj�ca odebra� dane do serwera
*/

void PlayState::receiveData()
{
	sf::Socket::Status status = game->socket.receive(buf, 66, t);		// Oczekujemy na dane

	if (status == sf::Socket::Done)
	{
		if (buf[0] == 'b')					// Je�eli przeciwnik wykona� ruch normalnie 
		{
			clock.restart();			
			pawnMap.setMap(buf);			// aktualizujemy pionki
			memset(buf, '*', 66);			// ustawimy * w ca�ym buforze
			isTurn = black;					// zmieniamy tur� na nasz�
		}
		else if (buf[0] == 'w')				// Je�eli przeciwnik wykona� ruch normalnie 
		{
			clock.restart();
			pawnMap.setMap(buf);
			memset(buf, '*', 66);
			isTurn = white;
		}
		else if (buf[0] == 'L')				// Je�eli przeciwnik wykry� �e przegrali�my
		{
			pawnMap.setMap(buf);
			memset(buf, '*', 66);
			gameOverLose = true;			// Flaga przegranej
		}

		else if (buf[0] == 'S')				// Je�eli przeciwnik si� podda�
		{
			memset(buf, '*', 66);
			surrenderEnemy = true;
		}
		else if (buf[0] == 'E')				// Je�eli przeciwnik straci� po��czenie z serwerem
			enemyConnError = true;

		else if (buf[0] == 'T')				// Je�eli przeciwnik 3 razy by� pasywny
			gameOverWin = true;

	}					
	else if (status == sf::Socket::Disconnected && !surrender)						// Obs�uga b��d�w przy odbieraniu
		game->pushState(new ServerErrorState(game, "Server connection error"));
	else if (status == sf::Socket::Error && !surrender)
		game->pushState(new ServerErrorState(game, "Server connection error"));	
	else
	{
		if(!surrender)
			game->pushState(new ServerErrorState(game, "Server connection error"));
	}

}

