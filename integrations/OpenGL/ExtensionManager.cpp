/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved.
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "ExtensionManager.h"
#include <assert.h>
#include <string.h>
#include <vector>

template<> Teardrop::Gfx::OpenGL::ExtensionManager* Teardrop::Singleton<Teardrop::Gfx::OpenGL::ExtensionManager>::mInst = 0;

namespace Teardrop {
namespace Gfx {
namespace OpenGL {

ExtensionManager::ExtensionManager()
{
    assert(!mInst);
    mInst = this;
}

ExtensionManager::~ExtensionManager()
{

}

void ExtensionManager::initialize()
{
    const char* ext = (const char*)glGetString(GL_EXTENSIONS);
    if (ext) {
        // things that simply are there at this time and date...
        glGenBuffers = (PFNGLGENBUFFERSPROC)GL_GETPROCADDRESS(GL_UBYTE_PTR "glGenBuffers");
        assert(glGenBuffers);
        glDeleteBuffers = (PFNGLDELETEBUFFERSPROC)GL_GETPROCADDRESS(GL_UBYTE_PTR "glDeleteBuffers");
        assert(glDeleteBuffers);
        glBindBuffer = (PFNGLBINDBUFFERPROC)GL_GETPROCADDRESS(GL_UBYTE_PTR "glBindBuffer");
        assert(glBindBuffer);
		glBufferData = (PFNGLBUFFERDATAPROC)GL_GETPROCADDRESS(GL_UBYTE_PTR "glBufferData");
		assert(glBufferData);
		glMapBuffer = (PFNGLMAPBUFFERPROC)GL_GETPROCADDRESS(GL_UBYTE_PTR "glMapBuffer");
		assert(glMapBuffer);
		glUnmapBuffer = (PFNGLUNMAPBUFFERPROC)GL_GETPROCADDRESS(GL_UBYTE_PTR "glUnmapBuffer");
		assert(glUnmapBuffer);

        if (strstr(ext, "GL_ARB_vertex_array_object")) {
            glBindVertexArray = (PFNGLBINDVERTEXARRAYPROC)GL_GETPROCADDRESS(GL_UBYTE_PTR "glBindVertexArray");
            assert(glBindVertexArray);
            glGenVertexArrays = (PFNGLGENVERTEXARRAYSPROC)GL_GETPROCADDRESS(GL_UBYTE_PTR "glGenVertexArrays");
            assert(glGenVertexArrays);
            glDeleteVertexArrays = (PFNGLDELETEVERTEXARRAYSPROC)GL_GETPROCADDRESS(GL_UBYTE_PTR "glDeleteVertexArrays");
            assert(glDeleteVertexArrays);
        }

        if (strstr(ext, "GL_ARB_sampler_objects")) {
            glGenSamplers = (PFNGLGENSAMPLERSPROC) GL_GETPROCADDRESS(GL_UBYTE_PTR "glGenSamplers");
            glDeleteSamplers = (PFNGLDELETESAMPLERSPROC) GL_GETPROCADDRESS(GL_UBYTE_PTR "glDeleteSamplers");
            glIsSampler = (PFNGLISSAMPLERPROC) GL_GETPROCADDRESS(GL_UBYTE_PTR "glIsSampler");
            glBindSampler = (PFNGLBINDSAMPLERPROC) GL_GETPROCADDRESS(GL_UBYTE_PTR "glBindSampler");
            glSamplerParameteri = (PFNGLSAMPLERPARAMETERIPROC) GL_GETPROCADDRESS(GL_UBYTE_PTR "glSamplerParameteri");
            glSamplerParameteriv = (PFNGLSAMPLERPARAMETERIVPROC) GL_GETPROCADDRESS(GL_UBYTE_PTR "glSamplerParameteriv");
            glSamplerParameterf = (PFNGLSAMPLERPARAMETERFPROC) GL_GETPROCADDRESS(GL_UBYTE_PTR "glSamplerParameterf");
            glSamplerParameterfv = (PFNGLSAMPLERPARAMETERFVPROC) GL_GETPROCADDRESS(GL_UBYTE_PTR "glSamplerParameterfv");
            glSamplerParameterIiv = (PFNGLSAMPLERPARAMETERIIVPROC) GL_GETPROCADDRESS(GL_UBYTE_PTR "glSamplerParameterIiv");
            glSamplerParameterIuiv = (PFNGLSAMPLERPARAMETERIUIVPROC) GL_GETPROCADDRESS(GL_UBYTE_PTR "glSamplerParameterIuiv");
            glGetSamplerParameteriv = (PFNGLGETSAMPLERPARAMETERIVPROC) GL_GETPROCADDRESS(GL_UBYTE_PTR "glGetSamplerParameteriv");
            glGetSamplerParameterIiv = (PFNGLGETSAMPLERPARAMETERIIVPROC) GL_GETPROCADDRESS(GL_UBYTE_PTR "glGetSamplerParameterIiv");
            glGetSamplerParameterfv = (PFNGLGETSAMPLERPARAMETERFVPROC) GL_GETPROCADDRESS(GL_UBYTE_PTR "glGetSamplerParameterfv");
            glGetSamplerParameterIuiv = (PFNGLGETSAMPLERPARAMETERIUIVPROC) GL_GETPROCADDRESS(GL_UBYTE_PTR "glGetSamplerParameterIuiv");
        }

        // FBO support (GL 3.x and above, ignore EXT versions that are older)
        if (strstr(ext, "GL_ARB_framebuffer_object")) {
            glIsRenderbuffer = (PFNGLISRENDERBUFFERPROC)GL_GETPROCADDRESS((GL_UBYTE_PTR "glIsRenderbuffer"));
            glBindRenderbuffer = (PFNGLBINDRENDERBUFFERPROC)GL_GETPROCADDRESS((GL_UBYTE_PTR "glBindRenderbuffer"));
            glDeleteRenderbuffers = (PFNGLDELETERENDERBUFFERSPROC)GL_GETPROCADDRESS((GL_UBYTE_PTR "glDeleteRenderbuffer"));
            glGenRenderbuffers = (PFNGLGENRENDERBUFFERSPROC)GL_GETPROCADDRESS((GL_UBYTE_PTR "glGenRenderbuffers"));
            glRenderbufferStorage = (PFNGLRENDERBUFFERSTORAGEPROC)GL_GETPROCADDRESS((GL_UBYTE_PTR "glRenderbufferStorage"));
            glGetRenderbufferParameteriv = (PFNGLGETRENDERBUFFERPARAMETERIVPROC)GL_GETPROCADDRESS((GL_UBYTE_PTR "glGetRenderbufferParameteriv"));
            glIsFramebuffer = (PFNGLISFRAMEBUFFERPROC)GL_GETPROCADDRESS((GL_UBYTE_PTR "glIsFramebuffer"));
            glBindFramebuffer = (PFNGLBINDFRAMEBUFFERPROC)GL_GETPROCADDRESS((GL_UBYTE_PTR "glBindFramebuffer"));
            glDeleteFramebuffers = (PFNGLDELETEFRAMEBUFFERSPROC)GL_GETPROCADDRESS((GL_UBYTE_PTR "glDeleteFramebuffers"));
            glGenFramebuffers = (PFNGLGENFRAMEBUFFERSPROC)GL_GETPROCADDRESS((GL_UBYTE_PTR "glGenFramebuffers"));
            glCheckFramebufferStatus = (PFNGLCHECKFRAMEBUFFERSTATUSPROC)GL_GETPROCADDRESS((GL_UBYTE_PTR "glCheckFramebufferStatus"));
            glFramebufferTexture1D = (PFNGLFRAMEBUFFERTEXTURE1DPROC)GL_GETPROCADDRESS((GL_UBYTE_PTR "glFramebufferTexture1D"));
            glFramebufferTexture2D = (PFNGLFRAMEBUFFERTEXTURE2DPROC)GL_GETPROCADDRESS((GL_UBYTE_PTR "glFramebufferTexture2D"));
            glFramebufferTexture3D = (PFNGLFRAMEBUFFERTEXTURE3DPROC)GL_GETPROCADDRESS((GL_UBYTE_PTR "glFramebufferTexture3D"));
            glFramebufferRenderbuffer = (PFNGLFRAMEBUFFERRENDERBUFFERPROC)GL_GETPROCADDRESS((GL_UBYTE_PTR "glFramebufferRenderbuffer"));
            glGetFramebufferAttachmentParameteriv = (PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVPROC)GL_GETPROCADDRESS((GL_UBYTE_PTR "glGetFramebufferAttachmentParameteriv"));
            glGenerateMipmap = (PFNGLGENERATEMIPMAPPROC)GL_GETPROCADDRESS((GL_UBYTE_PTR "glGenerateMipmap"));
            glBlitFramebuffer = (PFNGLBLITFRAMEBUFFERPROC)GL_GETPROCADDRESS((GL_UBYTE_PTR "glBlitFramebuffer"));
            glRenderbufferStorageMultisample = (PFNGLRENDERBUFFERSTORAGEMULTISAMPLEPROC)GL_GETPROCADDRESS((GL_UBYTE_PTR "glRenderbufferStorageMultisample"));
            glFramebufferTextureLayer = (PFNGLFRAMEBUFFERTEXTURELAYERPROC)GL_GETPROCADDRESS((GL_UBYTE_PTR "glFramebufferTextureLayer"));
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

void ExtensionManager::genBuffers(GLsizei aCount, GLuint *aBuffers)
{
    if (glGenBuffers) glGenBuffers(aCount, aBuffers);
}

void ExtensionManager::deleteBuffers(GLsizei aCount, GLuint *aBuffers)
{
    if (glDeleteBuffers) glDeleteBuffers(aCount, aBuffers);
}

void ExtensionManager::bindBuffer(GLenum aTarget, GLuint aBuffer)
{
    if (glBindBuffer) glBindBuffer(aTarget, aBuffer);
}

void ExtensionManager::bufferData(GLenum aTarget, GL_SIZEIPTR aSize, const GLvoid* aData, GLenum aUsage)
{
    if (glBufferData) glBufferData(aTarget, aSize, aData, aUsage);
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
    return (GL_TRUE == glIsSampler(sampler));
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
