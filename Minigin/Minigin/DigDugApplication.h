#pragma once
#include "Application.h"


class DigDugApplication final : public Engine::Application
{
public:
	DigDugApplication() = default;
	virtual ~DigDugApplication() = default;

	virtual void Initialize(Engine::SceneManager& sceneManager, const Engine::WindowSettings& windowSettings) override;


};

