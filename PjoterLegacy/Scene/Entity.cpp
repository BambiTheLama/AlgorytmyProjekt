#include "Entity.h"
#include "SceneManager.h"

Entity::Entity() {
	this->objectID = ID++;
	this->name = "";
}

Entity::~Entity()
{
	delete mesh;
}

size_t Entity::GetID() const { return objectID; }

void Entity::SetParentSceneManager(SceneManager* parentSceneManager)
{
	this->parentSceneManager = parentSceneManager;
}

SceneManager* Entity::GetSceneManager() const
{
	return this->parentSceneManager;
}

glm::vec3 Entity::GetScale()
{
	glm::vec3 scale;

	for (int i = 0; i < 3; i++)
		scale[i] = glm::length(glm::vec3(modelMatrix[i]));
	return scale;
}

glm::quat Entity::GetRotation()
{
	glm::vec3 scale;
	glm::quat rotation;

	for (int i = 0; i < 3; i++)
		scale[i] = glm::length(glm::vec3(modelMatrix[i]));
	const glm::mat3 rotMtx(
		glm::vec3(modelMatrix[0]) / scale[0],
		glm::vec3(modelMatrix[1]) / scale[1],
		glm::vec3(modelMatrix[2]) / scale[2]);
	rotation = glm::quat_cast(rotMtx);

	return rotation;
}

glm::vec3 Entity::GetTranslation()
{
	glm::vec3 translation;

	translation = modelMatrix[3];

	return translation;
}

void Entity::Translate(glm::vec3 translateVec)
{
	modelMatrix = glm::translate(modelMatrix, translateVec);
}

void Entity::Rotate(glm::vec3 rotateVec, float angle)
{
	modelMatrix = glm::rotate(modelMatrix, glm::radians(angle), rotateVec);
}

void Entity::Scale(glm::vec3 scaleVec)
{
	modelMatrix = glm::scale(modelMatrix, scaleVec);
}

glm::mat4 Entity::GetModelMatrix()
{
	return modelMatrix;
}

void Entity::SetShader(Shader* shader)
{
	this->shader = shader;
}

Shader* Entity::GetShader()
{
	return shader;
}

Mesh* Entity::GetMesh() const
{
	return mesh;
}

void Entity::SetMesh(Mesh* mesh)
{
	this->mesh = mesh;
}

size_t Entity::ID = 0;

