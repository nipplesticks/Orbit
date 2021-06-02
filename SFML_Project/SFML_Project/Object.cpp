#include "Object.h"
#include "Structures.h"

std::mutex Object::mutex_flag;
float Object::MASS_SCALE = 1.0f;
size_t Object::trace_res = 10;

sf::Vector2f Object::GetForceBetween(const Object& a, const Object& b)
{
  sf::Vector2f dir = b.GetPosition() - a.GetPosition();
  float dist = dir.x * dir.x + dir.y * dir.y;
  if (fabs(dist) < EPSILON) return sf::Vector2f(0.0f, 0.0f);
  float f = (b.GetMass() * a.GetMass()) / (dist / Object::MASS_SCALE);
  sf::Vector2f n = dir * (1.0f / sqrtf(dist));
  return n * f;
}

float Object::CalculateMass(float r)
{
  return PI * r * r;
}

float Object::CalculateIntersectingMass(const Object& a, const Object& b)
{
  float r1 = a.GetRadius();
  float r2 = b.GetRadius();

  sf::Vector2f p1 = a.GetPosition();
  sf::Vector2f p2 = b.GetPosition();
  sf::Vector2f dir = p1 - p2;

  float dist = sqrt(dir.x * dir.x + dir.y * dir.y);

  if (dist > r1 + r2) return 0.0f;
  if (dist <= fabs(r1 - r2)) return std::min(a.GetMass(), b.GetMass());

  float distCenter1 = (std::pow(r1, 2) - std::pow(r2, 2) + std::pow(dist, 2)) / (2 * dist);
  float distCenter2 = dist - distCenter1;
  float height = std::sqrt(std::pow(r1, 2) - std::pow(distCenter1, 2));

  float alpha = std::fmod(std::atan2(height, distCenter1) * 2.0 + 2 * PI, 2 * PI);
  float beta = std::fmod(std::atan2(height, distCenter2) * 2.0 + 2 * PI, 2 * PI);
  float A0 = std::pow(r1, 2) / 2.0 * (alpha - std::sin(alpha));
  float A1 = std::pow(r2, 2) / 2.0 * (beta - std::sin(beta));

  return A0 + A1;

}

Object::Object()
{
  myForce = sf::Vector2f(0.0f, 0.0f);
  myAcceleration = sf::Vector2f(0.0f, 0.0f);
  myVelocity = sf::Vector2f(0.0f, 0.0f);
  myTraceIdx = 0;
  memset(myTraceCount, 0, sizeof(myTraceCount));
  memset(myTrace, 0, sizeof(myTrace));
  myColor.r = rand() % 206 + 50;
  myColor.g = rand() % 206 + 50;
  myColor.b = rand() % 206 + 50;
  myColor.a = 255;
  myShape.setFillColor(myColor);
}

Object::~Object()
{
}

void Object::ApplyForce(const sf::Vector2f& force)
{
  Object::mutex_flag.lock();
  myForce = myForce + force;
  Object::mutex_flag.unlock();
}

void Object::SetMass(float mass)
{
  float r = std::max(sqrtf(mass / PI), 0.0f);
  myShape.setRadius(r);
  myShape.setOrigin(sf::Vector2f(r, r));
}

void Object::SetPosition(float x, float y)
{
  SetPosition(sf::Vector2f(x, y));
}

void Object::SetPosition(const sf::Vector2f& position)
{
  myShape.setPosition(position);
}

void Object::SetVelocity(float x, float y)
{
  SetVelocity(sf::Vector2f(x, y));
}

void Object::SetVelocity(const sf::Vector2f& velocity)
{
  myVelocity = velocity;
}

void Object::Update(float dt)
{
  _applyForces();
  _updatePosition(dt);

  sf::Vertex v;
  v.position = GetPosition();
  v.color = myColor;
  v.texCoords = sf::Vector2f(0.0f, 0.0f);

  if (myFrameCounter++ % Object::trace_res == 0)
  {
    if (myTraceCount[myTraceIdx] >= Object::MAX_TRACE_SIZE)
      myTraceIdx = (myTraceIdx + 1) % 2;
    myTrace[myTraceIdx][myTraceCount[myTraceIdx]++] = v;
    int idx = (myTraceIdx + 1) % 2;
    myTraceCount[idx] = myTraceCount[idx] - 1 >= 0 ? myTraceCount[idx] - 1 : 0;
  }
}

float Object::GetMass() const
{
  float r = myShape.getRadius();

  return PI * r * r;
}

float Object::GetRadius() const
{
  return myShape.getRadius();
}

const sf::Vector2f& Object::GetVelocity() const
{
  return myVelocity;
}

const sf::Vector2f& Object::GetAcceleration() const
{
  return myAcceleration;
}

const sf::Vector2f& Object::GetForce() const
{
  return myForce;
}

const sf::Vector2f& Object::GetPosition() const
{
  return myShape.getPosition();
}

bool Object::Intersects(const Object& other)
{
  float r1 = GetRadius();
  float r2 = other.GetRadius();

  sf::Vector2f p1 = GetPosition();
  sf::Vector2f p2 = other.GetPosition();
  sf::Vector2f dir = p1 - p2;

  float dist = sqrt(dir.x * dir.x + dir.y * dir.y);
  
  return (r1 + r2) > dist;
}

void Object::Draw(sf::RenderWindow* window_p)
{
  window_p->draw(myShape);
  if (GetRadius() > 1.0f)
  {
    int idx = (myTraceIdx + 1) % 2;
    if (myTraceCount[myTraceIdx] > 0)
      window_p->draw(myTrace[myTraceIdx], myTraceCount[myTraceIdx], sf::PrimitiveType::LineStrip);
    if (myTraceCount[idx])
      window_p->draw(myTrace[idx] + Object::MAX_TRACE_SIZE - myTraceCount[idx], myTraceCount[idx], sf::PrimitiveType::LineStrip);
  }
}

void Object::_applyForces()
{
  Object::mutex_flag.lock();
  float mass = GetMass();
  myAcceleration.x = myForce.x / mass;
  myAcceleration.y = myForce.y / mass;
  myForce = sf::Vector2f(0.0f, 0.0f);
  Object::mutex_flag.unlock();
}

void Object::_updatePosition(float dt)
{
  SetPosition(GetPosition() + myVelocity * dt + myAcceleration * dt * dt);
  myVelocity = myVelocity + myAcceleration * dt;
}
