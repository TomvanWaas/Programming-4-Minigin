#pragma once
#include <map>
#include "Rect.h"
#include "BaseComponent.h"
#include "Renderable.h"


namespace Minigin
{
	class Texture2D;
	class MultiRenderComponent final : public BaseComponent, public Renderable
	{
	public:

		explicit MultiRenderComponent() = default;
		explicit MultiRenderComponent(const std::string& texturePath);
		explicit MultiRenderComponent(const std::shared_ptr<Texture2D>& texture);
		virtual ~MultiRenderComponent() override = default;

		MultiRenderComponent(const MultiRenderComponent& other) = delete;
		MultiRenderComponent(MultiRenderComponent&& other) noexcept = delete;
		MultiRenderComponent& operator=(const MultiRenderComponent& other) = delete;
		MultiRenderComponent& operator=(MultiRenderComponent&& other) noexcept = delete;

		virtual void InitializeOverride(const SceneData& sceneData) override;
		virtual void DestroyOverride(const SceneData& sceneData) override;
		virtual void Render(const RenderManager& renderer) const override;

		void AddRenderDst(unsigned int id, const Rect& dst);
		void RemoveRenderDst(unsigned int id);
		const Rect* GetRenderDst(unsigned int id) const;
		void ClearRenderDst();

		const Rect& GetSource() const { return m_Source; }
		bool HasSource() const { return m_HasSource; }
		void ClearSource() { m_HasSource = false; }
		void SetSource(const Rect& src) { m_Source = src; m_HasSource = true; }


		void SetTexture(const std::string& filename);
		void SetTexture(const std::shared_ptr<Texture2D>& pTexture);
		const std::shared_ptr<Texture2D>& GetTexture() const;

	private:
		std::map<unsigned int, Rect> m_RenderDestinations;
		std::shared_ptr<Texture2D> m_pTexture;
		Rect m_Source;
		bool m_HasSource = false;

	};

}