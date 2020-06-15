#ifndef header_h
#define header_h

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <vector>
#include <sstream>
#include <cstdlib>


extern float speed;		//zmienna odpowiedzialna za przyspieszanie gry
extern bool gamePause;	//zmienna okreslajaca czy zostal wcisniety przycisk pauzy
extern bool gameOver;	//zmienna sprawdzajaca czy gra dalej trwa
extern bool gamePlay;	//zmienna sprawdzaj¹ca czy jesteœmy w menu
extern bool gameControls;	//zmienna sprawdzaj¹ca czy jesteœmy w zak³adce sterowanie
extern int score;			//punkty
extern bool restartGame;	//zmienna okreslajaca czy zostal wcisniety przycisk restartu gry

#define MAX_NUMBER_OF_ITEMS 3	//liczba pozycji w menu

class Obstacle;

class Player {

	friend void collisionDetection(const Player*, const Obstacle*);

	sf::Texture playerTexture;
	sf::Sprite playerSprite;
	float gravity = 1.2;
	int imageCount = 0;
	int row = 0;

public:

	float y = 800;
	float velocity = 0;

	Player();
	Player(std::string fileName);

	sf::Clock clockAnimation;	//zegar potrzebny do stworzenia animacji
	void drawPlayer(sf::RenderWindow &window);
	void updateMovement();

};

class Background
{
	sf::Texture backgroundTexture;
	sf::Sprite backgroundSprite;

public:
	float X = 0;
	Background();
	Background(std::string fileName);

	sf::Clock clock;
	void updateBackground(float &speed);
	void drawBackground(sf::RenderWindow &window);
};

class Scoreboard
{
	sf::Font font;
	sf::Text textScore;
	sf::Text gameOver;
	sf::Text yourScore;
	sf::Text playAgain;

public:
	sf::Text numbers;
	Scoreboard();
	Scoreboard(std::string fileName);

	void convertToString(int score);
	void drawScore(sf::RenderWindow &window);
	void drawGameOver(sf::RenderWindow &window);
};

class Obstacle
{
	sf::Texture obstacleTexture;
	sf::Clock clock;

	friend void collisionDetection(const Player*, const Obstacle*);
	friend void updateObstacle(Obstacle &przeszkoda1);
	friend void updateObstacle2(Obstacle &przeszkoda1);

public:
	float X=0;
	sf::Sprite obstacleSprite;

	Obstacle();
	Obstacle(std::string fileName);
	void drawObstacle(sf::RenderWindow &window);
};

class Menu
{
	sf::Texture menuTexture;	//MENU GLOWNE
	sf::Sprite menuSprite;

	sf::Texture controlsTexture;	//MENU ZAKLADKA STEROWANIE
	sf::Sprite controlsSprite;

public:
	int selectedItemIndex;	//wybrana linijka w menu

	Menu(std::string fileName, std::string fileName2, float width, float height);
	//funkcje rysujace menu oraz zakladke sterowanie
	void draw(sf::RenderWindow &window);
	void drawControls(sf::RenderWindow &window);
	//funkcje poruszania sie po menu w gore i w dol
	void moveUp();
	void moveDown();

private:

	sf::Font font;
	sf::Text menu[MAX_NUMBER_OF_ITEMS];

};

void collisionDetection(const Player* gracz, const Obstacle* przeszkoda);
void updateObstacle(Obstacle &przeszkoda1);
void updateObstacle2(Obstacle &przeszkoda1);
void playAgain(Player &gracz, Obstacle &przeszkoda1, Obstacle &przeszkoda2, Scoreboard &tab);

#endif //header_h