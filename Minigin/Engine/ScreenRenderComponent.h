#pragma once
#include "Renderable.h"
#include "BaseComponent.h"
#include "FlipMode.h"
#include "Rect.h"

namespace Engine
{
	class Texture2D;
	class ScreenRenderComponent final : public BaseComponent, public Renderable
	{
	public:
		explicit ScreenRenderComponent() = default;
		virtual ~ScreenRenderComponent() = default;

		virtual void InitializeOverride(const SceneData& sceneData) override;
		virtual void DestroyOverride(const SceneData& sceneData) override;
		virtual void Render(const RenderManager& renderer) const override;

		void SetTexture(const std::string& filename);
		void SetTexture(const std::shared_ptr<Texture2D>& pTexture) { m_pTexture = pTexture; };
		const std::shared_ptr<Texture2D>& GetTexture() const { return m_pTexture; };

		void SetSource(const Rect& r) { m_Source = r; m_HasSource = true; };
		void ClearSource() { m_HasSource = false; };
		const Rect& GetSource() const { return m_Source; };
		bool HasSource() const { return m_HasSource; };

		void SetFlipMode(FlipMode flip) { m_FlipMode = flip; };
		FlipMode GetFlipMode() const { return m_FlipMode; };

	private:
		std::shared_ptr<Texture2D> m_pTexture;
		Rect m_Source;
		bool m_HasSource;
		FlipMode m_FlipMode;
	};
}
