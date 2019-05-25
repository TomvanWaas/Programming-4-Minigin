#pragma once
#include "BaseComponent.h"

namespace DigDug
{
	class DigDugPlayerComponent : public Minigin::BaseComponent
	{
	public:
		explicit DigDugPlayerComponent(int id = 0);
		virtual ~DigDugPlayerComponent() = default;
		virtual void InitializeOverride(const Minigin::SceneData& sceneData) override;
		virtual void DestroyOverride(const Minigin::SceneData& sceneData) override;
		int GetID() const { return m_Id; }
	private:
		int m_Id;
	};

}