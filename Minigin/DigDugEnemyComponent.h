#pragma once
#include "BaseComponent.h"
class DigDugEnemyComponent : public BaseComponent
{
public:
	explicit DigDugEnemyComponent() = default;
	virtual ~DigDugEnemyComponent() = default;
	virtual void InitializeOverride(const SceneData& sceneData) override;
	virtual void DestroyOverride(const SceneData& sceneData) override;
private:
};

