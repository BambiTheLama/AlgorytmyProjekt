#include "VertexArray.h"

#include "renderer.h"
#include "VertexBufferLayout.h"

VertexArray::VertexArray()
{
	GlCall(glGenVertexArrays(1, &m_RendererID));
}

VertexArray::~VertexArray()
{
	GlCall(glDeleteVertexArrays(1, &m_RendererID));
}

void VertexArray::Bind() const
{
	GlCall(glBindVertexArray(m_RendererID));
}

void VertexArray::Unbind() const
{
	GlCall(glBindVertexArray(0));
}

void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
{
	Bind();
	vb.Bind();
	const auto& elements = layout.GetElements();

	unsigned int offset = 0;
	for (unsigned int i = 0; i < elements.size(); i++)
	{
		const auto& element = elements[i];
		glEnableVertexAttribArray(i);
		glVertexAttribPointer(i, element.count, element.type, element.normalized, 
			layout.GetStride(), (const void*)offset);

		offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
	}
}
