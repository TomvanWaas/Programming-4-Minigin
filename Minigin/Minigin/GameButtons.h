#pragma once
#include "Button.h"
namespace Minigin
{
	class Scene;
	class SceneManager;
}
namespace DigDug
{
	class SceneSwapButton final : public ButtonAction
	{
	public:
		explicit SceneSwapButton(Minigin::SceneManager& sceneManager, const std::string& name, bool reload = false);
		explicit SceneSwapButton(Minigin::SceneManager& sceneManager, Minigin::Scene& scene, bool reload = false);
		virtual ~SceneSwapButton() = default;
		virtual void Execute() override;
		void SetSceneName(const std::string& name) { m_SceneName = name; }
		const std::string& GetName() const { return m_SceneName; }
	private:
		std::string m_SceneName;
		Minigin::SceneManager* m_pSceneManager;
		bool m_Reload;
	};


	class QuitButton final : public ButtonAction
	{
	public:
		explicit QuitButton() = default;
		virtual ~QuitButton() = default;
		virtual void Execute() override;
	};










}


