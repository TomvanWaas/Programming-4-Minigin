#pragma once
#include "Scene.h"

namespace DigDug
{
	class SceneSwapButton;
	class DigDugPauseScene final : public Engine::Scene
	{
	public:
		explicit DigDugPauseScene(const std::string& name);
		virtual ~DigDugPauseScene() = default;
		virtual void SceneInitialize() override;
		virtual Engine::Scene* OnReload() const override;
		virtual void Enter(Engine::Scene* pPreviousScene) override;
	private:
		std::shared_ptr<SceneSwapButton> m_pResumeButton;
	};

}
