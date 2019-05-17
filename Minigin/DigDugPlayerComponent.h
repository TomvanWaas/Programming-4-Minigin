#pragma once
#include "BaseComponent.h"

class DigDugPlayerComponent : public BaseComponent
{
public:
	explicit DigDugPlayerComponent() = default;
	virtual ~DigDugPlayerComponent() = default;
	virtual void InitializeOverride(const SceneData& sceneData) override;
	virtual void DestroyOverride(const SceneData& sceneData) override;
private:
};

