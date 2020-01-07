#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>

#include <iostream>
#include <sstream>		// for ostringstream
#include <vector>

int main()
{
	// Define some constants
	const int gameWidth = 800;
	const int gameHeight = 600;

	// Create the window of the application
	sf::RenderWindow window(sf::VideoMode(gameWidth, gameHeight, 32), "SFML Box2D Test",
		sf::Style::Titlebar | sf::Style::Close);
	window.setVerticalSyncEnabled(true);

	// Load the text font
	sf::Font fontSensation;
	sf::Font fontArial;
	if (!fontSensation.loadFromFile("resources/sansation.ttf"))
		return EXIT_FAILURE;

	if (!fontArial.loadFromFile("resources/arial.ttf"))
		return EXIT_FAILURE;

	// Initialize the pause message
	sf::Text pauseMessage;
	pauseMessage.setFont(fontSensation);
	pauseMessage.setCharacterSize(40);
	pauseMessage.setFillColor(sf::Color::White);
	pauseMessage.setString("Welcome to SFML Box2D Game Title!\nPress space to start the game");
	pauseMessage.setStyle(sf::Text::Bold);

	// Center text
	sf::FloatRect textRect = pauseMessage.getLocalBounds();
	pauseMessage.setOrigin(textRect.left + textRect.width / 2.0f,
		textRect.top + textRect.height / 2.0f);
	pauseMessage.setPosition(sf::Vector2f(gameWidth / 2.0f, gameHeight / 2.0f));

	sf::Clock clock;
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
					clock.restart();
				}
			}
		}

		// In game movement
		if (isPlaying)
		{

		}

		// Clear the window
		window.clear(sf::Color(50, 200, 50));

		if (isPlaying)
		{
			// Draw the game properties
		}
		else {
			// Draw the pause message
			window.draw(pauseMessage);
		}

		// Displaye things on screen
		window.display();
	}

	return EXIT_SUCCESS;
}