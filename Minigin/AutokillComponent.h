#pragma once
#include "BaseComponent.h"

class AutokillComponent final : public BaseComponent
{
public:
	explicit AutokillComponent(float delay) : m_Delay(delay) {}
	virtual ~AutokillComponent() = default;
	virtual void UpdateFirstOverride(const SceneData& sceneData) override;
private:
	float m_Delay;
};

