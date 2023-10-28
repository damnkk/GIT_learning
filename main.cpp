
#include <chrono>
#include <conio.h>
#include <easyx.h>
#include <geometry.h>
#include "physicalSolver.h"
#include <graphics.h>
#include "threadPool.hpp"
#include <iostream>
#include <random>
#include <time.h>
#include <wingdi.h>

int main() {
   windowExtent=glm::vec2(300);
   //glm::vec2 windowExtent = glm::vec2(720, 720.0f);
   initgraph(windowExtent.x, windowExtent.y);
   PhysicalSolver pSolver;
   pSolver.m_accelerateGrid.init(windowExtent, 1);
   pSolver.m_threadPool.setPoolSize(30);
   setbkcolor(BLACK);
   constexpr uint32_t fps_cap = 60;
   const float dt = 1.0f / static_cast<float>(fps_cap);
   while (true) {
     BeginBatchDraw();
     cleardevice();
     if (pSolver.m_objects.size() < 20000) {
       for(uint32_t i{30};i--;){
         pSolver.addParticle({2.0f,50.0f+1.1f*i});
         pSolver.m_objects.back().m_lastPosition.x -=0.4;
       }
       //pSolver.addParticlePerFrame(100);
     }
     pSolver.solve(dt);
     //solidcircle(500, 500, 3);
     FlushBatchDraw();
     ++PhysicalSolver::m_frameCount;
   }
   EndBatchDraw();
   return 0;
}
