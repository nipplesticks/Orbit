#include "Object.h"
#include "Structures.h"

Object::Object()
{
  myForce = sf::Vector2f(0.0f, 0.0f);
  myAcceleration = sf::Vector2f(0.0f, 0.0f);
  myVelocity = sf::Vector2f(0.0f, 0.0f);
}

Object::~Object()
{
}

void Object::ApplyForce(const sf::Vector2f& force)
{
  myForce = myForce + force;
}

void Object::SetMass(float mass)
{
  float r = std::max(sqrtf(mass / PI), 0.5f);
  myShape.setRadius(r);
  myShape.setOrigin(sf::Vector2f(r, r));
}

void Object::SetPosition(float x, float y)
{
  SetPosition(sf::Vector2f(x, y));
}

void Object::SetPosition(const sf::Vector2f& position)
{
  myShape.setPosition(position);
}

void Object::Update(float dt)
{
  _applyForces();
  _updatePosition(dt);
}

float Object::GetMass() const
{
  float r = myShape.getRadius();

  return PI * r * r;
}

float Object::GetRadius() const
{
  return myShape.getRadius();
}

const sf::Vector2f& Object::GetVelocity() const
{
  return myVelocity;
}

const sf::Vector2f& Object::GetAcceleration() const
{
  return myAcceleration;
}

const sf::Vector2f& Object::GetForce() const
{
  return myForce;
}

const sf::Vector2f& Object::GetPosition() const
{
  return myShape.getPosition();
}

void Object::Draw(sf::RenderWindow* window_p)
{
  window_p->draw(myShape);
}

void Object::_applyForces()
{
  //Apply forces here

  myForce = sf::Vector2f(0.0f, 0.0f);
}

void Object::_updatePosition(float dt)
{
}
