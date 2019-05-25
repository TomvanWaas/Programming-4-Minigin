#pragma once
#include "Color.h"
#include "BaseComponent.h"
namespace Minigin
{
	class Font;
	class Scene;
}
namespace DigDug
{
	class ScoreObserver final : public Minigin::BaseComponent
	{
	public:
		explicit ScoreObserver(const std::shared_ptr<Minigin::Font>& pFont, const Minigin::Color4& color, Minigin::Scene* pScene);
		virtual ~ScoreObserver() = default;

		virtual void Notify(Minigin::ObservedEvent event, const Minigin::ObservedData& data) override;
	private:
		int m_Score = 0;

		Minigin::Color4 m_Color;
		std::shared_ptr<Minigin::Font> m_pFont;
		Minigin::Scene* m_pScene;

	};

}