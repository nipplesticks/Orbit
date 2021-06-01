#pragma once
#include <SFML/Graphics.hpp>
#include <mutex>

class Object
{
private:
  static std::mutex mutex_flag;

public:
  static float MASS_SCALE;
  static sf::Vector2f GetForceBetween(const Object& a, const Object& b);
  static float CalculateMass(float r);
  static float CalculateIntersectingMass(const Object& a, const Object& b);

public:
  Object();
  ~Object();

  void ApplyForce(const sf::Vector2f& force);
  
  void SetMass(float mass);

  void SetPosition(float x, float y);
  void SetPosition(const sf::Vector2f& position);
  void SetVelocity(float x, float y);
  void SetVelocity(const sf::Vector2f& velocity);

  void Update(float dt);

  float GetMass() const;
  float GetRadius() const;
  const sf::Vector2f& GetVelocity() const;
  const sf::Vector2f& GetAcceleration() const;
  const sf::Vector2f& GetForce() const;
  const sf::Vector2f& GetPosition() const;
  bool Intersects(const Object& other);

  void Draw(sf::RenderWindow* window_p);

private:
  void _applyForces();
  void _updatePosition(float dt);

private:
  sf::CircleShape myShape;
  sf::Vector2f myVelocity;
  sf::Vector2f myAcceleration;
  sf::Vector2f myForce;
};