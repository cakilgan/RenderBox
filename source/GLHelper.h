#pragma once
#include <Windows.h>
#include <glad.h>
#include <GLFW/glfw3.h>
#include <gl/GL.h>
#include <vector>
namespace GLHelper {
	struct __attrib {
        GLuint index;           
        GLint size;          
        GLenum type;  
        GLboolean normalized;
        size_t offset;
	};
    class Batch {
    public:
        GLuint vao, vbo;
        std::vector<__attrib> layout;
        std::vector<uint8_t> vertexData;  // raw vertex buffer
        size_t stride = 0;                // bir vertex kaç byte?

        Batch():vao(0),vbo(0) {
            
        }

        ~Batch() {
        }
        void init() {
            glGenVertexArrays(1, &vao);
            glGenBuffers(1, &vbo);
        }
        void dispose() {
            glDeleteVertexArrays(1, &vao);
            glDeleteBuffers(1, &vbo);
        }
        void setLayout(const std::vector<__attrib>& attribs) {
            layout = attribs;

            stride = 0;
            for (auto& a : layout) {
                size_t sizePerType = getTypeSize(a.type);
                stride += sizePerType * a.size;
            }

            // offset hesapla
            size_t runningOffset = 0;
            for (auto& a : layout) {
                a.offset = runningOffset;
                runningOffset += getTypeSize(a.type) * a.size;
            }
        }

        void submitVertex(const void* data) {
            const uint8_t* bytes = reinterpret_cast<const uint8_t*>(data);
            vertexData.insert(vertexData.end(), bytes, bytes + stride);
        }

        void upload() {
            glBindVertexArray(vao);
            glBindBuffer(GL_ARRAY_BUFFER, vbo);
            glBufferData(GL_ARRAY_BUFFER, vertexData.size(), vertexData.data(), GL_DYNAMIC_DRAW);

            for (const auto& a : layout) {
                glEnableVertexAttribArray(a.index);
                glVertexAttribPointer(
                    a.index, a.size, a.type, a.normalized,
                    stride, (void*)(a.offset)
                );
            }

            glBindVertexArray(0);
        }

        void draw(GLenum mode = GL_TRIANGLES) {
            glBindVertexArray(vao);
            glDrawArrays(mode, 0, vertexData.size() / stride);
            glBindVertexArray(0);
        }

    private:
        size_t getTypeSize(GLenum type) {
            switch (type) {
            case GL_FLOAT: return sizeof(float);
            case GL_INT: return sizeof(int);
            case GL_UNSIGNED_BYTE: return sizeof(uint8_t);
            default: return 0;
            }
        }
    };

	void enable(GLenum& v0);
	void disable(GLenum& v0);
	
}