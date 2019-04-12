#pragma once
#include "BaseComponent.h"
#include "Vector2.h"

class MovementComponent final : public BaseComponent
{
public:
	explicit MovementComponent(unsigned int interpolation);
	virtual ~MovementComponent() = default;

	virtual void Initialize(const SceneData& sceneData) override;
	virtual void Update(const SceneData& sceneData) override;

	void MovePosition(const Vector2& movement);

private:
	Vector2 m_Movement;
	bool m_HasMoved;
	unsigned int m_Interpolation;


};

