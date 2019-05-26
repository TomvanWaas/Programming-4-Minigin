#pragma once
#include "Color.h"
#include "BaseComponent.h"
namespace Engine
{
	class Font;
	class Scene;
}
namespace DigDug
{
	class ScoreObserver final : public Engine::BaseComponent
	{
	public:
		explicit ScoreObserver(const std::shared_ptr<Engine::Font>& pFont, const Engine::Color4& color, Engine::Scene* pScene);
		virtual ~ScoreObserver() = default;

		virtual void Notify(Engine::ObservedEvent event, const Engine::ObservedData& data) override;
	private:
		int m_Score = 0;

		Engine::Color4 m_Color;
		std::shared_ptr<Engine::Font> m_pFont;
		Engine::Scene* m_pScene;

	};

}