#pragma once
#include "Vector2.h"
class GameObject;
class Transform final
{
public:
	explicit Transform(GameObject& gameObject);
	~Transform() = default;

	Transform(const Transform& other) = delete;
	Transform(Transform&& other) noexcept = delete;
	Transform& operator=(const Transform& other) = delete;
	Transform& operator=(Transform&& other) noexcept = delete;

	const GameObject* GetGameObject() const;

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
	GameObject* m_pGameObject;
	Vector2 m_LocalPosition;
	Vector2 m_LocalScale;
	float m_LocalRotationDegrees;


};

