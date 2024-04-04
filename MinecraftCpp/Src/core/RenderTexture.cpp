#include "RenderTexture.h"
#include <iostream>
#include "Shader.h"
#include "VBO.h"
#include "VAO.h"
#include "Texture.h"
#include "../Properties.h"
#include "Engine.h"

static VAO *vao = NULL;
static VBO *vbo = NULL;
static Shader* shader = NULL;

static std::vector<glm::vec4> vertices{
    glm::vec4(1,1,1,1),
    glm::vec4(0,1,0,1),
    glm::vec4(1,0,1,0),

    glm::vec4(1,0,1,0),
    glm::vec4(0,1,0,1),
    glm::vec4(0,0,0,0)

};

void RenderTexture::setUpRenderTextures()
{
    shader = new Shader("Shader/texture.vert", "Shader/texture.frag");
    vao = new VAO();
    vbo = new VBO();
    vao->bind();
    vbo->setNewVertices(vertices);
    vao->linkData(*vbo, 0, 2, GL_FLOAT, sizeof(glm::vec4), (void*)0);
    vao->linkData(*vbo, 1, 2, GL_FLOAT, sizeof(glm::vec4), (void*)sizeof(glm::vec2));
}

void RenderTexture::endRenderTexture()
{
    if (vao)
        delete vao;
    if (vbo)
        delete vbo;
    if (shader)
        delete shader;
    vao = NULL;
    vbo = NULL;
    shader = NULL;
}

RenderTexture::RenderTexture(int w, int h)
{
    this->w = w;
    this->h = h;
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    // create a color attachment texture
    glGenTextures(1, &texture);
    slot = Texture::textureSlot++;
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);


    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, w, h); 
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
#ifdef DebugErrorMode
        printf("[ERROR]: Framebuffer is not complete! %d\n");
#endif
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

RenderTexture::~RenderTexture()
{
    glDeleteTextures(1, &texture);
    glDeleteRenderbuffers(1, &rbo);
    glDeleteFramebuffers(1, &fbo);
}

void RenderTexture::startUse()
{
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glViewport(0, 0, w, h);
    glClearColor(0.0f, 0.1f, 0.1f, 1.0f);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void RenderTexture::endUse()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    diffViewport();
}

void RenderTexture::use(Shader& s, const char* uniform)
{
    s.active();
    glActiveTexture(GL_TEXTURE0 + slot);
    s.setUniformI1(slot, uniform);
    glBindTexture(GL_TEXTURE_2D, texture);
    //glUniform1i(s.getUniformLocation(uniform), 0);


}

void RenderTexture::draw()
{
    shader->active();
    shader->setUniformVec3(glm::vec3(0.0f, 0.1f, 0.1f), "backGroundColor");
    vao->bind();
    use(*shader, "text0");

    glDisable(GL_DEPTH_TEST);

    glDrawArrays(GL_TRIANGLES, 0, 6);
    glEnable(GL_DEPTH_TEST);
}