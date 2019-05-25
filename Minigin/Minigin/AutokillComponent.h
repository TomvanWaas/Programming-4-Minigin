#pragma once
#include "BaseComponent.h"

namespace DigDug
{
	class AutokillComponent final : public Minigin::BaseComponent
	{
	public:
		explicit AutokillComponent(float delay) : m_Delay(delay) {}
		virtual ~AutokillComponent() = default;
		virtual void UpdateFirstOverride(const Minigin::SceneData& sceneData) override;
	private:
		float m_Delay;
	};
}
