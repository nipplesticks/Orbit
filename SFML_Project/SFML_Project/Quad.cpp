#include "Quad.h"

Quad::Quad()
{
  memset(myChildren, NULL, sizeof(myChildren));
  myDepth = 0;
  myIsLeaf = true;
  myMax = sf::Vector2f(FLT_MAX, FLT_MAX);
  myMin = sf::Vector2f(FLT_MIN, FLT_MIN);
}

Quad::~Quad()
{
  if (!myIsLeaf)
  {
    for (int i = 0; i < 4; i++)
    {
      delete myChildren[i];
      myChildren[i] = nullptr;
    }
  }
}

void Quad::SetDepth(int depth)
{
  myDepth = depth;
}

void Quad::SetMinMax(const sf::Vector2f& min, const sf::Vector2f& max)
{
  SetMin(min);
  SetMax(max);
}

void Quad::SetMin(const sf::Vector2f& min)
{
  myMin = min;
}

void Quad::SetMax(const sf::Vector2f& max)
{
  myMax = max;
}

void Quad::CreateChildren(int maxDepth)
{
  if (myDepth >= maxDepth)
    return;

  myIsLeaf = false;
  sf::Vector2f childSize = (myMax - myMin) * 0.5f;

  for (int y = 0; y < 2; y++)
  {
    for (int x = 0; x < 2; x++)
    {
      int idx = x + y * 2;
      myChildren[idx] = new Quad();
      myChildren[idx]->SetDepth(myDepth + 1);
      sf::Vector2f min(myMin.x + childSize.x * x, myMin.y + childSize.y * y);
      sf::Vector2f max = min + childSize;
      myChildren[idx]->SetMinMax(min, max);
      myChildren[idx]->CreateChildren(maxDepth);
    }
  }
}

void Quad::AddObject(Object* obj)
{
  if (!myIsLeaf)
  {
    for (int i = 0; i < 4; i++)
    {
      if (myChildren[i]->Intersects(obj->GetPosition(), obj->GetRadius()))
      {
        myChildren[i]->AddObject(obj);
      }
    }
  }
  else
  {
    myObjects.push_back(obj);
  }
}

void Quad::GetObjectsInside(const sf::Vector2f& point,
                            float rad,
                            std::vector<Object*>& outVector)
{
  if (!myIsLeaf)
  {
    for (int i = 0; i < 4; i++)
    {
      if (myChildren[i]->Intersects(point, rad))
      {
        myChildren[i]->GetObjectsInside(point, rad, outVector);
      }
    }
  }
  else
  {
    outVector.insert(outVector.end(), myObjects.begin(), myObjects.end());
  }
}

bool Quad::Intersects(const sf::Vector2f& center, float rad)
{
  sf::Vector2f point = center;

  if (point.x > myMax.x) point.x = myMax.x;
  if (point.x < myMin.x) point.x = myMin.x;
  if (point.y > myMax.y) point.x = myMax.y;
  if (point.y < myMin.y) point.x = myMin.y;

  sf::Vector2f dir = point - center;
  float distSqrd = dir.x * dir.x + dir.y * dir.y;

  return distSqrd < (rad * rad);
}

void Quad::Clean()
{
  if (!myIsLeaf)
  {
    for (int i = 0; i < 4; i++)
    {
      myChildren[i]->Clean();
    }
  }
  myObjects.clear();
}

int Quad::GetDepth() const
{
  return myDepth;
}

const sf::Vector2f& Quad::GetMin() const
{
  return myMin;
}

const sf::Vector2f& Quad::GetMax() const
{
  return myMax;
}

void Quad::Draw(sf::RenderWindow* wnd_p, int startDepth, int maxDepth)
{
  if (!myIsLeaf)
  {
    for (int i = 0; i < 4; i++)
      myChildren[i]->Draw(wnd_p, startDepth, maxDepth);
  }
  if (myDepth >= startDepth && myDepth <= maxDepth)
  {
    sf::RectangleShape rs;
    rs.setPosition(myMin);
    rs.setSize(myMax - myMin);
    rs.setFillColor(sf::Color::Transparent);
    rs.setOutlineThickness(-1.0f);
    rs.setOutlineColor(sf::Color::Magenta);
    wnd_p->draw(rs);
  }
}
