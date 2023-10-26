#include "physicalSolver.h"

void PhysicalSolver::solve(){
    static auto startTime = std::chrono::high_resolution_clock::now();
    auto currentTime = std::chrono::high_resolution_clock::now();
    float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime-startTime).count()/200.0;
    for(int i = 0;i<m_objects.size();++i){
        Particle& particle = m_objects[i];
        auto &speed = particle.m_velocity;
        speed +=glm::vec2(0.0f,1.0f)*m_gravity*time;
        particle.m_center += speed*time;
        for(int b = 0;b<m_objects.size();++b){
            if(i==b||(!m_objects[i].m_bounded)||(!m_objects[b].m_bounded)) continue;
            float dis = distance(m_objects[i],m_objects[b]);
            if(dis>(m_objects[i].m_radius+m_objects[b].m_radius)) continue;
            else{
                glm::vec2 toLeft = glm::normalize(m_objects[i].m_center-m_objects[b].m_center);
                m_objects[i].m_center+= toLeft*(dis/2.0f);
                m_objects[b].m_center -=toLeft*(dis/2.0f);
            }
        }
        resolveCollisions(particle);
        particle.draw();
    }

}

uint64_t PhysicalSolver::m_frameCount = 0;

void PhysicalSolver::addParticle(uint32_t num){
    for(int i = 0;i<num;++i){
        m_objects.emplace_back(Particle(glm::vec2(0.0f,50.0f),glm::vec2(100.0f,0.0f)));
    }
}

void PhysicalSolver::addParticlePerFrame(uint64_t frameCount){
    if(m_objects.size()>frameCount||m_frameCount%3!=1){
        return ;
    }
    m_objects.emplace_back(Particle(glm::vec2(5.0f,50.0f),glm::vec2(100.0f,0.0f)));
    if(m_objects.size()>5000){
        m_objects.emplace_back(Particle(glm::vec2(5.0f,55.0f),glm::vec2(100.0f,0.0f)));
    }
    if(m_objects.size()>10000){
        m_objects.emplace_back(Particle(glm::vec2(5.0f,60.0f),glm::vec2(100.0f,0.0f)));
    }
    if(m_objects.size()>50000){
        m_objects.emplace_back(Particle(glm::vec2(5.0f,65.0f),glm::vec2(100.0f,0.0f)));
    }
    if(m_objects.size()>250000){
        m_objects.emplace_back(Particle(glm::vec2(5.0f,70.0f),glm::vec2(100.0f,0.0f)));
    }
}

void PhysicalSolver::resolveCollisions(Particle& particle){
    glm::vec2 halfBoundSize = windowExtent/2.0f -  particle.m_radius;
    if(abs((particle.m_center-(windowExtent/2.0f)).x)>halfBoundSize.x){
        particle.m_center.x = halfBoundSize.x*glm::sign((particle.m_center-(windowExtent/2.0f)).x)+(windowExtent/2.0f).x;
        particle.m_velocity.x *=(-1.0f)*m_boundLoss;
        particle.m_bounded = particle.m_bounded==false?true:true;
    }
    if(abs(particle.m_center-(windowExtent/2.0f)).y>halfBoundSize.y){
        particle.m_center.y = halfBoundSize.y*glm::sign((particle.m_center-(windowExtent/2.0f)).y)+(windowExtent/2.0f).y;
        particle.m_velocity.y *=(-1.0f)*m_boundLoss;
        particle.m_bounded = particle.m_bounded==false?true:true;
    }
}

float PhysicalSolver::distance(Particle& p1,Particle& p2){
    return glm::length(p1.m_center-p2.m_center);
}