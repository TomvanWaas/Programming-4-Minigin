#pragma once
#include "BaseComponent.h"
#include "Vector2.h"

class TransformComponent final : public BaseComponent
{
public:
	explicit TransformComponent(GameObject& gameObject);
	virtual ~TransformComponent() override = default;

	TransformComponent(const TransformComponent& other) = delete;
	TransformComponent(TransformComponent&& other) noexcept = delete;
	TransformComponent& operator=(const TransformComponent& other) = delete;
	TransformComponent& operator=(TransformComponent&& other) noexcept = delete;

	void SetWorldPosition(float x, float y);
	void SetWorldPosition(const Vector2& p);
	Vector2 GetWorldPosition() const;
	void SetLocalPosition(float x, float y);
	void SetLocalPosition(const Vector2& p);
	const Vector2& GetLocalPosition() const;

	void SetWorldRotation(float rot, bool isdegrees = true);
	float GetWorldRotation(bool isdegrees = true) const;
	void SetLocalRotation(float rot, bool isdegrees = true);
	float GetLocalRotation(bool isdegrees = true) const;

	void SetWorldScale(Vector2 s);
	void SetWorldScale(float x, float y);
	Vector2 GetWorldScale() const;
	void SetLocalScale(const Vector2& s);
	void SetLocalScale(float x, float y);
	const Vector2& GetLocalScale() const;

private:
	//Local
	Vector2 m_LocalPosition;
	Vector2 m_LocalScale;
	float m_LocalRotationDegrees;


};

