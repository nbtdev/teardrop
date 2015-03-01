/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved.
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "ExtensionManager.h"
#include <vector>
#include <assert.h>
#include <string.h>

namespace Teardrop {
namespace Gfx {
namespace OpenGL {

ExtensionManager::ExtensionManager()
    : glBindVertexArray(0)
    , glDeleteVertexArrays(0)
    , glGenVertexArrays(0)
    , glGenSamplers(0)
    , glDeleteSamplers(0)
    , glIsSampler(0)
    , glBindSampler(0)
    , glSamplerParameteri(0)
    , glSamplerParameteriv(0)
    , glSamplerParameterf(0)
    , glSamplerParameterfv(0)
    , glSamplerParameterIiv(0)
    , glSamplerParameterIuiv(0)
    , glGetSamplerParameteriv(0)
    , glGetSamplerParameterIiv(0)
    , glGetSamplerParameterfv(0)
    , glGetSamplerParameterIuiv(0)
    , glIsRenderbuffer(0)
    , glBindRenderbuffer(0)
    , glDeleteRenderbuffers(0)
    , glGenRenderbuffers(0)
    , glRenderbufferStorage(0)
    , glGetRenderbufferParameteriv(0)
    , glIsFramebuffer(0)
    , glBindFramebuffer(0)
    , glDeleteFramebuffers(0)
    , glGenFramebuffers(0)
    , glCheckFramebufferStatus(0)
    , glFramebufferTexture1D(0)
    , glFramebufferTexture2D(0)
    , glFramebufferTexture3D(0)
    , glFramebufferRenderbuffer(0)
    , glGetFramebufferAttachmentParameteriv(0)
    , glGenerateMipmap(0)
    , glBlitFramebuffer(0)
    , glRenderbufferStorageMultisample(0)
    , glFramebufferTextureLayer(0)
{
}

void ExtensionManager::initialize()
{
    const char* ext = (const char*)glGetString(GL_EXTENSIONS);
    if (ext) {
        if (strstr(ext, "GL_ARB_vertex_array_object")) {
            glBindVertexArray = (PFNGLBINDVERTEXARRAYPROC)glXGetProcAddress((GLubyte*)"glBindVertexArray");
            assert(glBindVertexArray);
            glGenVertexArrays = (PFNGLGENVERTEXARRAYSPROC)glXGetProcAddress((GLubyte*)"glGenVertexArrays");
            assert(glGenVertexArrays);
            glDeleteVertexArrays = (PFNGLDELETEVERTEXARRAYSPROC)glXGetProcAddress((GLubyte*)"glDeleteVertexArrays");
            assert(glDeleteVertexArrays);
        }

        if (strstr(ext, "GL_ARB_sampler_objects")) {
            glGenSamplers = (PFNGLGENSAMPLERSPROC) glXGetProcAddress((GLubyte*)"glGenSamplers");
            glDeleteSamplers = (PFNGLDELETESAMPLERSPROC) glXGetProcAddress((GLubyte*)"glDeleteSamplers");
            glIsSampler = (PFNGLISSAMPLERPROC) glXGetProcAddress((GLubyte*)"glIsSampler");
            glBindSampler = (PFNGLBINDSAMPLERPROC) glXGetProcAddress((GLubyte*)"glBindSampler");
            glSamplerParameteri = (PFNGLSAMPLERPARAMETERIPROC) glXGetProcAddress((GLubyte*)"glSamplerParameteri");
            glSamplerParameteriv = (PFNGLSAMPLERPARAMETERIVPROC) glXGetProcAddress((GLubyte*)"glSamplerParameteriv");
            glSamplerParameterf = (PFNGLSAMPLERPARAMETERFPROC) glXGetProcAddress((GLubyte*)"glSamplerParameterf");
            glSamplerParameterfv = (PFNGLSAMPLERPARAMETERFVPROC) glXGetProcAddress((GLubyte*)"glSamplerParameterfv");
            glSamplerParameterIiv = (PFNGLSAMPLERPARAMETERIIVPROC) glXGetProcAddress((GLubyte*)"glSamplerParameterIiv");
            glSamplerParameterIuiv = (PFNGLSAMPLERPARAMETERIUIVPROC) glXGetProcAddress((GLubyte*)"glSamplerParameterIuiv");
            glGetSamplerParameteriv = (PFNGLGETSAMPLERPARAMETERIVPROC) glXGetProcAddress((GLubyte*)"glGetSamplerParameteriv");
            glGetSamplerParameterIiv = (PFNGLGETSAMPLERPARAMETERIIVPROC) glXGetProcAddress((GLubyte*)"glGetSamplerParameterIiv");
            glGetSamplerParameterfv = (PFNGLGETSAMPLERPARAMETERFVPROC) glXGetProcAddress((GLubyte*)"glGetSamplerParameterfv");
            glGetSamplerParameterIuiv = (PFNGLGETSAMPLERPARAMETERIUIVPROC) glXGetProcAddress((GLubyte*)"glGetSamplerParameterIuiv");
        }

        // FBO support (GL 3.x and above, ignore EXT versions that are older)
        if (strstr(ext, "GL_ARB_framebuffer_object")) {
            glIsRenderbuffer = (PFNGLISRENDERBUFFERPROC)glXGetProcAddress(((GLubyte*)"glIsRenderbuffer"));
            glBindRenderbuffer = (PFNGLBINDRENDERBUFFERPROC)glXGetProcAddress(((GLubyte*)"glBindRenderbuffer"));
            glDeleteRenderbuffers = (PFNGLDELETERENDERBUFFERSPROC)glXGetProcAddress(((GLubyte*)"glDeleteRenderbuffer"));
            glGenRenderbuffers = (PFNGLGENRENDERBUFFERSPROC)glXGetProcAddress(((GLubyte*)"glGenRenderbuffers"));
            glRenderbufferStorage = (PFNGLRENDERBUFFERSTORAGEPROC)glXGetProcAddress(((GLubyte*)"glRenderbufferStorage"));
            glGetRenderbufferParameteriv = (PFNGLGETRENDERBUFFERPARAMETERIVPROC)glXGetProcAddress(((GLubyte*)"glGetRenderbufferParameteriv"));
            glIsFramebuffer = (PFNGLISFRAMEBUFFERPROC)glXGetProcAddress(((GLubyte*)"glIsFramebuffer"));
            glBindFramebuffer = (PFNGLBINDFRAMEBUFFERPROC)glXGetProcAddress(((GLubyte*)"glBindFramebuffer"));
            glDeleteFramebuffers = (PFNGLDELETEFRAMEBUFFERSPROC)glXGetProcAddress(((GLubyte*)"glDeleteFramebuffers"));
            glGenFramebuffers = (PFNGLGENFRAMEBUFFERSPROC)glXGetProcAddress(((GLubyte*)"glGenFramebuffers"));
            glCheckFramebufferStatus = (PFNGLCHECKFRAMEBUFFERSTATUSPROC)glXGetProcAddress(((GLubyte*)"glCheckFramebufferStatus"));
            glFramebufferTexture1D = (PFNGLFRAMEBUFFERTEXTURE1DPROC)glXGetProcAddress(((GLubyte*)"glFramebufferTexture1D"));
            glFramebufferTexture2D = (PFNGLFRAMEBUFFERTEXTURE2DPROC)glXGetProcAddress(((GLubyte*)"glFramebufferTexture2D"));
            glFramebufferTexture3D = (PFNGLFRAMEBUFFERTEXTURE3DPROC)glXGetProcAddress(((GLubyte*)"glFramebufferTexture3D"));
            glFramebufferRenderbuffer = (PFNGLFRAMEBUFFERRENDERBUFFERPROC)glXGetProcAddress(((GLubyte*)"glFramebufferRenderbuffer"));
            glGetFramebufferAttachmentParameteriv = (PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVPROC)glXGetProcAddress(((GLubyte*)"glGetFramebufferAttachmentParameteriv"));
            glGenerateMipmap = (PFNGLGENERATEMIPMAPPROC)glXGetProcAddress(((GLubyte*)"glGenerateMipmap"));
            glBlitFramebuffer = (PFNGLBLITFRAMEBUFFERPROC)glXGetProcAddress(((GLubyte*)"glBlitFramebuffer"));
            glRenderbufferStorageMultisample = (PFNGLRENDERBUFFERSTORAGEMULTISAMPLEPROC)glXGetProcAddress(((GLubyte*)"glRenderbufferStorageMultisample"));
            glFramebufferTextureLayer = (PFNGLFRAMEBUFFERTEXTURELAYERPROC)glXGetProcAddress(((GLubyte*)"glFramebufferTextureLayer"));
        }
    }
}

bool ExtensionManager::hasVertexArrayObject()
{
    return (glGenVertexArrays != NULL);
}

void ExtensionManager::bindVertexArray(GLuint array)
{
    if (glBindVertexArray) glBindVertexArray(array);
}

void ExtensionManager::deleteVertexArrays(GLsizei n, const GLuint *arrays)
{
    if (glDeleteVertexArrays) glDeleteVertexArrays(n, arrays);
}

void ExtensionManager::genVertexArrays(GLsizei n, GLuint *arrays)
{
    if (glGenVertexArrays) glGenVertexArrays(n, arrays);
}

void* ExtensionManager::mapBuffer(GLenum target, GLenum access)
{
    return glMapBuffer(target, access);
}

void ExtensionManager::unmapBuffer(GLenum target)
{
    glUnmapBuffer(target);
}

bool ExtensionManager::hasGenSamplers()
{
    return (glGenSamplers != NULL);
}

void ExtensionManager::genSamplers(GLsizei count, GLuint* samplers)
{
    glGenSamplers(count, samplers);
}

void ExtensionManager::deleteSamplers(GLsizei count, const GLuint* samplers)
{
    glDeleteSamplers(count, samplers);
}

bool ExtensionManager::isSampler (GLuint sampler)
{
    return glIsSampler(sampler);
}

void ExtensionManager::bindSampler (GLuint unit, GLuint sampler)
{
    glBindSampler(unit, sampler);
}

void ExtensionManager::samplerParameteri (GLuint sampler, GLenum pname, GLint param)
{
    glSamplerParameteri(sampler, pname, param);
}

void ExtensionManager::samplerParameteriv (GLuint sampler, GLenum pname, const GLint *param)
{
    glSamplerParameteriv(sampler, pname, param);
}

void ExtensionManager::samplerParameterf (GLuint sampler, GLenum pname, GLfloat param)
{
    glSamplerParameterf(sampler, pname, param);
}

void ExtensionManager::samplerParameterfv (GLuint sampler, GLenum pname, const GLfloat *param)
{
    glSamplerParameterfv(sampler, pname, param);
}

void ExtensionManager::samplerParameterIiv (GLuint sampler, GLenum pname, const GLint *param)
{
    glSamplerParameterIiv(sampler, pname, param);
}

void ExtensionManager::samplerParameterIuiv (GLuint sampler, GLenum pname, const GLuint *param)
{
    glSamplerParameterIuiv(sampler, pname, param);
}

void ExtensionManager::getSamplerParameteriv (GLuint sampler, GLenum pname, GLint *params)
{
    glGetSamplerParameteriv(sampler, pname, params);
}

void ExtensionManager::getSamplerParameterIiv (GLuint sampler, GLenum pname, GLint *params)
{
    glGetSamplerParameterIiv(sampler, pname, params);
}

void ExtensionManager::getSamplerParameterfv (GLuint sampler, GLenum pname, GLfloat *params)
{
    glGetSamplerParameterfv(sampler, pname, params);
}

void ExtensionManager::getSamplerParameterIuiv (GLuint sampler, GLenum pname, GLuint *params)
{
    glGetSamplerParameterIuiv(sampler, pname, params);
}

bool ExtensionManager::hasGenFramebuffers()
{
    return (0 != glGenFramebuffers);
}

bool ExtensionManager::isRenderbuffer (GLuint renderbuffer)
{
    return (GL_TRUE == glIsRenderbuffer(renderbuffer));
}

void ExtensionManager::bindRenderbuffer (GLenum target, GLuint renderbuffer)
{
    glBindRenderbuffer(target, renderbuffer);
}

void ExtensionManager::deleteRenderbuffers (GLsizei n, const GLuint *renderbuffers)
{
    glDeleteRenderbuffers(n, renderbuffers);
}

void ExtensionManager::genRenderbuffers (GLsizei n, GLuint *renderbuffers)
{
    glGenRenderbuffers(n, renderbuffers);
}

void ExtensionManager::renderbufferStorage (GLenum target, GLenum internalformat, GLsizei width, GLsizei height)
{
    glRenderbufferStorage(target, internalformat, width, height);
}

void ExtensionManager::getRenderbufferParameteriv (GLenum target, GLenum pname, GLint *params)
{
    glGetRenderbufferParameteriv(target, pname, params);
}

bool ExtensionManager::isFramebuffer (GLuint framebuffer)
{
    return (GL_TRUE == glIsFramebuffer(framebuffer));
}

void ExtensionManager::bindFramebuffer (GLenum target, GLuint framebuffer)
{
    glBindFramebuffer(target, framebuffer);
}

void ExtensionManager::deleteFramebuffers (GLsizei n, const GLuint *framebuffers)
{
    glDeleteFramebuffers(n, framebuffers);
}

void ExtensionManager::genFramebuffers (GLsizei n, GLuint *framebuffers)
{
    glGenFramebuffers(n, framebuffers);
}

GLenum ExtensionManager::checkFramebufferStatus (GLenum target)
{
    return glCheckFramebufferStatus(target);
}

void ExtensionManager::framebufferTexture1D (GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level)
{
    glFramebufferTexture1D(target, attachment, textarget, texture, level);
}

void ExtensionManager::framebufferTexture2D (GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level)
{
    glFramebufferTexture2D(target, attachment, textarget, texture, level);
}

void ExtensionManager::framebufferTexture3D (GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level, GLint zoffset)
{
    glFramebufferTexture3D(target, attachment, textarget, texture, level, zoffset);
}

void ExtensionManager::framebufferRenderbuffer (GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer)
{
    glFramebufferRenderbuffer(target, attachment, renderbuffertarget, renderbuffer);
}

void ExtensionManager::getFramebufferAttachmentParameteriv (GLenum target, GLenum attachment, GLenum pname, GLint *params)
{
    glGetFramebufferAttachmentParameteriv(target, attachment, pname, params);
}

void ExtensionManager::generateMipmap (GLenum target)
{
    glGenerateMipmap(target);
}

void ExtensionManager::blitFramebuffer (GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter)
{
    glBlitFramebuffer(srcX0, srcY0, srcX1, srcY1, dstX0, dstY0, dstX1, dstY1, mask, filter);
}

void ExtensionManager::renderbufferStorageMultisample (GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height)
{
    glRenderbufferStorageMultisample(target, samples, internalformat, width, height);
}

void ExtensionManager::framebufferTextureLayer (GLenum target, GLenum attachment, GLuint texture, GLint level, GLint layer)
{
    glFramebufferTextureLayer(target, attachment, texture, level, layer);
}

} // namespace OpenGL
} // namespace Gfx
} // namespace Teardrop
