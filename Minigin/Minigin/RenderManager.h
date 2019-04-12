#pragma once


struct SDL_Window;
struct SDL_Renderer;
class Renderable;
class RenderManager;
struct Rect;
class Texture2D;

class RenderManager
{
public:
	explicit RenderManager() = default;
	~RenderManager() = default;

	void Update();
	void Render() const;

	void AddRenderable(const Renderable& renderable);
	void RemoveRenderable(const Renderable& renderable);

	//Static
	static void InitializeRenderer(SDL_Window* pWindow);
	static SDL_Renderer* GetSDLRenderer();

	//Render shapes, etc
	void RenderTexture(const Texture2D& texture, float x, float y) const;
	void RenderTexture(const Texture2D& texture, float x, float y, float scalex, float scaley) const;
	void RenderTexture(const Texture2D& texture, const Rect& dstRect) const;
	void RenderTexture(const Texture2D& texture, const Rect& srcRect, const Rect& dstRect) const;

private:
	static SDL_Renderer* m_pRenderer;

	std::vector<const Renderable*> m_pRenderables;

};
