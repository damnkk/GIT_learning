#include "physicalSolver.h"

void PhysicalSolver::solve(float dt){

    const float sub_dt = dt/static_cast<float>(m_subSteps);
    for(int i = 0;i<m_subSteps;++i){
        addParticleToGrid();
        solveCollisions();
        updateObjects_multi(sub_dt);
    }

    const uint32_t threadCount = m_threadPool.m_threads.size();
    const uint32_t perThread = m_objects.size() / threadCount;
    const uint32_t save = m_objects.size() % threadCount;
    const uint32_t particlePatch = m_objects.size() / threadCount;
    uint32_t currIndex = 0;
    for (int i = 0; i < m_threadPool.m_threads.size(); ++i) {
        uint32_t realParticlePatch = particlePatch;
        if (i < save) realParticlePatch += 1;
        else if (particlePatch == 0) break;
        m_threadPool.m_threads[i]->addJob([this, currIndex, realParticlePatch, dt]() {
            for (int k = currIndex; k < currIndex + realParticlePatch; ++k) {
                Particle& particle = m_objects[k];
                particle.draw();
            }
            });
        currIndex += realParticlePatch;
    }
    m_threadPool.wait();
   
}

uint64_t PhysicalSolver::m_frameCount = 0;

void PhysicalSolver::addParticle(glm::vec2 pos){
    m_objects.emplace_back(Particle(pos));
}

void PhysicalSolver::updateObjects_multi(float dt){
    const uint32_t threadCount = m_threadPool.m_threads.size();
    const uint32_t perThread = m_objects.size() / threadCount;
    const uint32_t save = m_objects.size() % threadCount;
    const uint32_t particlePatch = m_objects.size() / threadCount;
    uint32_t currIndex = 0;
    for (int i = 0; i < m_threadPool.m_threads.size(); ++i) {
        uint32_t realParticlePatch = particlePatch;
        if (i < save) realParticlePatch += 1;
        else if (particlePatch == 0) break;
        m_threadPool.m_threads[i]->addJob([this,currIndex, realParticlePatch,dt]() {
            for (int k =  currIndex; k <  currIndex+realParticlePatch; ++k) {
                Particle& particle = m_objects[k];
                particle.m_velocity += glm::vec2(0.0f, m_gravity);
                particle.update(dt);
                resolveCollisions(particle);
            }
            });
        currIndex += realParticlePatch;
    }
    m_threadPool.wait();
     
    //for(int i = 0;i<m_objects.size();++i){
    //    Particle& particle = m_objects[i];
    //    particle.m_velocity += glm::vec2(0.0f,m_gravity);
    //    particle.update(dt);
    //    resolveCollisions(particle);
    //}
}

void PhysicalSolver::solveCollisions(){

    const uint32_t threadCount = m_threadPool.m_threads.size();
    const uint32_t perThread = m_accelerateGrid.m_grids.size() / threadCount;
    const uint32_t save = m_accelerateGrid.m_grids.size() % threadCount;
    const uint32_t particlePatch = m_accelerateGrid.m_grids.size() / threadCount;
    uint32_t currIndex = 0;
    for (int i = 0; i < m_threadPool.m_threads.size(); ++i) {
        uint32_t realParticlePatch = particlePatch;
        if (i < save) realParticlePatch += 1;
        else if (particlePatch == 0) break;
        m_threadPool.m_threads[i]->addJob([this, i, realParticlePatch]() {
            solveCollisionThreaded(i, realParticlePatch);
            });
        currIndex += realParticlePatch;
    }
    m_threadPool.wait();


    //const uint32_t thread_count = m_threadPool.m_threads.size();
    //const uint32_t slice_count = thread_count*2;
    //const uint32_t slice_size = m_accelerateGrid.m_grids.size()/slice_count;

    //for(uint32_t i = 0;i<thread_count;++i){
    //    m_threadPool.m_threads[i]->addJob([this,i,slice_size](){
    //        solveCollisionThreaded(2*i, slice_size);
    //    });
    //}
    //m_threadPool.wait();
    //for(uint32_t i = 0;i<thread_count;++i){
    //    m_threadPool.m_threads[i]->addJob([this,i,slice_size](){
    //        solveCollisionThreaded(2*i+1, slice_size);
    //    });
    //}
    //m_threadPool.wait();


 /*    for (int i = 0; i < m_objects.size(); ++i) {
         Particle& particle = m_objects[i];
         uint32_t rowIndex = static_cast<uint32_t>(particle.m_center.x)/m_accelerateGrid.m_step;
         uint32_t colIndex = static_cast<uint32_t>(particle.m_center.y)/m_accelerateGrid.m_step;
         auto& grid = m_accelerateGrid.m_grids;
         for(int a = -1;a<2;++a){
             for(int b = -1;b<2;++b){
                 uint32_t realRowIndex = rowIndex+a;
                 uint32_t realColIndex = colIndex+b;
                 if(realRowIndex<0||realRowIndex>=grid.size()||realColIndex<0||realColIndex>=grid[0].size()) continue;
                 auto& newGrid = m_accelerateGrid.m_grids[realRowIndex][realColIndex];
                 for (auto& id : newGrid) {
                     solveContace(particle, m_objects[id]);
                 }
             }
         }
     }*/
}

void PhysicalSolver::solveCollisionThreaded(uint32_t i, uint32_t slice_size){
   // std::cout << i<<' ';
    uint32_t colStartIndex = i*slice_size;
    for(int i = colStartIndex;i< colStartIndex+slice_size;++i){
        if(i>m_accelerateGrid.m_grids.size()-1) break;
        for(int j = 0;j<m_accelerateGrid.m_grids[0].size();++j){
            //ÏÈÄÃÒ»¸öcell
            for(auto& particleIndex:m_accelerateGrid.m_grids[i][j]){
                Particle& particle = m_objects[particleIndex];
                auto& grid = m_accelerateGrid.m_grids;
                for(int a = -1;a<2;++a){
                    for(int b = -1;b<2;++b){
                        uint32_t realRowIndex = i+a;
                        uint32_t realColIndex = j+b;
                        if(realRowIndex<0||realRowIndex>=grid.size()||realColIndex<0||realColIndex>=grid[0].size()) continue;
                        auto& newGrid = m_accelerateGrid.m_grids[realRowIndex][realColIndex];
                        for(auto & id:newGrid){
                            solveContace(particle, m_objects[id]);
                        }
                    }
                }
            }
        }
    }

}

void PhysicalSolver::solveGrid(glm::ivec2 gridIndex1, glm::ivec2 gridIndex2){

    auto& grid1 = m_accelerateGrid.m_grids[gridIndex1.x][gridIndex1.y];
    auto& grid2 = m_accelerateGrid.m_grids[gridIndex2.x][gridIndex2.y];
    for(int i = 0;i<grid1.size();++i){
        for(int j = 0;j<grid2.size();++j){
            if (grid1[i] == grid2[j]) continue;
            Particle& p1 = m_objects[grid1[i]];
            Particle& p2 = m_objects[grid2[j]];
            solveContace(p1, p2);
        }
    }
}

void PhysicalSolver::solveContace(Particle& p1,Particle& p2){
    float response_coef = 1.0f;
    float eps = 0.0001f;
    const glm::vec2 o2_o1 = p1.m_center-p2.m_center;
    const float dis2 = o2_o1.x*o2_o1.x+o2_o1.y*o2_o1.y;
    if(dis2<1.0f&&dis2>eps){
        const float dist = std::sqrt(dis2);
        const float delta = response_coef*0.5f*(1.0f-dist);
        const glm::vec2 col_vec = (o2_o1/dist)*delta;
        p1.m_center +=col_vec;
        p2.m_center -= col_vec;
    }
}

void PhysicalSolver::resolveCollisions(Particle& particle){
    const float margin = particle.m_radius;
    if(particle.m_center.x>windowExtent.x-margin){
        particle.m_center.x = windowExtent.x-margin;
        //++particle.m_boundCount;
    }else if(particle.m_center.x<margin){
        particle.m_center.x = margin;
        //++particle.m_boundCount;
    }
    if(particle.m_center.y > windowExtent.y-margin){
        particle.m_center.y = windowExtent.y-margin;
        //++particle.m_boundCount;
    } else if(particle.m_center.y<margin){
        particle.m_center.y = margin;
        //++particle.m_boundCount;
    }
}

float PhysicalSolver::distance(Particle& p1,Particle& p2){
    return glm::length(p1.m_center-p2.m_center);
}

void PhysicalSolver::addParticleToGrid() {
    
    for (auto& i : m_accelerateGrid.m_grids) {
        for (auto& k : i) {
            uint32_t ksize= k.size();
            k.erase(k.begin(), k.end());
        }
    }
    for(uint32_t i = 0;i<m_objects.size();++i){
        Particle& particle = m_objects[i];
        uint32_t rowIndex = particle.m_center.x/m_accelerateGrid.m_step;
        uint32_t colIndex = particle.m_center.y/m_accelerateGrid.m_step;
        //if (m_accelerateGrid.m_grids[rowIndex - 1][colIndex - 1].size() > 4) continue;
        m_accelerateGrid.m_grids[rowIndex][colIndex].push_back(i);
    }
}