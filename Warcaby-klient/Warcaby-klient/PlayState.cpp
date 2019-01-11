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
	Metoda rysuj¹ca grê
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
	Metoda aktualizuj¹ca grê
*/

void PlayState::update()
{
	if (gameOverWin && !sendData)	//Je¿eli gracz wygra³
	{
		game->socket.disconnect();							// Od³¹czenie siê 
		game->pushState(new GameOverState(game,true,""));   // Wyœwietlenie stanu koniec gry z informacj¹ o wygranej
	}

	if (gameOverLose && !sendData) //Je¿eli gracz przegra³
	{
		game->socket.disconnect();
		game->pushState(new GameOverState(game, false,""));
	}

	if (surrender) // Je¿eli gracz siê podda³ 
	{
		moves = "S" + pawnMap.getMap() + "X";						 // Ustawienie odpowiednej informacji do wy³ania
		std::thread sendThread(&PlayState::sendDataToServer, this);  // Uruchomienie w¹tku który wyœle dan¹ informacjê do serwera
		sendThread.detach();
	}

	if (surrenderEnemy) // Wykycie sytuacji w której przeciwnik siê podda³
	{
		game->socket.disconnect();
		game->pushState(new GameOverState(game, true, "Your opponent has surrender"));	// Wyœwietlenie stanu koniec gry z informacj¹ o wygranej z odpowiednim powodem
	}

	if (enemyConnError)
	{
		game->socket.disconnect();
		game->pushState(new GameOverState(game, true, "Your opponent has disconnected"));
	}

	if (sendData && !gameOverLose && !surrender && !enemyConnError)		//Je¿eli aplikacja powinna wys³aæ dane
	{
		std::thread sendThread(&PlayState::sendDataToServer, this);		//Uruchomienie w¹tku który wyœle dane do serwera
		sendThread.detach();

		sendData = false;
	}

	if (isTurn != side && receive && !gameOverWin && !gameOverLose && !surrender && !surrenderEnemy && !enemyConnError) //Je¿eli aplikacja powinna oczekiwaæ na dane
	{
		std::thread receiveThread(&PlayState::receiveData, this);   //Uruchomienie w¹tku który bêdzie oczekiwa³ na dane
		receiveThread.detach();

		receive = false;
	}

	if (isTurn == side) // Obs³uga Timera 
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
			if (skipMoves == 3)		// Je¿eli po raz trzeci gracz jest pasywny
			{
				moves = "T" + pawnMap.getMap() + "X";						
				std::thread sendThread(&PlayState::sendDataToServer, this); // Wys³anie informacji do serwera
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
	Metoda obs³uguj¹ca wejœcie wyjœcie w grze
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


		if (event.type == Event::MouseButtonReleased)								// Je¿eli zwolniono przycisk myszy
		{

			if (surrenderButton->isButtonPressed(&game->window) && isTurn == side)	// Nacisniêto przycisk poddaj siê
				surrender = true;

			if (pawnMap.getField(Mouse::getPosition(game->window).y / PAWN_WIDTH,
				Mouse::getPosition(game->window).x / PAWN_HEIGHT) == 1 && side == black &&
				Mouse::getPosition(game->window).y / PAWN_WIDTH < 8 && Mouse::getPosition(game->window).x / PAWN_HEIGHT < 8) // Czy w polu na które klikamy jest damka
					king = true;
			
			
			if (pawnMap.getField(Mouse::getPosition(game->window).y / PAWN_WIDTH,
				Mouse::getPosition(game->window).x / PAWN_HEIGHT) == 4 && side == white &&
				Mouse::getPosition(game->window).y / PAWN_WIDTH < 8 && Mouse::getPosition(game->window).x / PAWN_HEIGHT < 8) // Czy w polu na które klikamy jest damka
					king = true;
			
			

			if (isTurn == side && (pawnMap.getField(Mouse::getPosition(game->window).y / PAWN_WIDTH,
				Mouse::getPosition(game->window).x / PAWN_HEIGHT) == side || king) && !selected &&
				Mouse::getPosition(game->window).y / PAWN_WIDTH < 8 && Mouse::getPosition(game->window).x / PAWN_HEIGHT < 8) // Czy w polu na które klikamy jest pionek lub damka
			{
					

				sourceX = (Mouse::getPosition(game->window).y / PAWN_HEIGHT);		// Wspó³rzêdne w tablicy pionów (zaznacznoego pionka)
				sourceY = (Mouse::getPosition(game->window).x / PAWN_WIDTH);
				selected = true;
				king = false;

				selectedRectangle.setPosition(sourceY * 64, sourceX * 64);
				drawRectangle = true;

			}

			else if (selected && isTurn == side)
			{
				destX = (Mouse::getPosition(game->window).y / PAWN_HEIGHT);		// Wspó³rzêdne w talbicy pionków (pola na który chcemy piona przenieœæ)
				destY = (Mouse::getPosition(game->window).x / PAWN_WIDTH);

				drawRectangle = false;

				if (pawnMap.update(sourceX, sourceY, destX, destY, side))		// Sprawdzenia czy mo¿na wykonaæ dany ruch przenosz¹c piona z pola (sourceX, sourceY) na pole (destX, destY)
				{

					if (!pawnMap.getNextMove())				// Sprawdzenie czy gracz nie musi wykonaæ jescze jedno bicia, w przypadku bicia wielokrotnego
					{
						
						pawnMap.updateKingPawn(side);		// Sprawdzenie czy któryœ z naszych pionków nie powienien zmieniæ siê w damkê

						if (side == white)					// Przygotowanie danych do wys³ania w zale¿noœci od strony
						{
							if (pawnMap.isLose(black))
							{
								moves = "L" + pawnMap.getMap() + "X";	// Je¿eli wyktyto ¿e przeciwnik przegra³
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

						if (side == white)		// Zmiana tury na turê przeciwnika
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
	Metoda pozwalaj¹ca wys³aæ dane do serwera
*/

void PlayState::sendDataToServer()
{
	sf::Socket::Status status = game->socket.send(moves.c_str(), 66, t);		// Wys³anie aktualnych danych do serwera

	if (status == sf::Socket::Done)			// Je¿eli wszystkie dane zosta³y wys³ane
	{
		if (moves[0] == 'S')				// Je¿eli wysy³aliœmy informacjê o tym ¿e siê poddajemy
		{
			game->socket.disconnect();
			game->pushState(new GameOverState(game, false,""));		// Wyœwietlamy stan w którym informujemy o pora¿ce
		}
		else if (moves[0] == 'T')									// Je¿eli wysy³aliœmy informacjê o tym ¿e byliœmy pasywni przez 3 rundy
		{
			game->socket.disconnect();
			game->pushState(new GameOverState(game, false,"You must play!")); // Wyœwietlamy stan w którym informujemy o pora¿ce z powodem
		}

		receive = true;
	}
	else if (status == sf::Socket::Disconnected)									// W przypadku b³êdów z wys³aniem
		game->pushState(new ServerErrorState(game, "Server connection error"));
	else if (status == sf::Socket::Error)
		game->pushState(new ServerErrorState(game, "Server connection error"));
	else
		game->pushState(new ServerErrorState(game, "Server connection error"));

}

/*
	Metoda pozwalaj¹ca odebraæ dane do serwera
*/

void PlayState::receiveData()
{
	sf::Socket::Status status = game->socket.receive(buf, 66, t);		// Oczekujemy na dane

	if (status == sf::Socket::Done)
	{
		if (buf[0] == 'b')					// Je¿eli przeciwnik wykona³ ruch normalnie 
		{
			clock.restart();			
			pawnMap.setMap(buf);			// aktualizujemy pionki
			memset(buf, '*', 66);			// ustawimy * w ca³ym buforze
			isTurn = black;					// zmieniamy turê na nasz¹
		}
		else if (buf[0] == 'w')				// Je¿eli przeciwnik wykona³ ruch normalnie 
		{
			clock.restart();
			pawnMap.setMap(buf);
			memset(buf, '*', 66);
			isTurn = white;
		}
		else if (buf[0] == 'L')				// Je¿eli przeciwnik wykry³ ¿e przegraliœmy
		{
			pawnMap.setMap(buf);
			memset(buf, '*', 66);
			gameOverLose = true;			// Flaga przegranej
		}

		else if (buf[0] == 'S')				// Je¿eli przeciwnik siê podda³
		{
			memset(buf, '*', 66);
			surrenderEnemy = true;
		}
		else if (buf[0] == 'E')				// Je¿eli przeciwnik straci³ po³¹czenie z serwerem
			enemyConnError = true;

		else if (buf[0] == 'T')				// Je¿eli przeciwnik 3 razy by³ pasywny
			gameOverWin = true;

	}					
	else if (status == sf::Socket::Disconnected && !surrender)						// Obs³uga b³êdów przy odbieraniu
		game->pushState(new ServerErrorState(game, "Server connection error"));
	else if (status == sf::Socket::Error && !surrender)
		game->pushState(new ServerErrorState(game, "Server connection error"));	
	else
	{
		if(!surrender)
			game->pushState(new ServerErrorState(game, "Server connection error"));
	}

}

