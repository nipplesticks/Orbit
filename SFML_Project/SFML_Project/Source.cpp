#include <SFML/Graphics.hpp>
#include "Utility/Timer.h"
#include "Game.h"

int main()
{
  srand(time(0));
  sf::RenderWindow window(sf::VideoMode(1280, 1080), "SFML works!");
  sf::View v = window.getView();
  v.setSize(1280 * 2, 1080 * 2);
  //v.setCenter(v.getSize());
  window.setView(v);
  Game g(sf::Vector2f(1280, 1080));
  Timer dt;
  dt.Start();
  bool firstFrame = true;
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
    g.Tick((float)dt.Stop() * !firstFrame, &window);
    window.display();
    firstFrame = false;
  }
}