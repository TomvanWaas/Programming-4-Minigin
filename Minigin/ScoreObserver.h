#pragma once
#include "Observer.h"
#include "Color.h"
class Font;
class Scene;
class ScoreObserver final : public Observer
{
public:
	explicit ScoreObserver(const std::shared_ptr<Font>& pFont, const Color4& color, Scene* pScene);
	virtual ~ScoreObserver() = default;

	virtual void Notify(ObservedEvent event, const ObservedData& data) override;
private:
	int m_Score = 0;

	Color4 m_Color;
	std::shared_ptr<Font> m_pFont;
	Scene* m_pScene;

};

