#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <Box2D/Box2D.h>
#include "MyRectangle.h"
#include <iostream>
#include <sstream>		// for ostringstream
#include <vector>
#include <stdlib.h>
#include <cstdlib>
#include <deque>

// Define some constants
static const int gameWidth = 800;
static const int gameHeight = 600;
static const int gameBorderSize = 16;

// Create gravity and world, then assign gravity to world
static b2Vec2 gravity(0.f, 12.0f);
static b2World world(gravity);
static float timer;
static int points;

// A vector/list of boxes
static std::vector<MyRectangle> boxList;
static float boxSpawnTimer = 20.0f;
static float fixedTimeStep = 0.02f;

//Window
static sf::RenderWindow window;

//Border 
static std::vector<MyRectangle> windowBorders;

// Music
static sf::Music bgMusic;
static sf::SoundBuffer whack;
static sf::Sound playwhack;
static std::deque<sf::Sound> soundInstances;

sf::Font loadFont(const std::string &fontFilename = "resources/04b03.ttf")
{
	sf::Font myFont;
	if (!myFont.loadFromFile(fontFilename))
	{
		std::cout << "Can not load font from " << fontFilename << std::endl;
		exit(1);
	}
	return myFont;
}

void Start()
{
	// Create the window of the application
	window.create(sf::VideoMode(gameWidth, gameHeight), "SFML in Box2D");
	window.setVerticalSyncEnabled(true);
	window.setFramerateLimit(60);
	window.setActive();
}

void Border()
{
	// Border Creation
	sf::Vector2f horizontalBorderSize(gameWidth, gameBorderSize);
	sf::Vector2f verticalBorderSize(gameBorderSize, gameHeight - gameBorderSize * 2);
	sf::Vector2f leftBorderPos(gameBorderSize / 2, gameHeight / 2);
	sf::Vector2f rightBorderPos(gameWidth - gameBorderSize / 2, gameHeight / 2);

	MyRectangle leftBorder(world, verticalBorderSize, leftBorderPos, 0.0, false);
	leftBorder.setOutlineThickness(-1);
	leftBorder.setOutlineColor(sf::Color::Black);
	leftBorder.setFillColor(sf::Color(100, 100, 100));

	MyRectangle rightBorder(world, verticalBorderSize, rightBorderPos, 0.0, false);
	rightBorder.setOutlineThickness(-1);
	rightBorder.setOutlineColor(sf::Color::Black);
	rightBorder.setFillColor(sf::Color(100, 100, 100));

	windowBorders.push_back(leftBorder);
	windowBorders.push_back(rightBorder);

	timer = 12;
	points = 0;
}

void loadSfx()
{
	//Loading music
	//SFML can only play .wav/.ogg but not .mp3 file
	if (!bgMusic.openFromFile("SD.ogg"))
		std::printf("Music file not found!");
	else
	{
		bgMusic.setPlayingOffset(sf::Time(sf::seconds(0.4f)));
		bgMusic.setLoop(true);
		bgMusic.play();
	}

	if (!whack.loadFromFile("whack.ogg"))
	{
		std::printf("Sound file not found!");
	}
}

void SpawnBoxes(std::vector<MyRectangle> &boxList, sf::RenderWindow &window, b2World &world)
{
	int boxtype = rand() % 5;
	sf::Transformable pos;

	if (boxtype == 0)
	{
		sf::Vector2f dynamicBoxSize(15.f, 15.f);
		pos.setPosition(static_cast<float>(
			gameBorderSize + (rand() % static_cast<int>(window.getSize().x - dynamicBoxSize.x - gameBorderSize))),
			gameHeight + dynamicBoxSize.y);
		MyRectangle r(world, dynamicBoxSize, pos.getPosition());
		r.setOutlineThickness(1);
		r.setOutlineColor(sf::Color::Black);
		r.setFillColor(sf::Color::Magenta);
		r.body_->SetActive(true);
		r.body_->ApplyLinearImpulse(
			b2Vec2((-3 + (rand() % 6)) * r.body_->GetMass(), -20 * r.body_->GetMass()),
			r.body_->GetWorldCenter(),
			true);
		boxList.push_back(r);
	}
	else if (boxtype == 1)
	{
		sf::Vector2f dynamicBoxSize(30.f, 30.f);
		pos.setPosition(
			static_cast<float>(gameBorderSize + (rand() % static_cast<int>(window.getSize().x - dynamicBoxSize.x - gameBorderSize))),
			gameHeight + dynamicBoxSize.y);
		MyRectangle r(world, dynamicBoxSize, pos.getPosition());
		r.setOutlineThickness(1);
		r.setOutlineColor(sf::Color::Black);
		r.setFillColor(sf::Color::Blue);
		r.body_->SetActive(true);
		r.body_->ApplyLinearImpulse(
			b2Vec2((-3 + (rand() % 6)) * r.body_->GetMass(), -20 * r.body_->GetMass()),
			r.body_->GetWorldCenter(),
			true);
		boxList.push_back(r);
	}
	else if (boxtype == 2)
	{
		sf::Vector2f dynamicBoxSize(50.f, 50.f);
		pos.setPosition(static_cast<float>(
			gameBorderSize + (rand() % static_cast<int>(window.getSize().x - dynamicBoxSize.x - gameBorderSize))),
			gameHeight + dynamicBoxSize.y);
		MyRectangle r(world, dynamicBoxSize, pos.getPosition());
		r.setOutlineThickness(1);
		r.setOutlineColor(sf::Color::Black);
		r.setFillColor(sf::Color::Cyan);
		r.body_->SetActive(true);
		r.body_->ApplyLinearImpulse(
			b2Vec2((-3 + (rand() % 6)) * r.body_->GetMass(), -20 * r.body_->GetMass()),
			r.body_->GetWorldCenter(),
			true);
		boxList.push_back(r);
	}
	else if (boxtype == 3)
	{
		sf::Vector2f dynamicBoxSize(70.f, 70.f);
		pos.setPosition(static_cast<float>(
			gameBorderSize + (rand() % static_cast<int>(window.getSize().x - dynamicBoxSize.x - gameBorderSize))),
			gameHeight + dynamicBoxSize.y);
		MyRectangle r(world, dynamicBoxSize, pos.getPosition());
		r.setOutlineThickness(1);
		r.setOutlineColor(sf::Color::Black);
		r.setFillColor(sf::Color::Red);
		r.body_->SetActive(true);
		r.body_->ApplyLinearImpulse(
			b2Vec2((-3 + (rand() % 6)) * r.body_->GetMass(), -20 * r.body_->GetMass()),
			r.body_->GetWorldCenter(),
			true);
		boxList.push_back(r);
	}
	else if (boxtype == 4)
	{
		sf::Vector2f dynamicBoxSize(100.0f, 100.f);
		pos.setPosition(static_cast<float>(
			gameBorderSize + (rand() % static_cast<int>(window.getSize().x - dynamicBoxSize.x - gameBorderSize))),
			gameHeight + dynamicBoxSize.y);
		MyRectangle r(world, dynamicBoxSize, pos.getPosition());
		r.setOutlineThickness(1);
		r.setOutlineColor(sf::Color::Black);
		r.setFillColor(sf::Color::Green);
		r.body_->SetActive(true);
		r.body_->ApplyLinearImpulse(
			b2Vec2((-3 + (rand() % 6)) * r.body_->GetMass(), -20 * r.body_->GetMass()),
			r.body_->GetWorldCenter(),
			true);
		boxList.push_back(r);
	}
}

int main()
{
	//Calling srand so that rand() gets a new value
	//Srand w/ time will generate a new seed for rand()
	std::srand(static_cast<unsigned>(time(NULL)));

	// Timer for fixed update
	sf::Clock fixedUpdateClock, timerclock, flashingclock;
	sf::Time elapsedtime = fixedUpdateClock.getElapsedTime();

	float timeElapsedSinceLastFrame = 0;
	float timeelapsed = 0;
	float flashingtimeelapsed = 0;

	//Init the world
	Start();
	Border();

	// Load SFX
	loadSfx();

	// Load the text font
	sf::Font fontSensation = loadFont("resources/sansation.ttf");
	sf::Font fontArial = loadFont("resources/arial.ttf");
	sf::Font font04b03 = loadFont("resources/04b03.ttf");

	// Initialize the title and instruction message
	sf::Text titleText, instructionMessage, pointstext, timertext, gameOverText, result;
	titleText.setFont(fontSensation);
	instructionMessage.setFont(fontSensation);
	pointstext.setFont(fontArial);
	timertext.setFont(font04b03);
	gameOverText.setFont(font04b03);

	titleText.setCharacterSize(80);
	titleText.setFillColor(sf::Color::White);
	titleText.setString("GAME TITLE");
	titleText.setStyle(sf::Text::Bold);

	instructionMessage.setCharacterSize(20);
	instructionMessage.setFillColor(sf::Color::White);
	instructionMessage.setString("Press space to start the game");
	instructionMessage.setStyle(sf::Text::Bold);

	result.setCharacterSize(76);
	result.setColor(sf::Color::White);

	timertext.setCharacterSize(150);
	timertext.setColor(sf::Color(255, 255, 255, 80));
	timertext.setPosition(sf::Vector2f(gameWidth / 2.0f -64.0f, gameHeight / 2.0f - 108.0f));

	gameOverText.setCharacterSize(80);
	gameOverText.setColor(sf::Color(255, 0, 0));
	gameOverText.setString("GAME OVER");
	gameOverText.setStyle(sf::Text::Bold);

	// Center text
	sf::FloatRect textRect = titleText.getLocalBounds();
	titleText.setOrigin(textRect.left + textRect.width / 2.0f,
		textRect.top + textRect.height / 2.0f);
	titleText.setPosition(sf::Vector2f(gameWidth / 2.0f, gameHeight / 2.0f));

	textRect = instructionMessage.getLocalBounds();
	instructionMessage.setOrigin(textRect.left + textRect.width / 2.0f,
		textRect.top + textRect.height / 2.0f);
	instructionMessage.setPosition(sf::Vector2f(gameWidth / 2.0f, (gameHeight / 2.0f) + 60.0f));

	textRect = gameOverText.getLocalBounds();
	gameOverText.setOrigin(textRect.left + textRect.width / 2.0f,
		textRect.top + textRect.height / 2.0f);
	gameOverText.setPosition(sf::Vector2f(gameWidth / 2.0f, gameHeight / 2.0f - 120.0f));

	bool isPlaying = false;
	bool gameOver = false;

	// A buffer to check whether left mouse button has been clicked before or not
	bool leftMousePressed = false;

	while (window.isOpen())
	{
		// Handle events
		sf::Event event;
		while (window.pollEvent(event))
		{
			// Window closed or escape key pressed: exit
			if ((event.type == sf::Event::Closed) ||
				((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape)))
			{
				window.close();
				break;
			}

			// Space key pressed: play
			if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Space))
			{
				if (!isPlaying)
				{
					// Start the game
					isPlaying = true;
				} 
			}
		}

		// We get the time elapsed since last frame and add it to timeElapsedSinceLastFrame
		timeElapsedSinceLastFrame += fixedUpdateClock.restart().asSeconds();

		/*
		* Number of boxes that are not clicked on
		* Implementing another vector where boxes are clicked upon crashes the program
		* Dirty hack?? to avoid declaring another vector
		*/
		int alivecount = 0;

		if (timer >= 0)
		{
			for (int i = 0; i < boxList.size(); i++)
			{
				if (boxList[i].dead == false)
				{
					alivecount++;
				}
			}

			if (alivecount < 8)
			{
				if (boxSpawnTimer >= 20.f)
				{
					SpawnBoxes(boxList, window, world);
					boxSpawnTimer = 0;
				}
				else
				{
					boxSpawnTimer += 1.0f;
				}
			}
		}
		else
		{
			boxList.clear();
		}

		// If sufficient time has elapsed, we update the physics
		if (timeElapsedSinceLastFrame >= fixedTimeStep)
		{
			// In game movement
			if (isPlaying)
			{

				// Step is used to update physics position/rotation
				world.Step(fixedTimeStep, //update frequency
					8,                //velocityIterations
					3                 //positionIterations  
				);

				// Update the objects that uses physics
				for (int i = 0; i < windowBorders.size(); i++)
				{
					windowBorders[i].update();
				}

				for (int i = 0; i < boxList.size(); i++)
				{
					boxList[i].update();

					/*IMPORTANT!!
					* body_ position != window size!!
					* Height of body_ @ 25 = Height of window: 800
					* Ratio of 0.03125 = 1 pixel
					* I think there's other ways to check if box is outside window
					*/
					if (boxList[i].body_->GetPosition().y > (0.03125 * gameHeight) + boxList[i].rect_.getSize().y)
					{
						boxList[i].body_->SetActive(false);
						world.DestroyBody(boxList[i].body_);
						boxList.erase(boxList.begin() + i);
					}

					if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
					{
						if (!leftMousePressed)
						{
							sf::Vector2f pos = sf::Vector2f(sf::Mouse::getPosition(window));
							for (int i = 0; i < boxList.size(); i++)
							{
								if (boxList[i].getShape().getGlobalBounds().contains(pos) && !boxList[i].dead)
								{
									//soundInstances is a deque that allows the program to play overlapping sound effects
									//Calling sf::Sound.play() without a deque would cancel the currently playing sound effect
									soundInstances.push_back(sf::Sound(whack));
									soundInstances.back().setPlayingOffset(sf::seconds(0.6f));
									soundInstances.back().play();

									if (boxList[i].rect_.getFillColor() == sf::Color::Magenta)
									{
										points += 10;
									}
									else if (boxList[i].rect_.getFillColor() == sf::Color::Blue)
									{
										points += 7;
									}
									else if (boxList[i].rect_.getFillColor() == sf::Color::Cyan)
									{
										points += 5;
									}
									else if (boxList[i].rect_.getFillColor() == sf::Color::Red)
									{
										points += 3;
									}
									else if (boxList[i].rect_.getFillColor() == sf::Color::Green)
									{
										points += 1;
									}

									//Set to grey and dead when clicked
									boxList[i].setFillColor(sf::Color(64, 64, 64));
									boxList[i].body_->ApplyLinearImpulse(
										b2Vec2((-3 + (rand() % 6)) * boxList[i].body_->GetMass(),
											-5 * boxList[i].body_->GetMass()),
										boxList[i].body_->GetWorldCenter(),
										true);
									boxList[i].body_->SetAngularVelocity(10.0f);

									//Disables collision of box using b2Filter catergorybits and maskbits
									boxList[i].disableCollision();
									boxList[i].dead = true;
								}
							}
							leftMousePressed = true;
						}
						else
						{
							leftMousePressed = false;
						}
					}
				}
			}

			// timeElapsedSinceLastFrame can be higher than fixedTimeStep,
			// so we deduct timeElapsedSinceLastFrame with fixedTimeStep
			timeElapsedSinceLastFrame -= fixedTimeStep;
		}

		// Clear the window
		window.clear(sf::Color(100, 149, 237));		//CORNFLOWER BLUE

		if (isPlaying)
		{
			// Draw the game properties

			//Deque sound effects that are done playing
			for (int i = 0; i < soundInstances.size(); ++i)
			{
				if (soundInstances[i].getStatus() == sf::Sound::Stopped)
				{
					soundInstances.erase(soundInstances.begin() + i);
					--i;
				}
			}

			// Render the outline box
			for (int i = 0; i < windowBorders.size(); i++)
			{
				window.draw(windowBorders[i].getShape());
			}

			for (int i = 0; i < boxList.size(); i++)
			{
				window.draw(boxList[i].getShape());
			}

			sf::Time deltat = timerclock.restart();
			timeelapsed += deltat.asSeconds();
			if (timeelapsed >= 1.0f)
			{
				timer--;
				timeelapsed = 0.f;
			}

			std::ostringstream pointsStream;
			std::ostringstream timerStream;
			timerStream << timer;
			if (timer >= 0)
			{
				if (timer < 10)
				{
					timertext.setPosition(window.getSize().x / 2 - 32.0f, window.getSize().y / 2 - 108);
				}
				if (timer < 5) {
					timertext.setColor(sf::Color(255, 0, 0, 100));
					sf::Time flashdt = flashingclock.restart();
					flashingtimeelapsed += flashdt.asSeconds();
					int alpha = timertext.getFillColor().a * flashingtimeelapsed;
					if (flashingtimeelapsed / (timer + 0.1) > 0.17f) {
						timertext.setColor(sf::Color(255, 0, 0, alpha));
						flashingtimeelapsed = 0;
						alpha = 100;
					}
					else if (flashingtimeelapsed / (timer + 0.1) < 0.17f) {
						timertext.setColor(sf::Color(255, 0, 0, alpha));
						alpha = 80;
					}
				}

				if (timer <= 0) {
					timertext.setColor(sf::Color(255, 0, 0, 100));
					gameOver = true;
				}


				timertext.setString(timerStream.str());
				timerclock.restart();
			}
			pointsStream << points;
			pointstext.setCharacterSize(16);
			pointstext.setColor(sf::Color::White);
			pointstext.setPosition(24, 0);
			pointstext.setString("Points: " + pointsStream.str());

			window.draw(timertext);
			window.draw(pointstext);

			if (gameOver == true)
			{
				//gameOverText.setPosition(window.getSize().x / 2 - 108, window.getSize().y / 2 - 108);
				window.draw(gameOverText);
			}
		}
		else {
			// Draw the pause message
			window.draw(titleText);
			window.draw(instructionMessage);
		}

		// Displays things on screen
		window.display();
	}

	return EXIT_SUCCESS;
}