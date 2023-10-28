#include "accelerateGrid.h"

void AccelerateGrid::init(glm::vec2 windowExtent, uint32_t step){
    m_step = step;
    uint32_t savedRow = static_cast<uint32_t>(windowExtent.x)%step;
    m_grids.clear();
    m_grids.resize(savedRow==0?static_cast<uint32_t>(windowExtent.x)/step:(static_cast<uint32_t>(windowExtent.x)/step)+1);
    uint32_t saveCol = static_cast<uint32_t>(windowExtent.y)%step==0?static_cast<uint32_t>(windowExtent.y)/step:(static_cast<uint32_t>(windowExtent.y)/step)+1;
    for(auto& i:m_grids){
        i.clear();
        i.resize(saveCol);
    }
}