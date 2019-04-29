#pragma once
#include "BaseComponent.h"
#include "Vector2.h"

class AABBCollisionComponent;
class MovementComponent final : public BaseComponent
{
public:
	explicit MovementComponent(GameObject& gameObject);
	virtual ~MovementComponent() = default;

	virtual void Initialize(const SceneData& sceneData) override;
	virtual void UpdateSecond(const SceneData& sceneData) override;

	void QueueMovement(const Vector2& movement);

	void SetInterpolation(unsigned int interpolation);
	unsigned int GetInterpolation() const;


private:
	Vector2 m_MovementQueue;
	bool m_HasQueue;
	unsigned int m_Interpolation;



	std::vector<AABBCollisionComponent*> m_pColliders;

};

