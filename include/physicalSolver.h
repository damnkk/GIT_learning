#ifndef PHYSICALSOLVER_H
#define PHYSICALSOLVER_H

#include "geometry.h"
#include <vector>
#include <chrono>
#include "threadPool.hpp"
#include <accelerateGrid.h>
#include <iostream>

class PhysicalSolver{
public:
    void solve(float dt);
    void addParticle(glm::vec2 pos);
    void resolveCollisions(Particle& particle);
    void solveCollisions();
    void updateObjects_multi(float dt);
    float distance(Particle& p1,Particle& p2);
    void addParticleToGrid();
    void solveContace(Particle& p1,Particle& p2);
    void solveGrid(glm::ivec2 gridIndex1,glm::ivec2 gridIndex2);
    void solveCollisionThreaded(uint32_t i, uint32_t slice_size);


    std::vector<Particle>                       m_objects;
    ThreadPool                                  m_threadPool;
    float                                       m_gravity = 20.8f;
    float                                       m_boundLoss = 0.75f;
    static uint64_t                             m_frameCount;
    uint32_t                                    m_subSteps=9;
    AccelerateGrid                              m_accelerateGrid;
};

#endif