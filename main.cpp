#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include "MyRectangle.h"
#include <iostream>
#include <sstream>		// for ostringstream
#include <vector>

int main()
{
	// Define some constants
	const int gameWidth = 800;
	const int gameHeight = 600;
	const int gameBorderSize = 16;
	// A buffer to check whether left mouse button has been clicked before or not
	bool leftMousePressed = false;

	// Create the window of the application
	sf::RenderWindow window(sf::VideoMode(gameWidth, gameHeight), "SFML Box2D Test");
	window.setVerticalSyncEnabled(true);
	window.setActive();

	// Create gravity and world, then assign gravity to world
	b2Vec2 gravity(0.f, 9.81f);
	b2World world(gravity);

	// Border Creation
	sf::Vector2f horizontalBorderSize(gameWidth, gameBorderSize);
	sf::Vector2f verticalBorderSize(gameBorderSize, gameHeight-gameBorderSize*2);
	sf::Vector2f topBorderPos(gameWidth/2,gameBorderSize/2);
	sf::Vector2f bottomBorderPos(gameWidth/2, gameHeight-gameBorderSize/2);
	sf::Vector2f leftBorderPos(gameBorderSize/2, gameHeight/2);
	sf::Vector2f rightBorderPos(gameWidth-gameBorderSize/2, gameHeight/2);

	MyRectangle topBorder(world, horizontalBorderSize, topBorderPos, 0.0, false);
	topBorder.setOutlineThickness(-1);
	topBorder.setOutlineColor(sf::Color::Black);
	topBorder.setFillColor(sf::Color(100, 100, 100));

	MyRectangle bottomBorder(world, horizontalBorderSize, bottomBorderPos, 0.0, false);
	bottomBorder.setOutlineThickness(-1);
	bottomBorder.setOutlineColor(sf::Color::Black);
	bottomBorder.setFillColor(sf::Color(100, 100, 100));

	MyRectangle leftBorder(world, verticalBorderSize, leftBorderPos, 0.0, false);
	leftBorder.setOutlineThickness(-1);
	leftBorder.setOutlineColor(sf::Color::Black);
	leftBorder.setFillColor(sf::Color(100, 100, 100));

	MyRectangle rightBorder(world, verticalBorderSize, rightBorderPos, 0.0, false);
	rightBorder.setOutlineThickness(-1);
	rightBorder.setOutlineColor(sf::Color::Black);
	rightBorder.setFillColor(sf::Color(100, 100, 100));

	// A vector/list of boxes
	std::vector<MyRectangle> boxList;
	sf::Vector2f dynamicBoxSize(32, 32);

	// A floor
	MyRectangle floor(world, sf::Vector2f(270,10), sf::Vector2f(270,270), 30.0f, false);
	floor.setFillColor(sf::Color::Red);

	// Load the text font
	sf::Font fontSensation;
	sf::Font fontArial;
	if (!fontSensation.loadFromFile("resources/sansation.ttf"))
		return EXIT_FAILURE;

	if (!fontArial.loadFromFile("resources/arial.ttf"))
		return EXIT_FAILURE;

	// Initialize the title and instruction message
	sf::Text titleText, instructionMessage, scoreText, lifesLeft;
	titleText.setFont(fontSensation);
	instructionMessage.setFont(fontSensation);
	scoreText.setFont(fontSensation);
	lifesLeft.setFont(fontSensation);

	titleText.setCharacterSize(80);
	titleText.setFillColor(sf::Color::White);
	titleText.setString("GAME TITLE");
	titleText.setStyle(sf::Text::Bold);

	instructionMessage.setCharacterSize(24);
	instructionMessage.setFillColor(sf::Color::White);
	instructionMessage.setString("Press space to start the game");
	instructionMessage.setStyle(sf::Text::Bold);

	scoreText.setCharacterSize(20);
	scoreText.setFillColor(sf::Color::White);
	scoreText.setPosition(20,15);

	lifesLeft.setCharacterSize(20);
	lifesLeft.setFillColor(sf::Color::White);
	lifesLeft.setPosition(20,40);

	// Center text
	sf::FloatRect textRect = titleText.getLocalBounds();
	titleText.setOrigin(textRect.left + textRect.width / 2.0f,
		textRect.top + textRect.height / 2.0f);
	titleText.setPosition(sf::Vector2f(gameWidth / 2.0f, gameHeight / 2.0f));

	textRect = instructionMessage.getLocalBounds();
	instructionMessage.setOrigin(textRect.left + textRect.width / 2.0f,
		textRect.top + textRect.height / 2.0f);
	instructionMessage.setPosition(sf::Vector2f(gameWidth / 2.0f, (gameHeight / 2.0f) + 60.0f));

	// Timer for fixed update
	float fixedTimeStep = 0.02f; // 50 times per second
	sf::Clock fixedUpdateClock;
	float timeElapsedSinceLastFrame = 0;

	bool isPlaying = false;
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
					// Restart the game
					isPlaying = true;
				}
			}
		}

		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			if (!leftMousePressed)
			{
				sf::Vector2f pos = sf::Vector2f(sf::Mouse::getPosition(window));
				MyRectangle r(world, dynamicBoxSize, pos);
				r.setOutlineThickness(1);
				r.setOutlineColor(sf::Color::Black);
				r.setFillColor(sf::Color(100, 100, 200));
				boxList.push_back(r);
				leftMousePressed = true;
			}
		}
		else
		{
			leftMousePressed = false;
		}

		// We get the time elapsed since last frame and add it to timeElapsedSinceLastFrame
		timeElapsedSinceLastFrame += fixedUpdateClock.restart().asSeconds();

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
				topBorder.update();
				bottomBorder.update();
				leftBorder.update();
				rightBorder.update();
				floor.update();

				for (int i = 0; i < boxList.size(); i++)
				{
					boxList[i].update();
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

			// Render the outline box
			window.draw(topBorder.getShape());
			window.draw(bottomBorder.getShape());
			window.draw(leftBorder.getShape());
			window.draw(rightBorder.getShape());
			window.draw(floor.getShape());

			for (int i = 0; i < boxList.size(); i++)
			{
				window.draw(boxList[i].getShape());
			}

			std::ostringstream boxListStream, lifeCount;
			boxListStream << boxList.size();
			lifeCount << 3;
			scoreText.setString("Score: " + boxListStream.str());
			lifesLeft.setString("Lives left: " + lifeCount.str());
			window.draw(scoreText);
			window.draw(lifesLeft);
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