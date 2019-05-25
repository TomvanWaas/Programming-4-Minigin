#pragma once
#include "Application.h"


class DigDugApplication final : public Minigin::Application
{
public:
	DigDugApplication() = default;
	virtual ~DigDugApplication() = default;

	virtual void Initialize(Minigin::SceneManager& sceneManager, const Minigin::WindowSettings& windowSettings) override;


};

