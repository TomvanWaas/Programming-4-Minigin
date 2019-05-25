#pragma once
#include "Scene.h"

namespace DigDug
{
	class SceneSwapButton;
	class DigDugPauseScene final : public Minigin::Scene
	{
	public:
		explicit DigDugPauseScene(const std::string& name);
		virtual ~DigDugPauseScene() = default;
		virtual void SceneInitialize() override;
		virtual Minigin::Scene* OnReload() const override;
		virtual void Enter(Minigin::Scene* pPreviousScene) override;
	private:
		std::shared_ptr<SceneSwapButton> m_pResumeButton;
	};

}
