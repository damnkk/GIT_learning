#ifndef GEOMETRY_H
#define GEOMETRY_H
#include "glm/ext/vector_float2.hpp"
#include <easyx.h>
#include <glm/geometric.hpp>
#include <glm/vec2.hpp>
#include <windef.h>

extern glm::vec2 windowExtent;
class Particle{
public:
  Particle();
  Particle(glm::vec2 center, glm::vec2 speed):m_center(center),m_velocity(speed){}
  glm::vec2 m_center=glm::vec2(0.0f);
  const float m_radius=2.0f;
  glm::vec2 m_velocity = glm::vec2(0.0f);
  bool m_bounded = false;
  void draw();
};

#endif