#pragma once
#include "BaseComponent.h"

class DigDugPlayerComponent : public BaseComponent
{
public:
	explicit DigDugPlayerComponent(int id = 0);
	virtual ~DigDugPlayerComponent() = default;
	virtual void InitializeOverride(const SceneData& sceneData) override;
	virtual void DestroyOverride(const SceneData& sceneData) override;
	int GetID() const { return m_Id; }
private:
	int m_Id;
};

