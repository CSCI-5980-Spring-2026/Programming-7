#include <GopherEngine/Renderer/ResourceUploader.hpp>

#include <GL/glew.h>          // Must come first
#include <SFML/OpenGL.hpp>    // After glew

#include <iostream>
using namespace std;

namespace GopherEngine
{
    bool ResourceUploader::upload_mesh(const Mesh& mesh) {

        // Release any existing GPU resources before re-uploading
        if(mesh.vao_ != 0)
            mesh.gpu_release_();

        glGenVertexArrays(1, &mesh.vao_);
        glGenBuffers(1, &mesh.vbo_);
        glGenBuffers(1, &mesh.ebo_);
        
        glBindVertexArray(mesh.vao_);

        glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo_);
        glBufferData(GL_ARRAY_BUFFER, mesh.array_buffer_.size() * sizeof(float), mesh.array_buffer_.data(), GL_STATIC_DRAW);

        // Indices
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ebo_);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, 
                     mesh.element_buffer_.size() * sizeof(unsigned int), 
                     mesh.element_buffer_.data(), 
                     GL_STATIC_DRAW);

        unsigned int stride = mesh.array_stride_ * sizeof(float);  

        // location 0: position
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
        glEnableVertexAttribArray(0);

        // location 1: normal
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        // localtion 2: color
        glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, stride, (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);

        // location 3: uv
        glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, stride, (void*)(10 * sizeof(float)));
        glEnableVertexAttribArray(3);

        // Unbind VAO and buffers to prevent accidental modification
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);   

        // Set up GPU release function to clean up buffers when mesh is destroyed
        mesh.gpu_release_ = [&mesh]() {
            if(mesh.vao_ != 0) glDeleteVertexArrays(1, &mesh.vao_);
            if(mesh.vbo_ != 0) glDeleteBuffers(1, &mesh.vbo_);
            if(mesh.ebo_ != 0) glDeleteBuffers(1, &mesh.ebo_);
            mesh.vao_ = mesh.vbo_ = mesh.ebo_ = 0;
        };

        if(mesh.vao_ == 0)
        {
            cerr << "Failed to upload geometry to GPU" << endl;
            return false;
        }
        
        return true;
    }

    void ResourceUploader::release_mesh(const Mesh& mesh) {

        if(mesh.vao_ != 0) glDeleteVertexArrays(1, &mesh.vao_);
        if(mesh.vbo_ != 0) glDeleteBuffers(1, &mesh.vbo_);
        if(mesh.ebo_ != 0) glDeleteBuffers(1, &mesh.ebo_);
        mesh.vao_ = mesh.vbo_ = mesh.ebo_ = 0;

    }

    bool ResourceUploader::upload_texture(const Texture& texture) {

        if (texture.pixels_.empty() || texture.width_ <= 0 || texture.height_ <= 0) {
            cerr << "ResourceUploader: cannot upload texture with no pixel data or invalid dimensions." << endl;
            return false;
        }

        // Release any existing GPU texture before re-uploading
        if (texture.texture_id_ != 0)
            texture.gpu_release_();

        GLenum gl_format = (texture.format_ == Texture::Format::RGBA) ? GL_RGBA : GL_RGB;

        glGenTextures(1, &texture.texture_id_);
        glBindTexture(GL_TEXTURE_2D, texture.texture_id_);

        // Sensible defaults — caller can rebind and adjust wrap/filter modes afterward
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,     GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,     GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexImage2D(GL_TEXTURE_2D, 0, gl_format,
                    texture.width_, texture.height_, 0,
                    gl_format, GL_UNSIGNED_BYTE, texture.pixels_.data());

        glGenerateMipmap(GL_TEXTURE_2D);

        glBindTexture(GL_TEXTURE_2D, 0);

        if (texture.texture_id_ == 0) {
            cerr << "ResourceUploader: glGenTextures failed." << endl;
            return false;
        }

        // Register the GPU cleanup callback on the texture
        texture.gpu_release_ = [&texture]() {
            if (texture.texture_id_ != 0)
                glDeleteTextures(1, &texture.texture_id_);
            texture.texture_id_ = 0;
        };

        return true;
    }

    void ResourceUploader::release_texture(const Texture& texture) {

        if (texture.texture_id_ != 0) 
            glDeleteTextures(1, &texture.texture_id_);
           
        texture.texture_id_ = 0;
        texture.gpu_release_ = {};
        
    }
}