#ifndef ACCELE_H
#define ACCELE_H

#include <vector>
#include <glm/glm.hpp>

class AccelerateGrid{
public:
    void init(glm::vec2 windowExtent, uint32_t step);

    std::vector<std::vector<std::vector<uint32_t>>> m_grids;
    uint32_t m_step;
};

#endif