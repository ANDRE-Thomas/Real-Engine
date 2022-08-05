#include "Components/Transform.h"

Transform::Transform() {}

Transform::Transform(vec3 position)
{
	this->position = position;
}

Transform::Transform(vec3 position, quat rotation)
{
	this->position = position;
	this->rotation = rotation;
}

Transform::Transform(vec3 position, quat rotation, vec3 scale)
{
	this->position = position;
	this->rotation = rotation;
	this->scale = scale;
}

Transform::Transform(mat4 matrix4x4)
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

vec3 Transform::GetPosition()
{
	return position;
}

vec3 Transform::GetScale()
{
	return scale;
}

quat Transform::GetRotation()
{
	return rotation;
}

void Transform::SetPosition(vec3 position)
{
	this->position = position;
}

void Transform::SetScale(vec3 scale)
{
	this->scale = scale;
}

void Transform::SetRotation(quat quaternion)
{
	this->rotation = rotation;
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
	SetRotation(quat_cast(rotate(mat4_cast(GetRotation()), angle, axis)));
}

mat4 Transform::GetMatrix4x4()
{
	mat4 translation = translate(mat4(1), position);
	mat4 rotation = mat4_cast(this->rotation);
	mat4 scale = glm::scale(this->scale);

	return translation * rotation * scale;
}