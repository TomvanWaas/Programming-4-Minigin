#pragma once
#include "Application.h"

class DigDugApplication final : public Application
{
public:
	DigDugApplication() = default;
	virtual ~DigDugApplication() = default;

	virtual void Initialize(SceneManager& sceneManager) override;
};

