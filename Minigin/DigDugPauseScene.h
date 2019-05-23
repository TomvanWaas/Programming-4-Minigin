#pragma once
#include "Scene.h"

namespace DigDug
{
	class SceneSwapButton;
	class DigDugPauseScene final : public Scene
	{
	public:
		explicit DigDugPauseScene(const std::string& name);
		virtual ~DigDugPauseScene() = default;
		virtual void SceneInitialize() override;
		virtual Scene* OnReload() const override;
		virtual void Enter(Scene* pPreviousScene) override;
	private:
		std::shared_ptr<SceneSwapButton> m_pResumeButton;
	};

}
