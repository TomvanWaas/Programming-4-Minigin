#pragma once
#include "BaseComponent.h"

namespace DigDug
{
	class DigDugEnemyComponent : public Engine::BaseComponent
	{
	public:
		explicit DigDugEnemyComponent(int id) : m_Id(id) {}
		virtual ~DigDugEnemyComponent() = default;
		virtual void InitializeOverride(const Engine::SceneData& sceneData) override;
		virtual void DestroyOverride(const Engine::SceneData& sceneData) override;
	private:
		int m_Id;
	};

}