
#include <chrono>
#include <conio.h>
#include <easyx.h>
#include <geometry.h>
#include "physicalSolver.h"
#include <graphics.h>
#include <iostream>
#include <random>
#include <time.h>
#include <wingdi.h>

int main() {
  windowExtent=glm::vec2(1020,720);
  //glm::vec2 windowExtent = glm::vec2(720, 720.0f);
  initgraph(windowExtent.x, windowExtent.y);
  PhysicalSolver pSolver;
  while (true) {
    BeginBatchDraw();
    cleardevice();
    pSolver.addParticlePerFrame(20000);
    pSolver.solve();
    //solidcircle(500, 500, 3);
    FlushBatchDraw();
    ++PhysicalSolver::m_frameCount;
  }
  EndBatchDraw();
  return 0;
}
