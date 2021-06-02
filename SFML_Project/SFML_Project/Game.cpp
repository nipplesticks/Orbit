#include "Game.h"
#include <iostream>
#include "Structures.h"
#include "HelpFunctions.h"

Timer Global::globalGameTime;

const float MAX_DIST_FROM_SUN = 10000.f;


Game::Game(const sf::Vector2f& winSize) :
  myQuadTree(sf::Vector2f(0.0f,0.0f), winSize * 20.f, 7)
{
  Global::globalGameTime.Start();
  myTexture.loadFromFile("Assets/Untitled.png");
  myTexture.setRepeated(true);
  myTexture.generateMipmap();
  myTexture.setSmooth(true);
  myRect.setTexture(&myTexture);
  myMinDepth = 0;
  myMaxDepth = 0;
  Object::MASS_SCALE = 2000.0f;

  Object obj1;
  obj1.SetMass(1000.0f);
  obj1.SetPosition(winSize.x * 0.5f, winSize.y * 0.5f);
  myObjects.push_back(obj1);
  myStar_p = &myObjects.back();
  myQuadTree.AddObject(&myObjects.back());

  for (int i = 0; i < 50; i++)
  {
    bool set = false;
    while (!set)
    {
      Object obj;
      set = true;
      obj.SetPosition(rand() % ((int)winSize.x * 2) ,rand() % ((int)winSize.y * 2));
      obj.SetPosition(obj.GetPosition() - myStar_p->GetPosition());
      obj.SetMass(rand() % 1000 + 300);

      std::vector<Object*> arr;
      myQuadTree.GetObjects(obj.GetPosition(), obj.GetRadius(), arr);

      if (!arr.empty())
      {
        for (auto& c : arr)
        {
          if (obj.Intersects(*c))
          {
            set = false;
            break;
          }
        }
      }

      if (!set)
        continue;

      int d = rand();
      int s = rand() % 500 + 50;

      obj.SetVelocity(std::sin(d) * (float)s, std::cos(d) * (float)s);
      myObjects.push_back(obj);
      myQuadTree.AddObject(&myObjects.back());
    }
  }
}

Game::~Game()
{
}

void Game::Tick(float dt, sf::RenderWindow* wnd_p)
{
  wnd_p->draw(myRect);
  myQuadTree.ClearObjects();
  if (myStar_p)
    myQuadTree.SetPosition(myStar_p->GetPosition());
  for (auto & obj : myObjects)
  {
    myQuadTree.AddObject(&obj);
  }

  // Move to thread
  for (auto& a : myObjects)
    for (auto& b : myObjects)
      if (&a != &b)
        a.ApplyForce(Object::GetForceBetween(a, b));

  std::vector<std::list<Object>::iterator> removeVector;

  std::list<Object>::iterator idx = myObjects.begin();

  auto view = wnd_p->getView();
  myStar_p = nullptr;

  std::list<Object> newObjects;
  for (auto& obj : myObjects)
  {
    obj.Update(dt);

    if (!myStar_p || myStar_p->GetMass() < obj.GetMass())
      myStar_p = &obj;

    obj.Draw(wnd_p);

    std::vector<Object*> col;
    myQuadTree.GetObjects(obj.GetPosition(), obj.GetRadius(), col);
    _handleCollisions(obj, col, newObjects);

    if (obj.GetRadius() < 1.0f ||
        Help::GetDistance(view.getCenter(), obj.GetPosition()) > MAX_DIST_FROM_SUN)
    {
      removeVector.push_back(idx);
    }
    idx++;
  }

  if (myStar_p)
  {
    view.setCenter(myStar_p->GetPosition());
    std::cout << "\r object count: " << myObjects.size() << " star mass: " << myStar_p->GetMass();
  }

  static bool wasMLeftPressed = false;
  static bool wasMRightPressed = false;

  bool isMLeftPressed = sf::Mouse::isButtonPressed(sf::Mouse::Left);
  bool isMRightPressed = sf::Mouse::isButtonPressed(sf::Mouse::Right);

  if (!wasMLeftPressed && isMLeftPressed)
    view.setSize(view.getSize() * 0.5f);
  if (!wasMRightPressed && isMRightPressed)
    view.setSize(view.getSize() * 2.0f);

  wasMLeftPressed = isMLeftPressed;
  wasMRightPressed = isMRightPressed;


  wnd_p->setView(view);


  for (auto& a : removeVector)
  {
    myObjects.erase(a);
  }
  myObjects.insert(myObjects.end(), newObjects.begin(), newObjects.end());

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

void Game::_handleCollisions(Object& obj, std::vector<Object*>& col, std::list<Object>& newObjects)
{
  for (auto& c : col)
  {
    if (c == &obj)
      continue;

    float lol = std::sin(rand());

    if (obj.Intersects(*c))
    {
      float r1 = obj.GetRadius();
      float r2 = c->GetRadius();
      float m = Object::CalculateIntersectingMass(obj, *c);

      int n = rand() % 10;
      float m2 = m / 10.0f;

      for (int i = 0; i < n; i++)
      {
        m -= m2;
        Object o;
        o.SetMass(m2);
        if (o.GetRadius() < 1.0f)
          continue;
        o.SetPosition(r1 > r2 ? c->GetPosition() : obj.GetPosition());
        float rValue = (float)rand();
        o.SetVelocity(std::sin(rValue) * (rand() % 250 + 50), std::cos(rValue) * (rand() % 250 + 50));
        newObjects.push_back(o);
      }

      if (r1 > r2)
      {
        obj.SetMass(obj.GetMass() + m);
        c->SetMass(c->GetMass() - m);
      }
      else
      {
        obj.SetMass(obj.GetMass() - m);
        c->SetMass(c->GetMass() + m);
      }
    }
  }
}
