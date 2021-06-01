#include "Object.h"
#include "Structures.h"

std::mutex Object::mutex_flag;
float Object::MASS_SCALE = 1.0f;

sf::Vector2f Object::GetForceBetween(const Object& a, const Object& b)
{
  sf::Vector2f dir = b.GetPosition() - a.GetPosition();
  float dist = dir.x * dir.x + dir.y * dir.y;
  if (fabs(dist) < EPSILON) return sf::Vector2f(0.0f, 0.0f);
  float f = (b.GetMass() * a.GetMass()) / (dist / Object::MASS_SCALE);
  sf::Vector2f n = dir * (1.0f / sqrtf(dist));
  return n * f;
}

float Object::CalculateMass(float r)
{
  return PI * r * r;
}

float Object::CalculateIntersectingMass(const Object& a, const Object& b)
{
  float r1 = a.GetRadius();
  float r2 = b.GetRadius();

  sf::Vector2f p1 = a.GetPosition();
  sf::Vector2f p2 = b.GetPosition();
  sf::Vector2f dir = p1 - p2;

  float dist = sqrt(dir.x * dir.x + dir.y * dir.y);

  if (dist > r1 + r2) return 0.0f;
  if (dist <= fabs(r1 - r2)) return std::min(a.GetMass(), b.GetMass());

  float distCenter1 = (std::pow(r1, 2) - std::pow(r2, 2) + std::pow(dist, 2)) / (2 * dist);
  float distCenter2 = dist - distCenter1;
  float height = std::sqrt(std::pow(r1, 2) - std::pow(distCenter1, 2));

  float alpha = std::fmod(std::atan2(height, distCenter1) * 2.0 + 2 * PI, 2 * PI);
  float beta = std::fmod(std::atan2(height, distCenter2) * 2.0 + 2 * PI, 2 * PI);
  float A0 = std::pow(r1, 2) / 2.0 * (alpha - std::sin(alpha));
  float A1 = std::pow(r2, 2) / 2.0 * (beta - std::sin(beta));

  return A0 + A1;

}

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
  Object::mutex_flag.lock();
  myForce = myForce + force;
  Object::mutex_flag.unlock();
}

void Object::SetMass(float mass)
{
  float r = std::max(sqrtf(mass / PI), 0.0f);
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

void Object::SetVelocity(float x, float y)
{
  SetVelocity(sf::Vector2f(x, y));
}

void Object::SetVelocity(const sf::Vector2f& velocity)
{
  myVelocity = velocity;
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

bool Object::Intersects(const Object& other)
{
  float r1 = GetRadius();
  float r2 = other.GetRadius();

  sf::Vector2f p1 = GetPosition();
  sf::Vector2f p2 = other.GetPosition();
  sf::Vector2f dir = p1 - p2;

  float dist = sqrt(dir.x * dir.x + dir.y * dir.y);
  
  return (r1 + r2) > dist;
}

void Object::Draw(sf::RenderWindow* window_p)
{
  window_p->draw(myShape);
}

void Object::_applyForces()
{
  Object::mutex_flag.lock();
  float mass = GetMass();
  myAcceleration.x = myForce.x / mass;
  myAcceleration.y = myForce.y / mass;
  myForce = sf::Vector2f(0.0f, 0.0f);
  Object::mutex_flag.unlock();
}

void Object::_updatePosition(float dt)
{
  SetPosition(GetPosition() + myVelocity * dt + myAcceleration * dt * dt);
  myVelocity = myVelocity + myAcceleration * dt;
}
