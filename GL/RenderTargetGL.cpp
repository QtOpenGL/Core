#include "RenderTargetGL.h"

namespace Core {

    RenderTargetGL::RenderTargetGL(Int32 initialFBOID): fboID(initialFBOID) {

    }

    RenderTargetGL::~RenderTargetGL() {
        this->destroy();
    }

    /*
     * Get the OpenGL FBO ID.
     */
    GLuint RenderTargetGL::getFBOID() const {
        return fboID;
    }

    /*
     * Destroy the FBO associated with this render target and all attached textures and/or
     * render buffers.
     */
    void RenderTargetGL::destroy() {
        // destroy the FBO
        if (fboID > 0) {
            glDeleteFramebuffers(1, &fboID);
        }
    }

    void RenderTargetGL::initFramebuffer() {
        // make sure to clean up existing frame-buffer objects (if they exist).
        this->destroy();

        // generate an OpenGL FBO.
        glGenFramebuffers(1, &fboID);
        if (fboID == 0) {
            throw RenderTargetException("RenderTargetGL::initFramebuffer -> Unable to create frame buffer object.");
        }

        glBindFramebuffer(GL_FRAMEBUFFER, fboID);
        
    }

    void RenderTargetGL::completeFramebuffer() {
        UInt32 status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

        if (status != GL_FRAMEBUFFER_COMPLETE) {
            switch(status) {
                case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
                    throw RenderTargetException("RenderTargetCubeGL::init -> Framebuffer is incomplete!. (GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT)");
                break;
             //   case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS:
            //        throw RenderTargetException("RenderTargetCubeGL::init -> Framebuffer is incomplete!. (GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS)");
             //   break;
                case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
                    throw RenderTargetException("RenderTargetCubeGL::init -> Framebuffer is incomplete!. (GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT)");
                break;
                case GL_FRAMEBUFFER_UNSUPPORTED:
                    throw RenderTargetException("RenderTargetCubeGL::init -> Framebuffer is incomplete!. (GL_FRAMEBUFFER_UNSUPPORTED)");
                break;

            }
            throw RenderTargetException("RenderTargetCubeGL::init -> Framebuffer is incomplete!.");
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void RenderTargetGL::initDepthStencilBufferCombo(UInt32 sizeX, UInt32 sizeY) {
         // In OpengL, if we want depth AND stencil abilities then they must both be render buffers and
        // they must be shared.

        GLuint depthStencilRenderBufferID;
        glGenRenderbuffers(1, &depthStencilRenderBufferID);

        if (depthStencilRenderBufferID == 0) {
            throw RenderTargetException("RenderTargetCubeGL::init -> Unable to create depth/stencil render buffer.");
        }

        glBindRenderbuffer(GL_RENDERBUFFER, depthStencilRenderBufferID);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, sizeX, sizeY);

        //Attach stencil buffer to FBO
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, depthStencilRenderBufferID);
    }

}
