#include "renderer/utils.hpp"

#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <assimp/Importer.hpp>

#include "renderer/resources_manager.hpp"

namespace renderer::utils {

Object LoadFile(const std::string& path) {
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(
        path.c_str(), aiProcess_Triangulate | aiProcess_GenNormals | aiProcess_FlipUVs);

    if ((scene == nullptr) or (scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) or
        (scene->mRootNode == nullptr)) {
        return Object{{}};
    }

    std::string directory;

    if (path.find_last_of('/') != std::string::npos) {
        directory = path.substr(0, path.find_last_of('/') + 1);
    } else {
        directory = "";
    }

    std::vector<renderer::Vertex> vertices;
    std::vector<renderer::Triangle> triangles;
    std::vector<renderer::MaterialId> materials(scene->mNumMaterials);

    ResourcesManager& manager = ResourcesManager::Get();

    for (size_t material_index = 0; material_index < scene->mNumMaterials; ++material_index) {
        const aiMaterial* material = scene->mMaterials[material_index];
        {
            assert(material and "LoadFile: material не должен быть nullptr");
        }
        renderer::Material new_material;
        {
            aiString name;
            if (material->Get(AI_MATKEY_NAME, name) == AI_SUCCESS) {
                if (strcmp(name.C_Str(), "DefaultMaterial") == 0) {
                    // материал по-умолчанию, переопределяется на материал Renderer
                    materials[material_index] = 0;
                    continue;
                }
            }
        }
        {
            aiColor3D color{new_material.ambient.r, new_material.ambient.g, new_material.ambient.b};
            material->Get(AI_MATKEY_COLOR_AMBIENT, color);
            new_material.ambient = {color.r, color.g, color.b};
        }
        {
            aiColor3D color{new_material.diffuse.r, new_material.diffuse.g, new_material.diffuse.b};
            material->Get(AI_MATKEY_COLOR_DIFFUSE, color);
            new_material.diffuse = {color.r, color.g, color.b};
        }
        {
            aiColor3D color{new_material.specular.r, new_material.specular.g,
                            new_material.specular.b};
            material->Get(AI_MATKEY_COLOR_SPECULAR, color);
            new_material.specular = {color.r, color.g, color.b};
        }
        {
            material->Get(AI_MATKEY_SHININESS, new_material.shininess);
        }
        {
            int two_sided = 0;
            material->Get(AI_MATKEY_TWOSIDED, two_sided);
            new_material.two_sided = (two_sided != 0);
        }
        {
            if (material->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
                aiString path;
                material->GetTexture(aiTextureType_DIFFUSE, 0, &path);
                std::string texture_relative_path{path.C_Str()};
                std::string texture_path = directory + texture_relative_path;
                new_material.texture = manager.PushTexture(texture_path);
            }
        }
        materials[material_index] = manager.PushMaterial(new_material);
    }

    for (size_t mesh_index = 0; mesh_index < scene->mNumMeshes; ++mesh_index) {
        const aiMesh* mesh = scene->mMeshes[mesh_index];
        {
            assert(mesh and "LoadFile: mesh не должен быть nullptr");
        }
        MaterialId material_index = 0;
        if (mesh->mMaterialIndex >= 0) {
            MaterialId material_index = materials[mesh->mMaterialIndex];
        }

        size_t mesh_vertices_start = vertices.size();
        for (size_t vertex_index = 0; vertex_index < mesh->mNumVertices; ++vertex_index) {
            Vertex new_vertex;
            new_vertex.point =
                Point{mesh->mVertices[vertex_index].x, mesh->mVertices[vertex_index].y,
                      mesh->mVertices[vertex_index].z};
            new_vertex.normal =
                Point{mesh->mNormals[vertex_index].x, mesh->mNormals[vertex_index].y,
                      mesh->mNormals[vertex_index].z};
            if (mesh->mTextureCoords[0]) {
                new_vertex.uv_coordinates = Point2{mesh->mTextureCoords[0][vertex_index].x,
                                                   mesh->mTextureCoords[0][vertex_index].y};
            }
            vertices.push_back(new_vertex);
        }
        for (size_t face_index = 0; face_index < mesh->mNumFaces; ++face_index) {
            const aiFace facet = mesh->mFaces[face_index];
            {
                assert((facet.mNumIndices == 3) and "LoadFile: грань должна содержать 3 вершины");
            }
            Triangle new_triangle;
            for (int i = 0; i < 3; ++i) {
                new_triangle.vertices[i] = vertices[mesh_vertices_start + facet.mIndices[i]];
            }
            new_triangle.material = materials[material_index];
            triangles.push_back(new_triangle);
        }
    }

    return Object{triangles};
}

}  // namespace renderer::utils
