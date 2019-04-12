#pragma once
class Renderer;
class RenderManager;
class Renderable abstract
{
public:
	explicit Renderable(int priority = 0);
	virtual ~Renderable() = default;

	Renderable(const Renderable& other) = delete;
	Renderable(Renderable&& other) noexcept = delete;
	Renderable& operator=(const Renderable& other) = delete;
	Renderable& operator=(Renderable&& other) noexcept = delete;

	virtual void Render(const RenderManager& renderer) const = 0;
	int GetRenderPriority() const;

protected:
	void RegisterRenderable(RenderManager* rendermanager);
	void UnRegisterRenderable(RenderManager* rendermanager);
private:
	int m_Priority;

};

