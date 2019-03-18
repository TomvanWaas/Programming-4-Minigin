#pragma once
#include "BaseComponent.h"
namespace dae
{
	class Texture2D;
}

class RenderComponent final : public BaseComponent
{
public:
	explicit RenderComponent() = default;
	explicit RenderComponent(const std::string& filename);
	explicit RenderComponent(const std::shared_ptr<dae::Texture2D>& pTexture);
	virtual ~RenderComponent() override = default;

	RenderComponent(const RenderComponent& other) = delete;
	RenderComponent(RenderComponent&& other) noexcept = delete;
	RenderComponent& operator=(const RenderComponent& other) = delete;
	RenderComponent& operator=(RenderComponent&& other) noexcept = delete;

	virtual void Render() const override;

	void SetTexture(const std::string& filename);
	void SetTexture(const std::shared_ptr<dae::Texture2D>& pTexture);
	const std::shared_ptr<dae::Texture2D>& GetTexture() const;

private:
	std::shared_ptr<dae::Texture2D> m_pTexture;

};

