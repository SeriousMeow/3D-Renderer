#include "renderer/utils.hpp"

#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <assimp/Importer.hpp>

namespace renderer::utils {

Object LoadFile(const std::string& path) {
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path.c_str(), aiProcess_Triangulate);

    if ((scene == nullptr) or (scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) or
        (scene->mRootNode == nullptr)) {
        return Object{{}};
    }
    std::vector<renderer::Point> points;
    std::vector<renderer::Triangle> triangles;

    for (size_t mesh_index = 0; mesh_index < scene->mNumMeshes; ++mesh_index) {
        const aiMesh* mesh = scene->mMeshes[mesh_index];
        {
            assert(mesh and "LoadFile: mesh не должен быть nullptr");
        }
        size_t mesh_vertices_start = points.size();
        for (size_t vertex_index = 0; vertex_index < mesh->mNumVertices; ++vertex_index) {
            points.push_back(Point{mesh->mVertices[vertex_index].x, mesh->mVertices[vertex_index].y,
                                   mesh->mVertices[vertex_index].z});
        }
        for (size_t face_index = 0; face_index < mesh->mNumFaces; ++face_index) {
            const aiFace facet = mesh->mFaces[face_index];
            {
                assert((facet.mNumIndices == 3) and "LoadFile: грань должна содержать 3 вершины");
            }
            Triangle new_triangle;
            for (int i = 0; i < 3; ++i) {
                new_triangle.vertices[i].point = points[mesh_vertices_start + facet.mIndices[i]];
            }
            triangles.push_back(new_triangle);
        }
    }

    return Object{triangles};
}

}  // namespace renderer::utils
