#pragma once
#include <map>
#include "Rect.h"
#include "BaseComponent.h"
#include "Renderable.h"

class Texture2D;
class MultiRenderComponent final : public BaseComponent, public Renderable
{
public:
	struct RenderInfo
	{
		Rect src;
		Rect dst;
		bool hasSrc;
		bool hasDst;
	};

	explicit MultiRenderComponent(GameObject& gameObject);
	virtual ~MultiRenderComponent() override = default;

	MultiRenderComponent(const MultiRenderComponent& other) = delete;
	MultiRenderComponent(MultiRenderComponent&& other) noexcept = delete;
	MultiRenderComponent& operator=(const MultiRenderComponent& other) = delete;
	MultiRenderComponent& operator=(MultiRenderComponent&& other) noexcept = delete;

	virtual void Initialize(const SceneData& sceneData) override;
	virtual void Render(const RenderManager& renderer) const override;

	void AddRenderInfo(unsigned int id ,const RenderInfo& info);
	void RemoveRenderInfo(unsigned int id);
	const RenderInfo* GetRenderInfo(unsigned int id) const;
	void ClearRenderInfos();

	void SetTexture(const std::string& filename);
	void SetTexture(const std::shared_ptr<Texture2D>& pTexture);
	const std::shared_ptr<Texture2D>& GetTexture() const;

private:
	std::map<unsigned int, RenderInfo> m_RenderInfos;
	std::shared_ptr<Texture2D> m_pTexture;


};

