/* Programming 7: Asynchronous File I/O
 * CSCI 5980, Spring 2026, University of Minnesota
 * Instructor: Evan Suma Rosenberg <suma@umn.edu>
 */ 


#include <GopherEngine/Core/MainLoop.hpp>
#include <GopherEngine/Core/LightComponent.hpp>
#include <GopherEngine/Core/MeshComponent.hpp>
#include <GopherEngine/Renderer/BlinnPhongMaterial.hpp>
#include <GopherEngine/Resource/MeshFactory.hpp>
#include <GopherEngine/Core/FileLoader.hpp>
using namespace GopherEngine;

#include <SFML/Graphics/Image.hpp>

#include <memory>
#include <iostream>
using namespace std;

// A simple subclass of MainLoop to test that the main loop is working
// and the window, scene, and node classes are functioning correctly
class MainLoopTest: public MainLoop
{
	public:
		// Constructor and destructor
		MainLoopTest();
		~MainLoopTest();

	private:

		// Override the pure virtual functions from MainLoop
		void configure() override;
		void initialize() override;
		void update(float delta_time) override;

		shared_ptr<Node> test_node_;
		shared_ptr<LoadHandle> test_handle_;

};


MainLoopTest::MainLoopTest() {

}

MainLoopTest::~MainLoopTest() {

}

// This function is called once at the beginning of the main loop, before the window is created.
// This means the OpenGL context is not yet available. It should be used for initial configuration.
void MainLoopTest::configure() {

	window_.set_title("CSCI 5980 Programming 7");
	renderer_.set_background_color(glm::vec4(0.5f, 0.75f, .9f, 1.f));
}

// This function is called once at the beginning of the main loop, after the window is created
// and the OpenGL context is available. It should be used for initializing the scene.
void MainLoopTest::initialize() {
	
	// Create default camera and set its initial position
	auto camera_node = scene_->create_default_camera();
	camera_node->transform().position_ = glm::vec3(0.f, 0.f, 3.f);

	// Create a new material
	auto test_material = make_shared<BlinnPhongMaterial>();
	test_material->set_ambient_color(glm::vec3(0.2f, 0.2f, 0.2f));
    test_material->set_diffuse_color(glm::vec3(0.2f, 0.2f, 0.2f));
    test_material->set_specular_color(glm::vec3(2.f, 2.f, 2.f));
    test_material->set_shininess(64.f);

	// Load a texture from disk and set it on the material. 
	auto handle = FileLoader::load_file_async("assets/Gravel_001_BaseColor.jpg");
	handle.on_complete([test_material](auto &file_data) {
		if(file_data.ok_) {
			cout << "Successfully loaded texture: " << file_data.path_ << " (" << file_data.bytes_.size() << " bytes)" << endl;

			sf::Image image;
			if(image.loadFromMemory(file_data.bytes_.data(), file_data.bytes_.size())) {
				auto texture = std::make_shared<GopherEngine::Texture>();
				texture->format_ = GopherEngine::Texture::Format::RGBA; 
				texture->width_  = image.getSize().x;
				texture->height_ = image.getSize().y;
				texture->pixels_.assign(
					image.getPixelsPtr(),
					image.getPixelsPtr() + texture->width_ * texture->height_ * 4
				);
				test_material->set_texture(texture);
			}
		} 
		else {
			cout << file_data.error_ << endl;
		}
	});

	test_handle_ = make_shared<LoadHandle>(handle);
	
    

	// Create a single node in the scene
	test_node_ = scene_->create_node();
	test_node_->transform().position_ = glm::vec3(0.f, 0.f, 0.f);

	// Add a mesh component to the node 
	auto test_component = make_shared<MeshComponent>();
	test_component->set_mesh(MeshFactory::create_cube());
	test_component->set_material(test_material);
	test_node_->add_component(test_component);
	
	// Create a point light
	auto light_component = make_shared<LightComponent>(LightType::Point);
	light_component->get_light()->ambient_intensity_ = glm::vec3(0.2f, 0.2f, 0.2f);
	light_component->get_light()->diffuse_intensity_ = glm::vec3(1.f, 1.f, 1.f);
	light_component->get_light()->specular_intensity_ = glm::vec3(1.f, 1.f, 1.f);
	
	// Add the point light to the scene
	auto light_node = scene_->create_node();
	light_node->add_component(light_component);
	light_node->transform().position_ = glm::vec3(-10.f, 0.f, 0.f);

}

// This function is called once per frame, before the scene's update function is called.
void MainLoopTest::update(float delta_time) {

	// Rotate the node around the Y axis at a constant speed of 45 degrees per second
    auto frame_rotation = glm::angleAxis(
        delta_time * glm::radians(45.f), 
        glm::vec3(0.f, 1.f, 0.f)
    );
    test_node_->transform().rotation_ =  frame_rotation * test_node_->transform().rotation_;

	if(test_handle_ && test_handle_->is_ready()) {
		test_handle_->fire_callback();
		test_handle_.reset();
	}
}

int main()
{
	// Create an instance of the MainLoop subclass and start the main game loop
	MainLoopTest app;
	return app.run();
}