#pragma once
#include "BaseComponent.h"

namespace DigDug
{
	class DigDugEnemyComponent : public Minigin::BaseComponent
	{
	public:
		explicit DigDugEnemyComponent(int id) : m_Id(id) {}
		virtual ~DigDugEnemyComponent() = default;
		virtual void InitializeOverride(const Minigin::SceneData& sceneData) override;
		virtual void DestroyOverride(const Minigin::SceneData& sceneData) override;
	private:
		int m_Id;
	};

}