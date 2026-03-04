#pragma once

#include <GopherEngine/Core/Transform.hpp>
#include <GopherEngine/Core/Component.hpp>
#include <GopherEngine/Core/RenderContext.hpp>

#include <memory> // for std::shared_ptr
#include <vector> // for std::vector
#include <atomic> // for the atomic id counter

namespace GopherEngine
{
    class Node
    {
        public:
            Node();
            ~Node();
            void update(float delta_time);
            void late_update(float delta_time);
            void pre_draw(RenderContext& context);
            void draw(const RenderContext& context);
            void add_component(std::shared_ptr<Component> component);
            
            const Transform& transform() const;
            Transform& transform();

        private:
            uint32_t id_;
            Transform transform_;
            glm::mat4 local_matrix_{1.f}; 
            glm::mat4 world_matrix_{1.f}; 

            std::vector<std::shared_ptr<Component>> components_;

            // Static member variable declaration for the id counter
            static std::atomic<std::uint32_t> id_counter_;
    };
}