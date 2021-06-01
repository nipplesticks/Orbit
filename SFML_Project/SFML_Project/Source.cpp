#include <SFML/Graphics.hpp>
#include "Utility/Timer.h"
#include "Game.h"

int main()
{
  sf::RenderWindow window(sf::VideoMode(1280, 720), "SFML works!");
  Game g(sf::Vector2f(1280.0f, 720.0f));
  Timer dt;
  dt.Start();

  while (window.isOpen())
  {
    sf::Event e;
    while (window.pollEvent(e))
    {
      if (e.type == sf::Event::Closed)
      {
        window.close();
      }
    }

    window.clear();
    g.Tick(dt.Stop(), &window);
    window.display();
  }
}