#include "header.h"

float speed = 12;
bool gamePause = false;
bool gameOver = false;
bool gamePlay = false;
bool gameControls = false;
bool restartGame = false;
int score = -2;

Player::Player(std::string fileName)
{
	if (!playerTexture.loadFromFile(fileName))		//zaladowanie tekstury gracza 
		std::cerr << "Error";
	playerSprite.setOrigin(0, 96);
	playerSprite.setPosition(200, 800);			//nadanie pocz¹tkowej pozycji 
}

void Player::drawPlayer(sf::RenderWindow &window)
{
	sf::Time switchTime = clockAnimation.getElapsedTime();

	if (y == 800) row = 0;                         // animacja biegu
	else row = 1;                                // animacja skoku

	if (switchTime.asSeconds() > 0.05f)             // 0.1s = czas, przez który wyœwietla siê jedna klatka animacji
	{
		if (imageCount == 11)                    // iloœæ klatek dla jednej animacji - 1
			imageCount = 0;
		else
			imageCount++;
		clockAnimation.restart();
	}
	if (row == 1) imageCount = 0;
	sf::IntRect rectSourceSprite(imageCount * 96, row * 96, 96, 96);   // wym. jednej klatki = 96x96
	sf::Sprite playerSprite(playerTexture, rectSourceSprite);

	playerSprite.setTextureRect(rectSourceSprite);
	playerSprite.setOrigin(0, 96);
	playerSprite.setPosition(200, y);           // updatuje pozycje gracza

	window.draw(playerSprite);

}

//funkcja updatuj¹ca pozycjê gracza w trakcie skoku
void Player::updateMovement()
{
	y += velocity;

	if (y < 800) velocity += gravity;
	else if (y > 800) y = 800;

	playerSprite.setPosition(200, y);
}


Background::Background(std::string fileName)
{
	if (!backgroundTexture.loadFromFile(fileName))		//zaladowanie tekstury t³a
		std::cerr << "Error";


	//ustawienia wlasciwosci t³a
	backgroundTexture.setRepeated(true);
	backgroundSprite.setTexture(backgroundTexture);
	backgroundSprite.setPosition(0, 0);
	backgroundSprite.setTextureRect(sf::IntRect(X, 0, 2200, 1200));
}

//funkcja obslugujaca poruszanie sie i przyspieszanie tla
void Background::updateBackground(float &speed)
{

	if (X < 2200) X += speed;
	else X = 0;
	backgroundSprite.setTextureRect(sf::IntRect(X, 0, 2200, 1200));

	sf::Time ileMinelo = clock.getElapsedTime();
	if (ileMinelo.asSeconds() > 3)
	{
		if (speed < 30) speed += 0.5;			
		clock.restart();
	}

}

//wyswetlanie tla
void Background::drawBackground(sf::RenderWindow &window)
{
	window.draw(backgroundSprite);
}


Scoreboard::Scoreboard(std::string fileName)
{
	if (!font.loadFromFile(fileName))
	{
		std::cerr << "Error";
	}

	//zaladowanie czcionki, ustawienia rozmiaru i pozycji napisow
	textScore.setFont(font);
	textScore.setString("Score: ");
	textScore.setFillColor(sf::Color::Black);
	textScore.setCharacterSize(60);
	textScore.setPosition(960, 20);

	numbers.setFont(font);
	numbers.setString("0");
	numbers.setFillColor(sf::Color::Black);
	numbers.setCharacterSize(60);
	numbers.setPosition(1190, 20);

	gameOver.setFont(font);
	gameOver.setString("GAME OVER");
	gameOver.setFillColor(sf::Color::Black);
	gameOver.setCharacterSize(180);
	gameOver.setPosition(560, 200);

	yourScore.setFont(font);
	yourScore.setString("Your score: ");
	yourScore.setFillColor(sf::Color::Black);
	yourScore.setCharacterSize(80);
	yourScore.setPosition(750, 440);

	playAgain.setFont(font);
	playAgain.setString("Press 'R' to play again");
	playAgain.setFillColor(sf::Color::Black);
	playAgain.setCharacterSize(60);
	playAgain.setPosition(680, 560);
}

//funkcja konwertujaca aktualny wynik w postaci liczby calkowitej na stringa
void Scoreboard::convertToString(int score)
{
	std::stringstream s;
	s << score;
	numbers.setString(s.str());
}

//wyswietlanie biezacego wyniku
void Scoreboard::drawScore(sf::RenderWindow &window)
{
	window.draw(textScore);
	window.draw(numbers);
}

//wyswietlanie ekranu po przegraniu gry
void Scoreboard::drawGameOver(sf::RenderWindow &window)
{
	window.draw(gameOver);
	window.draw(yourScore);
	numbers.setCharacterSize(80);
	numbers.setPosition(1350, 440);
	numbers.setFillColor(sf::Color::Red);
	window.draw(numbers);
	window.draw(playAgain);
}


Obstacle::Obstacle(std::string fileName)
{
	if (!obstacleTexture.loadFromFile(fileName))		//zaladowanie tekstury przeszkody
		std::cerr << "Error";

	//ustawienia wartosci poczatkowych przeszkod
	obstacleSprite.setTexture(obstacleTexture);
	obstacleSprite.setOrigin(80, 80);
	obstacleSprite.setPosition(2200, 820);
}

//wyswietlanie przeszkody w okienku
void Obstacle::drawObstacle(sf::RenderWindow &window)
{
	window.draw(obstacleSprite);
}


//funkcja wykrywajaca kolizje gracza z przeszkoda
void collisionDetection(const Player* gracz, const Obstacle* przeszkoda)
{
	sf::FloatRect playerShape = gracz->playerSprite.getGlobalBounds();
	sf::FloatRect obstacleShape = przeszkoda->obstacleSprite.getGlobalBounds();

	playerShape.width = 96;                 // szerokosc gracza
	playerShape.height = 96;                // wysokosc gracza

	//zakonczenie gry w przypadku kolizji
	if (playerShape.intersects(obstacleShape))	
	{
		gamePause = true;
		gameOver = true;
	}
}

//funkcje aktualizujace pozycje przeszkod
void updateObstacle(Obstacle &przeszkoda1)
{
	sf::Clock clock;
	if (przeszkoda1.X > 0) przeszkoda1.X = przeszkoda1.X - speed;
	else
	{
		przeszkoda1.X = 2200;
		score++;
	}

	(przeszkoda1.obstacleSprite).setPosition(przeszkoda1.X, 820);

}

void updateObstacle2(Obstacle &przeszkoda1)
{
	sf::Clock clock;
	if (przeszkoda1.X > 0) przeszkoda1.X = przeszkoda1.X - speed;
	else
	{
		przeszkoda1.X = 3300;
		score++;
	}

	(przeszkoda1.obstacleSprite).setPosition(przeszkoda1.X, 820);
}

//funkcja, ktora przywraca grze wartosci poczatkowe po przegranej
void playAgain(Player &gracz, Obstacle &przeszkoda1, Obstacle &przeszkoda2, Scoreboard &tab)
{
	speed = 8;
	score = 0;
	gamePause = false;
	gameOver = false;
	gracz.y = 800;
	przeszkoda1.X = 2200;
	przeszkoda2.X = 3300;
	tab.numbers.setCharacterSize(60);
	tab.numbers.setPosition(1190, 20);
	tab.numbers.setFillColor(sf::Color::Black);
};


Menu::Menu(std::string fileName, std::string fileName2, float width, float height)
{
	if (!font.loadFromFile("resources/pixel.ttf"))
		std::cerr << "Error";
	if (!menuTexture.loadFromFile(fileName))
		std::cerr << "Error";
	if (!controlsTexture.loadFromFile(fileName2))
		std::cerr << "Error";

	menuSprite.setTexture(menuTexture);
	menuSprite.setPosition(0, 0);

	controlsSprite.setTexture(controlsTexture);
	controlsSprite.setPosition(0, 0);
	//napis PLAY w menu
	menu[0].setFont(font);
	menu[0].setFillColor(sf::Color::Red);
	menu[0].setCharacterSize(100);
	menu[0].setString("Play");
	menu[0].setPosition(200, height / 6 + 300);
	//napis STEROWANIE w menu
	menu[1].setFont(font);
	menu[1].setFillColor(sf::Color::White);
	menu[1].setCharacterSize(100);
	menu[1].setString("Sterowanie");
	menu[1].setPosition(200, height / 6 * 2 + 300);
	//napis EXIT w menu
	menu[2].setFont(font);
	menu[2].setFillColor(sf::Color::White);
	menu[2].setCharacterSize(100);
	menu[2].setString("Exit");
	menu[2].setPosition(200, height / 6 * 3 + 300);

	selectedItemIndex = 0;
}

void Menu::draw(sf::RenderWindow &window)
{
	window.draw(menuSprite);
	for (int i = 0; i < MAX_NUMBER_OF_ITEMS; i++)	//rysowanie wszystkich 3 linijek menu
	{
		window.draw(menu[i]);
	}
}
void Menu::drawControls(sf::RenderWindow &window)
{
	window.draw(controlsSprite);
}
//poruszanie sie w gore i w dol po menu
//wybrana opcja zmienia kolor na czerwony oraz delikatanie zwieksza czcionke
void Menu::moveUp()
{
	if (selectedItemIndex - 1 >= 0)
	{
		menu[selectedItemIndex].setFillColor(sf::Color::White);
		menu[selectedItemIndex].setCharacterSize(100);
		selectedItemIndex--;
		menu[selectedItemIndex].setFillColor(sf::Color::Red);
		menu[selectedItemIndex].setCharacterSize(110);
	}
}
void Menu::moveDown()
{
	if (selectedItemIndex + 1 < MAX_NUMBER_OF_ITEMS)
	{
		menu[selectedItemIndex].setFillColor(sf::Color::White);
		menu[selectedItemIndex].setCharacterSize(100);
		selectedItemIndex++;
		menu[selectedItemIndex].setFillColor(sf::Color::Red);
		menu[selectedItemIndex].setCharacterSize(110);
	}
}
