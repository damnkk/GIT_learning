#include <__msvc_chrono.hpp>
#include <chrono>
#include <conio.h>
#include <easyx.h>
#include <geometry.h>
#include <graphics.h>
#include <iostream>
#include <random>
#include <time.h>
#include <wingdi.h>

int main() {
  windowExtent = glm::vec2(640, 480.0f);
  initgraph(windowExtent.x, windowExtent.y);

  Basket bs(0.6, 0.2);
  Ball ball;
  decltype(std::chrono::system_clock::now()) start, end;
  start = std::chrono::system_clock::now();
  while (true) {
    BeginBatchDraw();
    cleardevice();

    // 绘制篮筐
    bs.draw();

    // 设置篮球颜色
    setlinecolor(RED);
    ball.draw();
    setlinecolor(WHITE);
    clearcircle(ball.m_ball.m_center.x, ball.m_ball.m_center.y,
                ball.m_ball.m_radius);
    auto end = std::chrono::system_clock::now();
    auto duration =
        std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    double time = double(duration.count()) *
                  std::chrono::microseconds::period::num /
                  std::chrono::microseconds::period::den;
    ball.update(time);
    // if (c.m_center.x > 1.0f) {
    //   c.m_center.x = 0;
    // } else if (c.m_center.y > 1.0f)
    //   c.m_center.y = 0;

    // Sleep(1);
    FlushBatchDraw();
    start = std::chrono::system_clock::now();
  }
  EndBatchDraw();
  return 0;
}
