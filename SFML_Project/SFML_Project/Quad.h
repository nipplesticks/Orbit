#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>
#include <vector>
#include "Object.h"


class Quad
{
public:
  Quad();
  ~Quad();

  void SetDepth(int depth);
  void SetMinMax(const sf::Vector2f& min, const sf::Vector2f& max);
  void SetMin(const sf::Vector2f& min);
  void SetMax(const sf::Vector2f& max);
  void CreateChildren(int maxDepth);
  void AddObject(Object* obj, const sf::Vector2f& offset);
  void GetObjectsInside(const sf::Vector2f& point,
                        float rad,
                        std::vector<Object*>& outVector,
                        const sf::Vector2f& offset);

  bool Intersects(const sf::Vector2f& center, float rad, const sf::Vector2f& offset);

  void Clean();

  int GetDepth() const;
  const sf::Vector2f& GetMin() const;
  const sf::Vector2f& GetMax() const;
  void Draw(sf::RenderWindow* wnd_p, int startDepth, int maxDepth,
            const sf::Vector2f& offset);

private:
  int myDepth;
  bool myIsLeaf;
  sf::Vector2f myMin;
  sf::Vector2f myMax;
  Quad* myChildren[4];
  std::vector<Object*> myObjects;
};