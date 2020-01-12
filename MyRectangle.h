#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>

class MyRectangle
{
 private:
	// sf::RectangleShape rect_;
	b2BodyDef bodyDef_;
	// b2Body* body_;
	b2PolygonShape bodyShape_;
	// b2FixtureDef bodyFixtureDef_;

 public:
 	sf::RectangleShape rect_;
	b2Body* body_;
	b2FixtureDef bodyFixtureDef_;
	bool dead = false;
	MyRectangle(b2World& world,
               sf::Vector2f size,
					sf::Vector2f position,
					float rotation = 0,
               bool isDynamic = true
              );
	void setFillColor(sf::Color col);
	void setOutlineThickness(float thickness);
	void setOutlineColor(sf::Color col);
	void update();
	void disableCollision();
	sf::Shape& getShape();
	// b2Body getBodyDef();
	b2Body setBodyDef(b2Body otherbody);
};
