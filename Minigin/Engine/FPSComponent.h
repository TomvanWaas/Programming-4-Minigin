#pragma once
#include "BaseComponent.h"

namespace Engine
{
	class TextComponent;
	class FPSComponent final : public BaseComponent
	{
	public:
		explicit FPSComponent(unsigned int prec = 2);
		virtual ~FPSComponent() override = default;

		FPSComponent(const FPSComponent& other) = delete;
		FPSComponent(FPSComponent&& other) noexcept = delete;
		FPSComponent& operator=(const FPSComponent& other) = delete;
		FPSComponent& operator=(FPSComponent&& other) noexcept = delete;

		virtual void InitializeOverride(const SceneData& sceneData) override;
		virtual void UpdateFirstOverride(const SceneData& sceneData) override;

		void SetPrecision(unsigned int prec);
		unsigned int GetPrecision() const;

	private:
		unsigned int m_Precision;

		TextComponent* m_pTextComponent;
	};

}