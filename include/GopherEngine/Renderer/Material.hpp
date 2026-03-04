#pragma once

#include <GopherEngine/Resource/Geometry.hpp>
#include <GopherEngine/Renderer/ShaderProgram.hpp>
#include <GopherEngine/Core/RenderContext.hpp>

#include <glm/glm.hpp>
#include <memory>

namespace GopherEngine
{
    class Material
    {
        public:
            Material() = default;
            virtual ~Material() = default;
            virtual void draw(const Geometry& geometry, const glm::mat4 &world_matrix, const RenderContext& context) = 0;

        protected:
            void bind_shader(const ShaderProgram& shader_program, const glm::mat4 &world_matrix, const RenderContext& context, bool compute_normal_matrix, bool enable_lighting);
            void unbind_shader(const ShaderProgram& shader_program);
    };
}