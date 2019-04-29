#pragma once
#include "BaseComponent.h"
class TextComponent;
class FPSComponent final : public BaseComponent
{
public:
	explicit FPSComponent(GameObject& gameObject);
	virtual ~FPSComponent() override = default;

	FPSComponent(const FPSComponent& other) = delete;
	FPSComponent(FPSComponent&& other) noexcept = delete;
	FPSComponent& operator=(const FPSComponent& other) = delete;
	FPSComponent& operator=(FPSComponent&& other) noexcept = delete;

	virtual void Initialize(const SceneData& sceneData) override;
	virtual void UpdateFirst(const SceneData& sceneData) override;

	void SetPrecision(unsigned int prec);
	unsigned int GetPrecision() const;

private:
	unsigned int m_Precision;

	TextComponent* m_pTextComponent;
};

