#include "QuadTree.h"

QuadTree::QuadTree(const sf::Vector2f& minSize, const sf::Vector2f& size, int maxDepth)
{
  myPosition = sf::Vector2f(0.0f, 0.0f);
  myRoot.SetMinMax(minSize, minSize + size);
  myRoot.SetDepth(0);
  myRoot.CreateChildren(maxDepth);
}

QuadTree::~QuadTree()
{
}

void QuadTree::SetPosition(const sf::Vector2f& position)
{
  myPosition = position + (myRoot.GetMin() - myRoot.GetMax()) * 0.5f;
}

void QuadTree::AddObject(Object* obj)
{
  myRoot.AddObject(obj, myPosition);
}

void QuadTree::GetObjects(const sf::Vector2f& point, float rad, std::vector<Object*>& outVector)
{
  myRoot.GetObjectsInside(point, rad, outVector, myPosition);
}

void QuadTree::ClearObjects()
{
  myRoot.Clean();
}

void QuadTree::Draw(sf::RenderWindow* wnd_p, int minDepth, int maxDepth)
{
  myRoot.Draw(wnd_p, minDepth, maxDepth, myPosition);
}
