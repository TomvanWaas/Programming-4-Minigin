#include "MiniginPCH.h"
#include "TransformComponent.h"


void TransformComponent::SetPosition(float x, float y, float z)
{
	m_Position.x = x;
	m_Position.y = y;
	m_Position.z = z;
}

void TransformComponent::SetEulerRotation(float x, float y, float z)
{
	SetEulerRotation(glm::vec3{ x, y, z });
}

void TransformComponent::SetQuaternionRotation(float x, float y, float z, float w)
{
	m_Rotation.x = x;
	m_Rotation.y = y;
	m_Rotation.z = z;
	m_Rotation.w = w;
}

void TransformComponent::SetScale(float x, float y, float z)
{
	m_Scale.x = x;
	m_Scale.y = y;
	m_Scale.z = z;
}

void TransformComponent::SetPosition(const glm::vec3& pos)
{
	m_Position = pos;
}

void TransformComponent::SetEulerRotation(const glm::vec3& rot)
{
	SetQuaternionRotation(ToQuaternion(rot));
}

void TransformComponent::SetQuaternionRotation(const glm::vec4& rot)
{
	m_Rotation = rot;
}

void TransformComponent::SetScale(const glm::vec3& sca)
{
	m_Scale = sca;
}

const glm::vec3& TransformComponent::GetPosition() const
{
	return m_Position;
}

glm::vec3 TransformComponent::GetEulerRotation() const
{
	return ToEuler(GetQuaternionRotation());
}

const glm::vec4& TransformComponent::GetQuaternionRotation() const
{
	return m_Rotation;
}

const glm::vec3& TransformComponent::GetScale() const
{
	return m_Scale;
}

 glm::vec3 TransformComponent::ToEuler(const glm::vec4& quaternion) const
{
	//Definition found on wikipedia: 'https://en.wikipedia.org/wiki/Conversion_between_quaternions_and_Euler_angles'

	glm::vec3 euler{};
	euler.x = atan(2 * (quaternion.x*quaternion.y + quaternion.z*quaternion.w) / (1 - 2 * (quaternion.y*quaternion.y + quaternion.z*quaternion.z)));
	euler.y = asin(2 * (quaternion.x*quaternion.z - quaternion.y*quaternion.w));
	euler.z = atan(2 * (quaternion.x*quaternion.z + quaternion.y*quaternion.w) / (1 - 2 * (quaternion.z*quaternion.z + quaternion.w*quaternion.w)));
	return euler;
}
glm::vec4 TransformComponent::ToQuaternion(const glm::vec3& euler) const
{
	//Definition found on wikipedia: 'https://en.wikipedia.org/wiki/Conversion_between_quaternions_and_Euler_angles'
	
	glm::vec4 quaternion{};
	float cy = cos(euler.z*0.5f);
	float sy = sin(euler.z*0.5f);
	float cp = cos(euler.y*0.5f);
	float sp = sin(euler.y*0.5f);
	float cr = cos(euler.x*0.5f);
	float sr = sin(euler.x*0.5f);
	quaternion.w = cy * cp * cr + sy * sp * sr;
	quaternion.x = cy * cp * sr - sy * sp * cr;
	quaternion.y = sy * cp * sr + cy * sp * cr;
	quaternion.z = sy * cp * cr - cy * sp * sr;
	return quaternion;
}
