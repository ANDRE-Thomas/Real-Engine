#include "Components/Transform.h"

Transform::Transform() : Component()
{
	this->position = vec3(0);
	this->rotation = quat_identity<f32, highp>();
	this->scale = vec3(1);
}

Transform::Transform(vec3 position) : Component()
{
	this->position = position;
	this->rotation = quat_identity<f32, highp>();
	this->scale = vec3(1);
}

Transform::Transform(vec3 position, quat rotation) : Component()
{
	this->position = position;
	this->rotation = rotation;
	this->scale = vec3(1);
}

Transform::Transform(vec3 position, quat rotation, vec3 scale) : Component()
{
	this->position = position;
	this->rotation = rotation;
	this->scale = scale;
}

Transform::Transform(mat4 matrix4x4) : Component()
{
	position = vec3(matrix4x4[3][0], matrix4x4[3][1], matrix4x4[3][2]);

	float sX = sqrt(matrix4x4[0][0] * matrix4x4[0][0] + matrix4x4[0][1] * matrix4x4[0][1] + matrix4x4[0][2] * matrix4x4[0][2]);
	float sY = sqrt(matrix4x4[1][0] * matrix4x4[1][0] + matrix4x4[1][1] * matrix4x4[1][1] + matrix4x4[1][2] * matrix4x4[1][2]);
	float sZ = sqrt(matrix4x4[2][0] * matrix4x4[2][0] + matrix4x4[2][1] * matrix4x4[2][1] + matrix4x4[2][2] * matrix4x4[2][2]);

	scale = vec3(sX, sY, sZ);

	mat4 rotationMatrix = mat4(1);

	rotationMatrix[0][0] = scale.x == 0 ? 0 : matrix4x4[0][0] / scale.x;
	rotationMatrix[0][1] = scale.x == 0 ? 0 : matrix4x4[0][1] / scale.x;
	rotationMatrix[0][2] = scale.x == 0 ? 0 : matrix4x4[0][2] / scale.x;

	rotationMatrix[1][0] = scale.y == 0 ? 0 : matrix4x4[1][0] / scale.y;
	rotationMatrix[1][1] = scale.y == 0 ? 0 : matrix4x4[1][1] / scale.y;
	rotationMatrix[1][2] = scale.y == 0 ? 0 : matrix4x4[1][2] / scale.y;

	rotationMatrix[2][0] = scale.z == 0 ? 0 : matrix4x4[2][0] / scale.z;
	rotationMatrix[2][1] = scale.z == 0 ? 0 : matrix4x4[2][1] / scale.z;
	rotationMatrix[2][2] = scale.z == 0 ? 0 : matrix4x4[2][2] / scale.z;

	rotation = quat_cast(rotationMatrix);
}

vec3 Transform::Forward()
{
	return rotation * vec3(0, 0, -1);
}

vec3 Transform::Up()
{
	return rotation * vec3(0, 1, 0);
}

vec3 Transform::Right()
{
	return rotation * vec3(1, 0, 0);
}

void Transform::Translate(vec3 translation)
{
	position += translation;
}

void Transform::Scale(vec3 scalingFactor)
{
	scale *= scalingFactor;
}

void Transform::Rotate(quat rotation)
{
	this->rotation = rotation * this->rotation;
}

void Transform::RotateAxis(vec3 axis, float angle)
{
	this->rotation = quat_cast(rotate(radians(angle), axis)) * this->rotation;
}

mat4 Transform::GetMatrix4x4()
{
	mat4 translation = translate(mat4(1), position);
	mat4 rotation = glm::mat4_cast(this->rotation);
	mat4 scale = glm::scale(this->scale);

	return translation * rotation * scale;
}