#include "glm/ext/vector_float2.hpp"
#include <geometry.h>

glm::vec2 windowExtent;

Circle::Circle() {
  m_center = glm::vec2(0.0f, 0.0f);
  m_radius = 0.0f;
}

Circle::Circle(glm::vec2 center, float radius)
    : m_center(center), m_radius(radius) {
  m_pos = m_center;
}

void Circle::draw() {
  circle(m_center.x * windowExtent.x, m_center.y * windowExtent.y, m_radius);
}

void Circle::update(glm::vec2 pos) {
  m_center = pos;
  m_pos = m_center;
}

Point::Point() { m_pos = glm::vec2(0.0f); }

Point::Point(glm::vec2 pos) { m_pos = pos; }

void Point::draw() {
  putpixel(m_pos.x * windowExtent.x, m_pos.y * windowExtent.y, BLUE);
}

void Point::update(glm::vec2 pos) { m_pos = pos; }

ArcTangle::ArcTangle() {
  m_leftUp = glm::vec2(0.0f);
  m_rightDown = glm::vec2(0.0f);
  m_pos = glm::vec2(0.0f);
}

ArcTangle::ArcTangle(glm::vec2 leftup, glm::vec2 rightDown) {
  m_leftUp = leftup;
  m_rightDown = rightDown;
  m_pos = (m_leftUp + m_rightDown) / 2.0f;
}

void ArcTangle::draw() {
  rectangle(m_leftUp.x * windowExtent.x, m_leftUp.y * windowExtent.y,
            m_rightDown.x * windowExtent.x, m_rightDown.y * windowExtent.y);
}

void ArcTangle::update(glm::vec2 pos) {
  glm::vec2 moveVec = pos - m_pos;
  m_leftUp += moveVec;
  m_rightDown = moveVec;
  m_pos = pos;
}

Basket::Basket() {}

Basket::Basket(float height, float side) {
  glm::vec2 boardPos = glm::vec2(0.0f);
  if (side < 0.5f) {
    boardPos = glm::vec2(0.05f, height);
  } else
    boardPos = glm::vec2(1.0f - 0.05f, height);
  glm::vec2 boardParam = glm::vec2(-0.01f, 0.2f);
  m_board = ArcTangle(boardPos + boardParam, boardPos - boardParam);
  if (boardPos.x < 0.5) {
    m_inside = glm::vec2(boardPos.x - 5 * boardParam.x, boardPos.y + 0.1f);
    m_outside =
        glm::vec2(m_inside.m_pos.x + 0.7 * boardParam.y, m_inside.m_pos.y);
  } else {
    m_inside = glm::vec2(boardPos.x + 5 * boardParam.x, boardPos.y + 0.1f);
    m_outside =
        glm::vec2(m_inside.m_pos.x - 0.7 * boardParam.y, m_inside.m_pos.y);
  }
}

void Basket::draw() {
  m_board.draw();
  line(m_outside.m_pos.x * windowExtent.x, m_outside.m_pos.y * windowExtent.y,
       m_inside.m_pos.x * windowExtent.x, m_inside.m_pos.y * windowExtent.y);
}

Ball::Ball() {
  m_ball = Circle(glm::vec2(0.2f, 0.1f), 30);
  m_speed = glm::vec2(900.0f, 0.0f);
  m_gravity = 1.8f;
  m_decrease = 0.99999f;
}

// 这里总结了,
// 球体的反弹边界情况,首先位置符合反弹要求,此外,当前运动方向应该是入射才行,
// 弹到最后需要手动停球,手动停球的条件是在计算过反弹之后,方向依然是入射,且还在反弹判定区域内,这说明速度太慢了,上一次反弹没弹出去,这次又开始转换成错误方向了.
void Ball::update(float time) {
  // 首先speed没有归一化
  glm::vec2 moveValue = time * m_speed;
  // 因此算出来的moveValue需要归一化
  moveValue.x /= windowExtent.x;
  moveValue.y /= windowExtent.y;
  // 所以球的位置更新了

  m_ball.m_center += moveValue;

  if (m_ball.m_center.y > 0.95 && m_speed.y > 0) {
    m_speed = glm::reflect(m_speed, glm::vec2(0.0f, -1.0f));
    m_speed *= 0.85f;
  }
  if (m_ball.m_center.x > 1.0f) {
    m_ball.m_center.x = 0.0f;
  }
  m_speed += glm::vec2(0.0f, 1.0f) * m_gravity;
  m_speed *= m_decrease;
  if (m_speed.y > 0.0f && m_ball.m_center.y > 0.95)
    m_speed.y = 0.0f;
}
void Ball::draw() { m_ball.draw(); }