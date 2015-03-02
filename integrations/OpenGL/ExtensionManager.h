/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved.
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#ifndef EXTENSIONMANAGER_H
#define EXTENSIONMANAGER_H

#include <Core/Singleton.h>
#define GL_GLEXT_PROTOTYPES 1
#include <GL/gl.h>
#include <GL/glext.h>
#include <vector>

namespace Teardrop {
    namespace Gfx {
        namespace OpenGL {

            class ExtensionManager : public Singleton<ExtensionManager>
            {
            public:
                ExtensionManager();
                ~ExtensionManager();

                void initialize();

                bool hasVertexArrayObject();
                void bindVertexArray(GLuint array);
                void deleteVertexArrays(GLsizei n, const GLuint *arrays);
                void genVertexArrays(GLsizei n, GLuint *arrays);

                // glMapBuffer has been part of core OpenGL since 1.5
                bool hasMapBuffer() { return true; }
                virtual void* mapBuffer(GLenum target, GLenum access);
                virtual void unmapBuffer(GLenum target);

                // these may not appear in gl.h even though they
                // are also in core OpenGL since 1.x...
                void genBuffers(GLsizei aCount, GLuint* aBuffers);
                void deleteBuffers(GLsizei aCount, GLuint* aBuffers);
                void bindBuffer(GLenum target, GLuint aBuffer);
                void bufferData(GLenum target, GLsizeiptr aSize, const GLvoid* aData, GLenum aUsage);

                // glGenSamplers is present in 3.3 and above
                bool hasGenSamplers();
                void genSamplers(GLsizei count, GLuint* samplers);
                void deleteSamplers(GLsizei count, const GLuint* samplers);
                bool isSampler (GLuint sampler);
                void bindSampler (GLuint unit, GLuint sampler);
                void samplerParameteri (GLuint sampler, GLenum pname, GLint param);
                void samplerParameteriv (GLuint sampler, GLenum pname, const GLint *param);
                void samplerParameterf (GLuint sampler, GLenum pname, GLfloat param);
                void samplerParameterfv (GLuint sampler, GLenum pname, const GLfloat *param);
                void samplerParameterIiv (GLuint sampler, GLenum pname, const GLint *param);
                void samplerParameterIuiv (GLuint sampler, GLenum pname, const GLuint *param);
                void getSamplerParameteriv (GLuint sampler, GLenum pname, GLint *params);
                void getSamplerParameterIiv (GLuint sampler, GLenum pname, GLint *params);
                void getSamplerParameterfv (GLuint sampler, GLenum pname, GLfloat *params);
                void getSamplerParameterIuiv (GLuint sampler, GLenum pname, GLuint *params);

                // glGenFramebuffers is present in 3.0 and above
                bool hasGenFramebuffers();
                bool isRenderbuffer (GLuint renderbuffer);
                void bindRenderbuffer (GLenum target, GLuint renderbuffer);
                void deleteRenderbuffers (GLsizei n, const GLuint *renderbuffers);
                void genRenderbuffers (GLsizei n, GLuint *renderbuffers);
                void renderbufferStorage (GLenum target, GLenum internalformat, GLsizei width, GLsizei height);
                void getRenderbufferParameteriv (GLenum target, GLenum pname, GLint *params);
                bool isFramebuffer (GLuint framebuffer);
                void bindFramebuffer (GLenum target, GLuint framebuffer);
                void deleteFramebuffers (GLsizei n, const GLuint *framebuffers);
                void genFramebuffers (GLsizei n, GLuint *framebuffers);
                GLenum checkFramebufferStatus (GLenum target);
                void framebufferTexture1D (GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
                void framebufferTexture2D (GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
                void framebufferTexture3D (GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level, GLint zoffset);
                void framebufferRenderbuffer (GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer);
                void getFramebufferAttachmentParameteriv (GLenum target, GLenum attachment, GLenum pname, GLint *params);
                void generateMipmap (GLenum target);
                void blitFramebuffer (GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter);
                void renderbufferStorageMultisample (GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height);
                void framebufferTextureLayer (GLenum target, GLenum attachment, GLuint texture, GLint level, GLint layer);

            private:
                PFNGLBINDVERTEXARRAYPROC glBindVertexArray = nullptr;
                PFNGLDELETEVERTEXARRAYSPROC glDeleteVertexArrays = nullptr;
                PFNGLGENVERTEXARRAYSPROC glGenVertexArrays = nullptr;
                PFNGLGENBUFFERSPROC glGenBuffers = nullptr;
                PFNGLDELETEBUFFERSPROC glDeleteBuffers = nullptr;
                PFNGLBINDBUFFERPROC glBindBuffer = nullptr;
                PFNGLBUFFERDATAPROC glBufferData = nullptr;

                PFNGLGENSAMPLERSPROC glGenSamplers = nullptr;
                PFNGLDELETESAMPLERSPROC glDeleteSamplers = nullptr;
                PFNGLISSAMPLERPROC glIsSampler = nullptr;
                PFNGLBINDSAMPLERPROC glBindSampler = nullptr;
                PFNGLSAMPLERPARAMETERIPROC glSamplerParameteri = nullptr;
                PFNGLSAMPLERPARAMETERIVPROC glSamplerParameteriv = nullptr;
                PFNGLSAMPLERPARAMETERFPROC glSamplerParameterf = nullptr;
                PFNGLSAMPLERPARAMETERFVPROC glSamplerParameterfv = nullptr;
                PFNGLSAMPLERPARAMETERIIVPROC glSamplerParameterIiv = nullptr;
                PFNGLSAMPLERPARAMETERIUIVPROC glSamplerParameterIuiv = nullptr;
                PFNGLGETSAMPLERPARAMETERIVPROC glGetSamplerParameteriv = nullptr;
                PFNGLGETSAMPLERPARAMETERIIVPROC glGetSamplerParameterIiv = nullptr;
                PFNGLGETSAMPLERPARAMETERFVPROC glGetSamplerParameterfv = nullptr;
                PFNGLGETSAMPLERPARAMETERIUIVPROC glGetSamplerParameterIuiv = nullptr;

                PFNGLISRENDERBUFFERPROC glIsRenderbuffer = nullptr;
                PFNGLBINDRENDERBUFFERPROC glBindRenderbuffer = nullptr;
                PFNGLDELETERENDERBUFFERSPROC glDeleteRenderbuffers = nullptr;
                PFNGLGENRENDERBUFFERSPROC glGenRenderbuffers = nullptr;
                PFNGLRENDERBUFFERSTORAGEPROC glRenderbufferStorage = nullptr;
                PFNGLGETRENDERBUFFERPARAMETERIVPROC glGetRenderbufferParameteriv = nullptr;
                PFNGLISFRAMEBUFFERPROC glIsFramebuffer = nullptr;
                PFNGLBINDFRAMEBUFFERPROC glBindFramebuffer = nullptr;
                PFNGLDELETEFRAMEBUFFERSPROC glDeleteFramebuffers = nullptr;
                PFNGLGENFRAMEBUFFERSPROC glGenFramebuffers = nullptr;
                PFNGLCHECKFRAMEBUFFERSTATUSPROC glCheckFramebufferStatus = nullptr;
                PFNGLFRAMEBUFFERTEXTURE1DPROC glFramebufferTexture1D = nullptr;
                PFNGLFRAMEBUFFERTEXTURE2DPROC glFramebufferTexture2D = nullptr;
                PFNGLFRAMEBUFFERTEXTURE3DPROC glFramebufferTexture3D = nullptr;
                PFNGLFRAMEBUFFERRENDERBUFFERPROC glFramebufferRenderbuffer = nullptr;
                PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVPROC glGetFramebufferAttachmentParameteriv = nullptr;
                PFNGLGENERATEMIPMAPPROC glGenerateMipmap = nullptr;
                PFNGLBLITFRAMEBUFFERPROC glBlitFramebuffer = nullptr;
                PFNGLRENDERBUFFERSTORAGEMULTISAMPLEPROC glRenderbufferStorageMultisample = nullptr;
                PFNGLFRAMEBUFFERTEXTURELAYERPROC glFramebufferTextureLayer = nullptr;
            };
        } // namespace OpenGL
    } // namespace Gfx
} // namespace Teardrop

#endif // EXTENSIONMANAGER_H
