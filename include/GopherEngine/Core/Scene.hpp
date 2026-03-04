#pragma once

#include <GopherEngine/Core/Node.hpp>
#include <GopherEngine/Core/CameraComponent.hpp>

#include <memory> // for std::shared_ptr
#include <vector> // for std::vector

namespace GopherEngine
{
    class Scene
    {
        public:
            Scene();
            ~Scene();

            std::shared_ptr<Node> create_default_camera();
            std::shared_ptr<Camera> get_main_camera() const;
            void set_main_camera(std::shared_ptr<Camera> camera);

            void update(float delta_time);
            void draw();
            void add_node(std::shared_ptr<Node> node);
            std::shared_ptr<Node> create_node();
            
        private:
            std::vector<std::shared_ptr<Node>> nodes_;
            std::shared_ptr<Camera> main_camera_{nullptr};
    };
}