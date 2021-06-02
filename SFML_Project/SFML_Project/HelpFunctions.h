#pragma once
#include <SFML/System/Vector2.hpp>
#include <math.h>
namespace Help
{
  float GetDistance(const sf::Vector2f& a, const sf::Vector2f& b)
  {
    sf::Vector2f dir = b - a;
    return sqrtf(dir.x * dir.x + dir.y * dir.y);
  }

}