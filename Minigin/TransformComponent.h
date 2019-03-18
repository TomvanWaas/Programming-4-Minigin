#pragma once
#include "BaseComponent.h"

#pragma warning (push)	// Give warnings: Nameless union/struct
#pragma warning (disable:4201)
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#pragma warning (pop)

class TransformComponent final : public BaseComponent
{
public:
	explicit TransformComponent() = default;
	virtual ~TransformComponent() override = default;

	TransformComponent(const TransformComponent& other) = delete;
	TransformComponent(TransformComponent&& other) noexcept = delete;
	TransformComponent& operator=(const TransformComponent& other) = delete;
	TransformComponent& operator=(TransformComponent&& other) noexcept = delete;

	virtual void Update() override {}
	virtual void Render() const override {}

	void SetPosition(float x, float y, float z);
	void SetEulerRotation(float x, float y, float z);
	void SetQuaternionRotation(float x, float y, float z, float w);
	void SetScale(float x, float y, float z);
	void SetPosition(const glm::vec3& pos);
	void SetEulerRotation(const glm::vec3& rot);
	void SetQuaternionRotation(const glm::vec4& rot);
	void SetScale(const glm::vec3& sca);
	const glm::vec3& GetPosition() const;
	glm::vec3 GetEulerRotation() const;
	const glm::vec4& GetQuaternionRotation() const;
	const glm::vec3& GetScale() const;

private:
	glm::vec3 m_Position;
	glm::vec4 m_Rotation;
	glm::vec3 m_Scale;

	glm::vec3 ToEuler(const glm::vec4& quaternion) const;
	glm::vec4 ToQuaternion(const glm::vec3& euler) const;

};

