#include "QuadTree.h"

QuadTree::QuadTree(const sf::Vector2f& minSize, const sf::Vector2f& size, int maxDepth)
{
  myRoot.SetMinMax(minSize, minSize + size);
  myRoot.SetDepth(0);
  myRoot.CreateChildren(maxDepth);
}

QuadTree::~QuadTree()
{
}

void QuadTree::AddObject(Object* obj)
{
  myRoot.AddObject(obj);
}

void QuadTree::GetObjects(const sf::Vector2f& point, float rad, std::vector<Object*>& outVector)
{
  myRoot.GetObjectsInside(point, rad, outVector);
}

void QuadTree::ClearObjects()
{
  myRoot.Clean();
}

void QuadTree::Draw(sf::RenderWindow* wnd_p, int minDepth, int maxDepth)
{
  myRoot.Draw(wnd_p, minDepth, maxDepth);
}
