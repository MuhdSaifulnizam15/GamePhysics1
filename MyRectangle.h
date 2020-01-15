/********************************************************************
Course: TGD2251 Game Physics
Session: Trimester 2, 2019/20
ID and Name #1: 1171300077 Lim Kuang Tar
Contacts	#1: 0164230329 1171300077@student.mmu.edu.my
ID and Name #2: 1171302090 Muhammad Saifulnizam bin Mohd Samsul Sukri
Contacts	#2: 0182446467 1171302090@student.mmu.edu.my
*********************************************************************/

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
