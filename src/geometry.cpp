#include "glm/ext/vector_float2.hpp"
#include <geometry.h>

glm::vec2 windowExtent = glm::vec2(0.0f);

Particle::Particle(){}

void Particle::draw(){
  solidcircle(m_center.x, m_center.y, m_radius);
}

void Particle::update(float dt){
  const glm::vec2 last_update_position = m_center -m_lastPosition;
  const glm::vec2 new_position = m_center + last_update_position+(m_velocity-last_update_position*40.0f)*(dt*dt);
  m_lastPosition = m_center;
  m_center = new_position;
  m_velocity = {0.0f,0.0f};
}