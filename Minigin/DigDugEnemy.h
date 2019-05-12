#pragma once
#include "BaseComponent.h"

class DigDugEnemy final : public BaseComponent
{
public:
	explicit DigDugEnemy() = default;
	virtual ~DigDugEnemy() = default;

	virtual void InitializeOverride(const SceneData& sceneData) override;
	virtual void DestroyOverride(const SceneData& sceneData) override;
};

