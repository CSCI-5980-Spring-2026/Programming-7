#pragma once

#include <GopherEngine/Core/Events.hpp>
#include <GopherEngine/Platform/Window.hpp>
#include <GopherEngine/Renderer/Renderer.hpp>
#include <GopherEngine/Core/Scene.hpp>
#include <GopherEngine/Core/Clock.hpp>

#include <memory>

namespace GopherEngine
{
    class MainLoop
    {
        public:
            MainLoop();
            ~MainLoop();
            int run();

        // Protected members and functions are accessible to subclasses, but not to outside code
        protected:

            // Pure virtual functions to be implemented by subclasses
            virtual void configure() = 0;
            virtual void initialize() = 0;
            virtual void update(float delta_time) = 0;
            
            // Event handlers that can be overridden by subclasses
            virtual void on_mouse_move(const MouseMoveEvent& event) {}
            virtual void on_mouse_down(const MouseButtonEvent& event) {}
            virtual void on_mouse_up(const MouseButtonEvent& event) {}
            virtual void on_mouse_scroll(const MouseScrollEvent& event) {}
            virtual void on_key_down(const KeyEvent& event) {}
            virtual void on_key_up(const KeyEvent& event) {}
            virtual void on_key_press(const KeyEvent& event) {}

            // Protected member variables that can be accessed by subclasses
            Window window_;
            Renderer renderer_;
            Clock clock_;
            std::shared_ptr<Scene> scene_;

        // Private members and functions are only accessible within the class
        private:
            void handle_resize();
            void register_input_callbacks();

    };
}