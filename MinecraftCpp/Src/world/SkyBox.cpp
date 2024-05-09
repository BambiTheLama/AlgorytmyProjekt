#include "SkyBox.h"
#include "../core/VAO.h"
#include "../core/VBO.h"
#include "../core/EBO.h"
#include "../core/Shader.h"
#include "../core/Camera.h"
#include <vector>
#include <string>
#include <stb/stb_image.h>
#include "../Properties.h"

static std::vector<glm::vec3> vertices{

	glm::vec3(-1.0f, -1.0f,  1.0f),
	glm::vec3( 1.0f, -1.0f,  1.0f),
	glm::vec3( 1.0f, -1.0f, -1.0f),
	glm::vec3(-1.0f, -1.0f, -1.0f),
	glm::vec3(-1.0f,  1.0f,  1.0f),
	glm::vec3( 1.0f,  1.0f,  1.0f),
	glm::vec3( 1.0f,  1.0f, -1.0f),
	glm::vec3(-1.0f,  1.0f, -1.0f)
};
static std::vector<GLuint> indices{

	2, 1, 6,
	6, 1, 5,
	0, 7, 4,
	7, 0, 3,
	4, 6, 5,
	6, 4, 7,
	0, 2, 3,
	2, 0, 1,
	0, 5, 1,
	5, 0, 4,
	3, 6, 7,
	6, 3, 2
};

SkyBox::SkyBox(const char* path)
{

	shader = new Shader("Shader/SkyBox.vert", "Shader/SkyBox.frag");
	vao = new VAO();
	ebo = new EBO();
	vbo = new VBO();
	vao->bind();
	vbo->setNewVertices(vertices);
	vao->linkData(*vbo, 0, 3, GL_FLOAT, sizeof(glm::vec3), NULL);
	ebo->setNewIndices(indices);
	ebo->bind();
	vao->unbind();
	ebo->unbind();

	glGenTextures(1, &cubeTexture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubeTexture);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	std::string textNames[6] = { "x+","x-","y+","y-","z+","z-" };

	for (int i = 0; i < 6; i++)
	{
		int w, h, chanels;
		std::string texturePath = path + textNames[i] + ".png";
		unsigned char* bytes = stbi_load(texturePath.c_str(), &w, &h, &chanels, 0);
		if (!bytes)
		{
#ifdef DebugFailMode
#define info
			printf("[FAIL]: No texture loaded [%s]\n", texturePath.c_str());
#endif
			continue;
		}

		
		stbi_set_flip_vertically_on_load(false);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, bytes);
		stbi_image_free(bytes);
		


	}

}

SkyBox::~SkyBox()
{
	delete vao;
	delete vbo;
	delete shader;
	delete ebo;
	glDeleteTextures(1, &cubeTexture);

}

void SkyBox::draw(Camera* camera)
{
	glDisable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	
	shader->active();
	camera->useCamera(*shader, "camera");
	shader->setUniformVec3(camera->getPos(), "cameraPos");
	vao->bind();
	ebo->bind();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubeTexture);
	glDrawElements(GL_TRIANGLES, indices.size() , GL_UNSIGNED_INT, 0);
	glDepthFunc(GL_LESS);
	glEnable(GL_DEPTH_TEST);
}
