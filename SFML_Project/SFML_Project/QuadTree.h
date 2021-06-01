#pragma once
#include "Quad.h"

class QuadTree
{
public:
  QuadTree(const sf::Vector2f& minSize, const sf::Vector2f& size, int maxDepth = 5);
  ~QuadTree();

  void AddObject(Object* obj);
  void GetObjects(const sf::Vector2f& point, float rad, std::vector<Object*>& outVector);
  void ClearObjects();
  void Draw(sf::RenderWindow* wnd_p, int minDepth, int maxDepth);

private:
  Quad myRoot;

};