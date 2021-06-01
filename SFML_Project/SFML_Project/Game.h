#pragma once
#include "QuadTree.h"
#include "Object.h"
#include <list>

class Game
{
public:
  Game(const sf::Vector2f& winSize);
  ~Game();

  void Tick(float dt, sf::RenderWindow* wnd_p);

private:
  QuadTree myQuadTree;
  std::list<Object> myObjects;
  int myMinDepth;
  int myMaxDepth;

};