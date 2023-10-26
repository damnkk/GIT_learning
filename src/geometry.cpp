#include "glm/ext/vector_float2.hpp"
#include <geometry.h>

glm::vec2 windowExtent = glm::vec2(0.0f);

Particle::Particle(){}

void Particle::draw(){
  solidcircle(m_center.x, m_center.y, m_radius);
}