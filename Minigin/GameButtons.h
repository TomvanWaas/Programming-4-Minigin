#pragma once
#include "Button.h"
class Scene;
class SceneManager;

namespace DigDug
{
	class SceneSwapButton final : public ButtonAction
	{
	public:
		explicit SceneSwapButton(SceneManager& sceneManager, const std::string& name);
		explicit SceneSwapButton(SceneManager& sceneManager, Scene& scene);
		virtual ~SceneSwapButton() = default;
		virtual void Execute() override;
	private:
		std::string m_SceneName;
		SceneManager* m_pSceneManager;
	};


	class QuitButton final : public ButtonAction
	{
	public:
		explicit QuitButton() = default;
		virtual ~QuitButton() = default;
		virtual void Execute() override;
	};










}


