#pragma once
#include "Scene.h"

namespace DigDug
{
	class DigDugMenuScene final : public Engine::Scene
	{
	public:
		explicit DigDugMenuScene(const std::string& name);
		virtual ~DigDugMenuScene() = default;

		virtual void SceneInitialize() override;
		virtual Engine::Scene* OnReload() const override;
	private:


	};

}