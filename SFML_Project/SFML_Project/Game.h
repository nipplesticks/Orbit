#pragma once
#include "QuadTree.h"
#include "Object.h"
#include <list>
#include <thread>

class Game
{
public:
  Game(const sf::Vector2f& winSize);
  ~Game();

  void Tick(float dt, sf::RenderWindow* wnd_p);

private:
  void _handleCollisions(Object& obj, std::vector<Object*>& col, std::list<Object>& newObjects);

private:
  QuadTree myQuadTree;
  std::list<Object> myObjects;
  int myMinDepth;
  int myMaxDepth;
  std::thread myForceThread;
  std::mutex myObjectsMutex;
  Object* myStar_p;
  sf::Texture myTexture;
  sf::RectangleShape myRect;
};