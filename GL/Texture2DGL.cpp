#include "Texture2DGL.h"
#include "../common/Exception.h"
#include "../image/RawImage.h"

namespace Core {

    Texture2DGL::Texture2DGL(const TextureAttributes& attributes): Texture2D(attributes) {

    }

    Texture2DGL::~Texture2DGL() {
        if (this->textureId > 0) {
            glDeleteTextures(1, &this->textureId);
        }
    }

    void Texture2DGL::build(WeakPointer<RawImage> imageData) {
        this->setupTexture(imageData->getWidth(), imageData->getHeight(), imageData->getImageData());
    }
      
    void Texture2DGL::build(UInt32 width, UInt32 height) {
        this->setupTexture(width, height, nullptr);
    }

    void Texture2DGL::setupTexture(UInt32 width, UInt32 height, Byte* data) {
        GLuint tex;

        // generate the OpenGL texture
        glGenTextures(1, &tex);
        if (!tex) {
            throw AllocationException("Texture2DGL::setupTexture -> Unable to generate texture");
        }
        glBindTexture(GL_TEXTURE_2D, tex);

        // set the wrap mode
        if (this->attributes.WrapMode == TextureWrap::Mirror) {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
        }
        else if (this->attributes.WrapMode == TextureWrap::Repeat) {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        }
        else {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        }

        // set the filter mode. if bi-linear or tri-linear filtering is used,
        // we will be using mip-maps
        if (this->attributes.FilterMode == TextureFilter::Linear) {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        }
        else if (this->attributes.FilterMode == TextureFilter::BiLinear) {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        }
        else if (this->attributes.FilterMode == TextureFilter::TriLinear) {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        }
        else {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        }

        // we only generate mip-maps if bi-linear or tri-linear filtering is used
        if (this->attributes.FilterMode == TextureFilter::TriLinear || attributes.FilterMode == TextureFilter::BiLinear) {
            glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, attributes.MipMapLevel);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, attributes.MipMapLevel);
        }

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

        glBindTexture(GL_TEXTURE_2D, 0);
        this->textureId = (Int32)tex;
    }
    
};
