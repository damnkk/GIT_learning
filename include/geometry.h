#include "glm/ext/vector_float2.hpp"
#include <easyx.h>
#include <glm/geometric.hpp>
#include <glm/vec2.hpp>
#include <windef.h>


extern glm::vec2 windowExtent;

class geometry {
public:
  glm::vec2 m_pos;
  COLORREF m_color;
  virtual void draw() = 0;
  virtual void update(glm::vec2 pos) = 0;
};

class Circle : public geometry {
public:
  Circle();
  Circle(glm::vec2 center, float radius);
  glm::vec2 m_center;
  float m_radius;
  virtual void draw() override;
  virtual void update(glm::vec2 pos) override;
};

class Point : public geometry {
public:
  Point();
  Point(glm::vec2 pos);
  virtual void draw() override;
  virtual void update(glm::vec2 pos) override;
};

class ArcTangle : public geometry {
public:
  ArcTangle();
  ArcTangle(glm::vec2 leftUp, glm::vec2 rightDown);
  glm::vec2 m_leftUp;
  glm::vec2 m_rightDown;
  virtual void draw() override;
  virtual void update(glm::vec2 pos) override;
};

class Basket {
public:
  Basket();
  Basket(float height, float side);
  ArcTangle m_board;
  Point m_inside, m_outside;
  void draw();
};

/*
如何去抽象一个球体的运动过程,在Ball类当中设置一个二维向量作为速度,设置一个浮点数作为重力,每帧对速度向量乘以一个小于0的数作为阻力衰减,
因为场景中物体比较少,每帧遍历篮板,篮筐两点和地面横线这四个物体做碰撞判定,碰撞不会减少任何速度,只会改变速度方向
1.先计算速度
+(后期鼠标点击事件 还要对速度做调整)
2.再计算碰撞

update 函数,传入上一帧到当前的时间,根据时间,算出重力向下增加量
*/

class Ball {
public:
  Ball();
  void update(float time);
  void draw();
  Circle m_ball;
  glm::vec2 m_speed;
  float m_gravity;
  float m_decrease;
};