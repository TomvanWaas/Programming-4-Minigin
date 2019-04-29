#pragma once

struct Vector2;
struct SDL_Window;
struct SDL_Renderer;
class Renderable;
class RenderManager;
struct Rect;
class Texture2D;

class RenderManager final
{
public:
	explicit RenderManager() = default;
	~RenderManager() = default;

	void Update();
	void Render() const;

	void AddRenderable(const Renderable& renderable);
	void RemoveRenderable(const Renderable& renderable);
	void UpdatePriorities();

	//Static
	static void InitializeRenderer(SDL_Window* pWindow);
	static SDL_Renderer* GetSDLRenderer();

	//Render shapes, etc
	void RenderTexture(const Texture2D& texture, float x, float y) const;
	void RenderTexture(const Texture2D& texture, float x, float y, float scalex, float scaley) const;
	void RenderTexture(const Texture2D& texture, const Rect& dstRect) const;
	void RenderTexture(const Texture2D& texture, const Rect& srcRect, const Rect& dstRect) const;

	void RenderLine(const Vector2& a, const Vector2& b) const;
	void RenderRect(const Rect& r) const;
	void RenderPoint(const Vector2& p) const;
	void RenderPoint(const Vector2& p, float r) const;
	void SetRenderColor(UINT8 r = 0, UINT8 g = 0, UINT8 b = 0, UINT8 a = 255) const;
	void ClearRenderColor() const;

private:
	static SDL_Renderer* m_pRenderer;

	std::vector<const Renderable*> m_pRenderables;

};
