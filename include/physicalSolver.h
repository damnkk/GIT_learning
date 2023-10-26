#ifndef PHYSICALSOLVER_H
#define PHYSICALSOLVER_H

#include "geometry.h"
#include <vector>
#include <chrono>

class PhysicalSolver{
public:
    std::vector<Particle> m_objects;
    float m_gravity = 9.8f;
    float m_boundLoss = 0.75f;
    void solve();
    void addParticle(uint32_t num);
    void addParticlePerFrame(uint64_t frameCount);
    void resolveCollisions(Particle& particle);
    void subSolve(Particle& p1,Particle& p2);
    float distance(Particle& p1,Particle& p2);
    static uint64_t m_frameCount;
};

#endif