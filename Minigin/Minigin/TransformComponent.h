#pragma once
#include "BaseComponent.h"

#pragma warning (push)	// Give warnings: Nameless union/struct
#pragma warning (disable:4201)
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#pragma warning (pop)

#include "Vector2.h"

class TransformComponent final : public BaseComponent
{
public:
	explicit TransformComponent();
	virtual ~TransformComponent() override = default;

	TransformComponent(const TransformComponent& other) = delete;
	TransformComponent(TransformComponent&& other) noexcept = delete;
	TransformComponent& operator=(const TransformComponent& other) = delete;
	TransformComponent& operator=(TransformComponent&& other) noexcept = delete;

	virtual void Initialize(const SceneData& sceneData) override;
	virtual void ReInitialize(const SceneData& sceneData) override;



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
	Vector2 m_LocalPosition;
	Vector2 m_LocalScale;
	float m_LocalRotationDegrees;

	TransformComponent* m_pParent;
};

