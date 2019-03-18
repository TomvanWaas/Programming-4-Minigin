#pragma once
#include "BaseComponent.h"
class TextComponent;
class FPSComponent final : public BaseComponent
{
public:
	explicit FPSComponent();
	virtual ~FPSComponent() override = default;

	FPSComponent(const FPSComponent& other) = delete;
	FPSComponent(FPSComponent&& other) noexcept = delete;
	FPSComponent& operator=(const FPSComponent& other) = delete;
	FPSComponent& operator=(FPSComponent&& other) noexcept = delete;

	virtual void Initialize() override;
	virtual void Update() override;

	void SetPrecision(unsigned int prec);
	unsigned int GetPrecision() const;

private:
	TextComponent* m_pTextComponent;
	unsigned int m_Precision;
};

