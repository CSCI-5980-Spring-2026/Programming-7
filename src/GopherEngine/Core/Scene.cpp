#include <GopherEngine/Core/Scene.hpp>
#include <GopherEngine/Core/RenderContext.hpp>

using namespace std;

namespace GopherEngine {

    Scene::Scene() {

    }

    Scene::~Scene() {

    }

    shared_ptr<Node> Scene::create_default_camera() {

        auto camera_component = make_shared<CameraComponent>();
        auto camera_node = create_node();
        camera_node->add_component(camera_component);

        main_camera_ = camera_component->get_camera();
        main_camera_->set_perspective(60.f, 4.f/3.f, 0.1f, 1000.f);    

        return camera_node;
    }

    shared_ptr<Camera> Scene::get_main_camera() const {
        return main_camera_;
    }

    void Scene::set_main_camera(shared_ptr<Camera> camera) {
        main_camera_ = camera;
        main_camera_->set_projection_matrix_dirty(true);
    }

    void Scene::add_node(shared_ptr<Node> node) {

        nodes_.push_back(node);

    }

    shared_ptr<Node> Scene::create_node() {

        shared_ptr<Node> node = make_shared<Node>();
        nodes_.push_back(node);
        return node;

    }

    void Scene::update(float delta_time) {

        for(auto& node: nodes_) {
            node->update(delta_time);
        }

        for(auto& node: nodes_) {
            node->late_update(delta_time);
        }
    }

    void Scene::draw() {

        RenderContext context;
    
        for(auto& node: nodes_) {
            node->pre_draw(context);
        }

        if(main_camera_) {
            context.view_matrix_ = main_camera_->get_view_matrix();
            context.projection_matrix_ = main_camera_->get_projection_matrix();
            context.eye_position_ = main_camera_->get_world_position();
            
            for(auto& node: nodes_) {
                node->draw(context);
            }
        }

    }
}