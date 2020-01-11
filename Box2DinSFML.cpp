#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <Box2D/Box2D.h>
#include "MyRectangle.h"
#include <iostream>
#include <sstream> // for ostringstream
#include <vector>
#include <stdlib.h>
#include <cstdlib>
#include <deque>

//Window variables
static int windowSizeX = 800, windowSizeY = 600;
static int windowBorderSize = 16;
static sf::RenderWindow window;
static std::vector<MyRectangle> windowBorders;
static sf::Music bgmusic;
static sf::SoundBuffer whack;
static sf::Sound playwhack;
static std::deque<sf::Sound> soundInstances;

//Box variables
static float boxSpawnTimer = 20.0f;
static float fixedTimeStep = 0.02f;
static std::vector<MyRectangle> boxList;

//World variables
static b2Vec2 worldgravity(0.f, 12.f);
static b2World world(worldgravity);

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
	sf::Vector2u size(windowSizeX, windowSizeY);
	window.create(sf::VideoMode(windowSizeX, windowSizeY), "SFML in Box2D");

	// Set vSync to true (and as a result, we have a cap of 60FPS)
	// Or you can use window.setFramerateLimit to cap 60FPS
	window.setFramerateLimit(60);
	window.setActive();

	// Border creation
	sf::Vector2f horizontalBorderSize(windowSizeX, windowBorderSize);
	sf::Vector2f verticalBorderSize(windowBorderSize, windowSizeY + 50);
	sf::Vector2f leftBorderPos(windowBorderSize / 2, windowSizeY / 2);
	sf::Vector2f rightBorderPos(windowSizeX - windowBorderSize / 2, windowSizeY / 2);
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
}

void LoadSfx()
{
	//Loading music
	//SFML can only play .wav/.ogg but not .mp3 file
	if (!bgmusic.openFromFile("SD.ogg"))
		std::printf("Music file not found!");
	else
	{
		bgmusic.play();
	}

	if (!whack.loadFromFile("whack.ogg"))
	{
		std::printf("Sound file not found!");
	}
}

// void Keyboard(sf::Keyboard key)
// {
// 	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
// 	{
// 		boxList[0].body_->ApplyLinearImpulse(b2Vec2(0,-150), boxList[0].body_->GetWorldCenter(), true);
// 	}
// }

void SpawnBoxes(std::vector<MyRectangle> &boxList, sf::RenderWindow &window, b2World &world)
{
	int boxtype = rand() % 5;
	sf::Transformable pos;

	if (boxtype == 0)
	{
		sf::Vector2f dynamicBoxSize(15.f, 15.f);
		pos.setPosition(static_cast<float>(windowBorderSize + (rand() % static_cast<int>(window.getSize().x - dynamicBoxSize.x - windowBorderSize))), -dynamicBoxSize.y);
		MyRectangle r(world, dynamicBoxSize, pos.getPosition());
		r.setOutlineThickness(1);
		r.setOutlineColor(sf::Color::Black);
		r.setFillColor(sf::Color::Magenta);
		r.body_->SetActive(true);

		boxList.push_back(r);
	}
	else if (boxtype == 1)
	{
		sf::Vector2f dynamicBoxSize(30.f, 30.f);
		pos.setPosition(static_cast<float>(windowBorderSize + (rand() % static_cast<int>(window.getSize().x - dynamicBoxSize.x - windowBorderSize))), -dynamicBoxSize.y);
		MyRectangle r(world, dynamicBoxSize, pos.getPosition());
		r.setOutlineThickness(1);
		r.setOutlineColor(sf::Color::Black);
		r.setFillColor(sf::Color::Blue);
		r.body_->SetActive(true);

		boxList.push_back(r);
	}
	else if (boxtype == 2)
	{
		sf::Vector2f dynamicBoxSize(50.f, 50.f);
		pos.setPosition(static_cast<float>(windowBorderSize + (rand() % static_cast<int>(window.getSize().x - dynamicBoxSize.x - windowBorderSize))), -dynamicBoxSize.y);
		MyRectangle r(world, dynamicBoxSize, pos.getPosition());
		r.setOutlineThickness(1);
		r.setOutlineColor(sf::Color::Black);
		r.setFillColor(sf::Color::Cyan);
		r.body_->SetActive(true);

		boxList.push_back(r);
	}
	else if (boxtype == 3)
	{
		sf::Vector2f dynamicBoxSize(70.f, 70.f);
		pos.setPosition(static_cast<float>(windowBorderSize + (rand() % static_cast<int>(window.getSize().x - dynamicBoxSize.x - windowBorderSize))), -dynamicBoxSize.y);
		MyRectangle r(world, dynamicBoxSize, pos.getPosition());
		r.setOutlineThickness(1);
		r.setOutlineColor(sf::Color::Black);
		r.setFillColor(sf::Color::Red);
		r.body_->SetActive(true);

		boxList.push_back(r);
	}
	else if (boxtype == 4)
	{
		sf::Vector2f dynamicBoxSize(100.0f, 100.f);
		pos.setPosition(static_cast<float>(windowBorderSize + (rand() % static_cast<int>(window.getSize().x - dynamicBoxSize.x - windowBorderSize))), -dynamicBoxSize.y);
		MyRectangle r(world, dynamicBoxSize, pos.getPosition());
		r.setOutlineThickness(1);
		r.setOutlineColor(sf::Color::Black);
		r.setFillColor(sf::Color::Green);
		r.body_->SetActive(true);

		boxList.push_back(r);
	}
}

int main()
{
	//Calling srand so that rand() gets a new value
	//Srand w/ time will generate a new seed for rand()
	std::srand(static_cast<unsigned>(time(NULL)));

	// Timer for fixed update
	sf::Clock fixedUpdateClock;
	float timeElapsedSinceLastFrame = 0;

	//Init the world
	Start();

	//Play Music
	LoadSfx();

	// Text creation
	sf::Font font = loadFont();
	sf::Text text;
	sf::Keyboard key;
	text.setFont(font);
	text.setCharacterSize(16);
	text.setPosition(3, -3);
	text.setColor(sf::Color::White);

	// A buffer to check whether left mouse button has been clicked before or not
	bool leftMousePressed = false;

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			// This is input handling via poll event
			// Do not use this for game input
			// Why? Delay issues
			// READ SFML DOCUMENTATION!
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
		}
		// We get the time elapsed since last frame and add it to timeElapsedSinceLastFrame
		timeElapsedSinceLastFrame += fixedUpdateClock.restart().asSeconds();


		//By right, beyond this point it should be in an Update() function lol

		//Number of boxes that are not clicked on
		//Implementing another vector where boxes are clicked upon crashes the program
		//Dirty hack?? to avoid declaring another vector
		int alivecount = 0;
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

		// If sufficient time has elapsed, we update the physics
		if (timeElapsedSinceLastFrame >= fixedTimeStep)
		{
			// Step is used to update physics position/rotation
			world.Step(fixedTimeStep, //update frequency
					   8,			  //velocityIterations
					   3			  //positionIterations
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
				if (boxList[i].body_->GetPosition().y > (0.03125 * windowSizeY))
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

								//Set to grey when clicked
								boxList[i].setFillColor(sf::Color(64, 64, 64));
								boxList[i].body_->ApplyLinearImpulse(
									b2Vec2((-3 + (rand() % 6)) * boxList[i].body_->GetMass(), -20 * boxList[i].body_->GetMass()),
									boxList[i].body_->GetWorldCenter(),
									true);
								boxList[i].body_->SetAngularVelocity(5.0f);

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

			// timeElapsedSinceLastFrame can be higher than fixedTimeStep,
			// so we deduct timeElapsedSinceLastFrame with fixedTimeStep
			timeElapsedSinceLastFrame -= fixedTimeStep;
		}

		//Deque sound effects that are done playing
		for (int i = 0; i < soundInstances.size(); ++i)
		{
			if (soundInstances[i].getStatus() == sf::Sound::Stopped)
			{
				soundInstances.erase(soundInstances.begin() + i);
				--i;
			}
		}

		// Rendering
		window.clear(sf::Color(100, 149, 237)); // CORNFLOWER BLUE!

		// Render all objects

		for (int i = 0; i < windowBorders.size(); i++)
		{
			window.draw(windowBorders[i].getShape());
		}

		for (int i = 0; i < boxList.size(); i++)
		{
			window.draw(boxList[i].getShape());
		}

		std::ostringstream boxListStream;
		boxListStream << boxList.size();
		text.setString("Number of boxes: " + boxListStream.str());
		window.draw(text);
		window.display();
	}

	return 0;
}
