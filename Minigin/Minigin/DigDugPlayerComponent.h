#pragma once
#include "BaseComponent.h"

namespace DigDug
{
	class DigDugPlayerComponent : public Engine::BaseComponent
	{
	public:
		explicit DigDugPlayerComponent(int id = 0);
		virtual ~DigDugPlayerComponent() = default;
		virtual void InitializeOverride(const Engine::SceneData& sceneData) override;
		virtual void DestroyOverride(const Engine::SceneData& sceneData) override;
		int GetID() const { return m_Id; }
	private:
		int m_Id;
	};

}