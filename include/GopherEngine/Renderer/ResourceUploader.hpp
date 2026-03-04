#pragma once
#include <GopherEngine/Resource/Mesh.hpp>
#include <GopherEngine/Resource/Texture.hpp>

#include <memory>

namespace GopherEngine
{
    class ResourceUploader
    {
        public:
            static bool upload_mesh(const Mesh& mesh);
            static void release_mesh(const Mesh& mesh);

            static bool upload_texture(const Texture& texture);
            static void release_texture(const Texture& texture);
    };
}