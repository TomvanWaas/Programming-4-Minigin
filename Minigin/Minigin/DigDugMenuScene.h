#pragma once
#include "Scene.h"

namespace DigDug
{
	class DigDugMenuScene final : public Minigin::Scene
	{
	public:
		explicit DigDugMenuScene(const std::string& name);
		virtual ~DigDugMenuScene() = default;

		virtual void SceneInitialize() override;
		virtual Minigin::Scene* OnReload() const override;
	private:


	};

}