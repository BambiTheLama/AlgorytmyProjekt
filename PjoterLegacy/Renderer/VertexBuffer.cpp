#include "VertexBuffer.h"

#include "renderer.h"

VertexBuffer::VertexBuffer() { m_RendererID = 0; }

VertexBuffer::VertexBuffer(const void* data, unsigned int size)
{
    m_RendererID = 0;
    glGenBuffers(1, &m_RendererID);
    glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

VertexBuffer::~VertexBuffer()
{
    glDeleteBuffers(1, &m_RendererID);
}

void VertexBuffer::Set(const void* data, unsigned int size)
{
    glDeleteBuffers(1, &m_RendererID);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glGenBuffers(1, &m_RendererID);
    glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW);
}

void VertexBuffer::Bind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
}

void VertexBuffer::Unbind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
