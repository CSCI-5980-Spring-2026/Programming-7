#include <GopherEngine/Resource/MeshFactory.hpp>

#include <glm/glm.hpp>
using namespace glm;

#include <vector>
using namespace std;

namespace GopherEngine {

    std::shared_ptr<Mesh> MeshFactory::create_cube(float width, float height, float depth)
    {
        const float w = width  / 2.f;
        const float h = height / 2.f;
        const float d = depth  / 2.f;

        vector<vec3> vertices = {
            // Front face
            {-w, -h,  d}, {w, -h,  d}, {w,  h,  d}, {-w,  h,  d},
            // Back face
            {-w, -h, -d}, {w, -h, -d}, {w,  h, -d}, {-w,  h, -d},
            // Left face
            {-w, -h, -d}, {-w, -h,  d}, {-w,  h,  d}, {-w,  h, -d},
            // Right face
            {w, -h, -d}, {w, -h,  d}, {w,  h,  d}, {w,  h, -d},
            // Top face
            {-w,  h,  d}, {w,  h,  d}, {w,  h, -d}, {-w,  h, -d},
            // Bottom face
            {-w, -h,  d}, {w, -h,  d}, {w, -h, -d}, {-w, -h, -d},
        };

        vector<vec3> normals = {
            // Front face
            {0.f,  0.f,  1.f}, {0.f,  0.f,  1.f}, {0.f,  0.f,  1.f}, {0.f,  0.f,  1.f},
            // Back face
            {0.f,  0.f, -1.f}, {0.f,  0.f, -1.f}, {0.f,  0.f, -1.f}, {0.f,  0.f, -1.f},
            // Left face
            {-1.f,  0.f,  0.f}, {-1.f,  0.f,  0.f}, {-1.f,  0.f,  0.f}, {-1.f,  0.f,  0.f},
            // Right face
            {1.f,  0.f,  0.f}, {1.f,  0.f,  0.f}, {1.f,  0.f,  0.f}, {1.f,  0.f,  0.f},
            // Top face
            {0.f,  1.f,  0.f}, {0.f,  1.f,  0.f}, {0.f,  1.f,  0.f}, {0.f,  1.f,  0.f},
            // Bottom face
            {0.f, -1.f,  0.f}, {0.f, -1.f,  0.f}, {0.f, -1.f,  0.f}, {0.f, -1.f,  0.f},
        };

        vector<vec2> uvs = {
            // Front face
            {0.f, 1.f}, {1.f, 1.f}, {1.f, 0.f}, {0.f, 0.f},
            // Back face
            {1.f, 1.f}, {0.f, 1.f}, {0.f, 0.f}, {1.f, 0.f},
            // Left face
            {0.f, 1.f}, {1.f, 1.f}, {1.f, 0.f}, {0.f, 0.f},
            // Right face
            {1.f, 1.f}, {0.f, 1.f}, {0.f, 0.f}, {1.f, 0.f},
            // Top face
            {0.f, 1.f}, {1.f, 1.f}, {1.f, 0.f}, {0.f, 0.f},
            // Bottom face
            {1.f, 1.f}, {0.f, 1.f}, {0.f, 0.f}, {1.f, 0.f},
        };

        vector<uint32_t> indices = {
            // Front face
             0,  1,  2,  2,  3,  0,
            // Back face
             4,  6,  5,  6,  4,  7,
            // Left face
             8,  9, 10, 10, 11,  8,
            // Right face
            12, 14, 13, 14, 12, 15,
            // Top face
            16, 17, 18, 18, 19, 16,
            // Bottom face
            20, 22, 21, 22, 20, 23,
        };

        auto mesh = std::make_shared<Mesh>();
        mesh->array_buffer_ = interleave_vertex_data(vertices, normals, {}, uvs);
        mesh->element_buffer_ = indices;
        return mesh;
    }

    vector<float> MeshFactory::interleave_vertex_data(const vector<vec3>& vertices, const vector<vec3>& normals, const vector<vec4>& colors, const vector<vec2>& uvs)
    {
        std::vector<float> buffer;

        // 3 + 3 + 4 + 2 floats per vertex
        buffer.reserve(vertices.size() * 12);  

        for(size_t i = 0; i < vertices.size(); i++)
        {
            buffer.push_back(vertices[i].x);
            buffer.push_back(vertices[i].y);
            buffer.push_back(vertices[i].z);

            if(i < normals.size())
            {
                buffer.push_back(normals[i].x);
                buffer.push_back(normals[i].y);
                buffer.push_back(normals[i].z);
            }
            else
            {
                buffer.push_back(0.f);
                buffer.push_back(0.f);
                buffer.push_back(0.f);
            }

            if(i < colors.size())
            {
                buffer.push_back(colors[i].r);
                buffer.push_back(colors[i].g);
                buffer.push_back(colors[i].b);
                buffer.push_back(colors[i].a);
            }
            else
            {
                buffer.push_back(1.f);
                buffer.push_back(1.f);
                buffer.push_back(1.f);
                buffer.push_back(1.f);
            }

            if(i < uvs.size())
            {
                buffer.push_back(uvs[i].x);
                buffer.push_back(uvs[i].y);
            }
            else
            {
                buffer.push_back(0.f);
                buffer.push_back(0.f);
            }
        }

        return buffer;
    }   
}