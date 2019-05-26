#pragma once
#include "BaseComponent.h"

namespace DigDug
{
	class AutokillComponent final : public Engine::BaseComponent
	{
	public:
		explicit AutokillComponent(float delay) : m_Delay(delay) {}
		virtual ~AutokillComponent() = default;
		virtual void UpdateFirstOverride(const Engine::SceneData& sceneData) override;
	private:
		float m_Delay;
	};
}
