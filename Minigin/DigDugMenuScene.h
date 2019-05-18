#pragma once
#include "Scene.h"
class DigDugMenuScene final : public Scene
{
public:
	explicit DigDugMenuScene(const std::string& name);
	virtual ~DigDugMenuScene() = default;

	virtual void SceneInitialize() override;
	virtual Scene* GetNew() const override;
private:


};

