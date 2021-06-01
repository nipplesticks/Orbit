#include "Game.h"
#include <iostream>

Game::Game(const sf::Vector2f& winSize) :
  myQuadTree(sf::Vector2f(0.0f, 0.0f), winSize)
{
  myMinDepth = 0;
  myMaxDepth = 0;
  
  Object obj;
  obj.SetMass(3652);
  obj.SetPosition(365, 365);
  myObjects.push_back(obj);

  obj.SetMass(4523);
  obj.SetPosition(winSize.x - 365, winSize.y - 365);
  myObjects.push_back(obj);

}

Game::~Game()
{
}

void Game::Tick(float dt, sf::RenderWindow* wnd_p)
{
  myQuadTree.ClearObjects();
  for (auto & obj : myObjects)
  {
    myQuadTree.AddObject(&obj);
  }

  for (auto& obj : myObjects)
  {
    obj.Draw(wnd_p);
  }

  static bool wasSpacePressed = false;
  static bool drawQT = false;
  bool isSpacePressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Space);
  
  if (!wasSpacePressed && isSpacePressed)
    drawQT = !drawQT;

  wasSpacePressed = isSpacePressed;

  if (drawQT)
  {
    static bool wasLeftPressed = false;
    static bool wasRightPressed = false;
    static bool wasUpPressed = false;
    static bool wasDownPressed = false;

    bool isLeftPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Left);
    bool isRightPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Right);
    bool isUpPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Up);
    bool isDownPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Down);

    if (!wasLeftPressed && isLeftPressed) myMinDepth--;
    if (!wasRightPressed && isRightPressed) myMinDepth++;
    if (!wasUpPressed && isUpPressed) myMaxDepth++;
    if (!wasDownPressed && isDownPressed) myMaxDepth--;

    wasLeftPressed = isLeftPressed;
    wasRightPressed = isRightPressed;
    wasUpPressed = isUpPressed;
    wasDownPressed = isDownPressed;


    myQuadTree.Draw(wnd_p, myMinDepth, myMaxDepth);
  }
}
