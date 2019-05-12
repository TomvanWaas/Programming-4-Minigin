#pragma once
#include "BaseComponent.h"

class DigDugPlayer final : public BaseComponent
{
public:
	explicit DigDugPlayer() = default;
	virtual ~DigDugPlayer() = default;

	virtual void InitializeOverride(const SceneData& sceneData) override;
	virtual void DestroyOverride(const SceneData& sceneData) override;
};

