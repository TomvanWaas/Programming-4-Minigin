#pragma once
#include "BaseComponent.h"
#include "Rect.h"
#include "Renderable.h"
#include "FlipMode.h"
class Texture2D;

class RenderComponent final : public BaseComponent, public Renderable
{
public:
	explicit RenderComponent() = default;
	explicit RenderComponent(const std::string& texturePath);
	explicit RenderComponent(const std::shared_ptr<Texture2D>& texture);
	virtual ~RenderComponent() override = default;

	RenderComponent(const RenderComponent& other) = delete;
	RenderComponent(RenderComponent&& other) noexcept = delete;
	RenderComponent& operator=(const RenderComponent& other) = delete;
	RenderComponent& operator=(RenderComponent&& other) noexcept = delete;

	virtual void InitializeOverride(const SceneData& sceneData) override;
	virtual void DestroyOverride(const SceneData& sceneData) override;
	virtual void Render(const RenderManager& renderer) const override;

	void SetTexture(const std::string& filename);
	void SetTexture(const std::shared_ptr<Texture2D>& pTexture);
	const std::shared_ptr<Texture2D>& GetTexture() const;

	void SetSource(const Rect& r);
	void ClearSource();
	const Rect& GetSource() const;
	bool HasSource() const;

	void SetFlipMode(FlipMode flip);
	FlipMode GetFlipMode() const;


private:
	std::shared_ptr<Texture2D> m_pTexture;
	Rect m_Source;
	bool m_HasSource;
	FlipMode m_FlipMode;

};

