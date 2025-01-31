#include "renderer/utils.hpp"

#include <fstream>
#include <sstream>

namespace renderer::utils {

Object LoadObjFile(const std::string& path) {
    std::ifstream input{path};
    if (not input.is_open()) {
        throw std::runtime_error{"LoadObjFile: не удалось открыть файл"};
    }
    std::vector<renderer::Point> points;
    std::vector<renderer::Triangle> triangles;
    std::string line;
    while (std::getline(input, line)) {
        if (line.empty()) {
            continue;
        }
        std::stringstream s;
        s << line;
        if (line[0] == 'v') {
            char _;
            Point new_point;
            s >> _ >> new_point.x >> new_point.y >> new_point.z;
            points.push_back(new_point);
        }
        if (line[0] == 'f') {
            char _;
            Triangle new_triangle;
            size_t index;
            s >> _;
            for (size_t i = 0; i < 3; ++i) {
                s >> index;
                new_triangle.vertices[i].point = points[index - 1];
            }
            triangles.push_back(new_triangle);
        }
    }
    input.close();
    return Object{triangles};
}
}  // namespace renderer::utils
