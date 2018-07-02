#include <memory>
#include <vector>

#include "CubeTextureGL.h"
#include "../common/Exception.h"
#include "../common/gl.h"
#include "../image/RawImage.h"

namespace Core {

    CubeTextureGL::CubeTextureGL(const TextureAttributes& attributes): CubeTexture(attributes) {

    }

    CubeTextureGL::~CubeTextureGL() {
        
    }

    void CubeTextureGL::build(WeakPointer<RawImage> front, WeakPointer<RawImage> back, 
                              WeakPointer<RawImage> top, WeakPointer<RawImage> bottom, 
                              WeakPointer<RawImage> left, WeakPointer<RawImage> right) {
                                  
        this->setupTexture(front->getWidth(), front->getHeight(), 
                           front->getImageData(), back->getImageData(), 
                           top->getImageData(), bottom->getImageData(), 
                           left->getImageData(), right->getImageData());
    }

    void CubeTextureGL::build(UInt32 width, UInt32 height) {
        this->setupTexture(width, height, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
    }

    void CubeTextureGL::setupTexture(UInt32 width, UInt32 height, Byte* front, Byte* back, Byte* top, Byte* bottom, Byte* left, Byte* right) {
        // glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

        GLuint tex;

        // generate the OpenGL cube texture
        glGenTextures(1, &tex);
        if (!tex) {
            throw AllocationException("CubeTexture::createCubeTexture -> Unable to generate texture");
        }
        glBindTexture(GL_TEXTURE_CUBE_MAP, tex);

        std::vector<WeakPointer<RawImage>> images = {front, back, top, bottom, left, right};
        std::vector<GLuint> faces = {GL_TEXTURE_CUBE_MAP_POSITIVE_Z, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,
                                     GL_TEXTURE_CUBE_MAP_POSITIVE_Y, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
                                     GL_TEXTURE_CUBE_MAP_NEGATIVE_X, GL_TEXTURE_CUBE_MAP_POSITIVE_X};

        for (UInt32 i = 0; i < 6; i++) {
            glTexImage2D(faces[i], 0, GL_RGBA, images[i]->getWidth(), images[i]->getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, images[i]->getImageData());
        }

        // set the relevant texture properties
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

        glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

        this->textureId = (Int32)tex;
    }
}