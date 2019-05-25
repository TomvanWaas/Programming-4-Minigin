#pragma once
#include "Manager.h"
#include "FlipMode.h"
#include "Vector2.h"
	struct SDL_Window;
	struct SDL_Renderer;
namespace Minigin
{
	class Renderable;
	struct Rect;
	class Texture2D;

	class RenderManager final : public Manager
	{
	public:
		explicit RenderManager() = default;
		~RenderManager() = default;

		void Render(const Vector2& scale = Vector2::One) const;

		void AddRenderable(const Renderable& renderable);
		void RemoveRenderable(const Renderable& renderable);
		void UpdatePriorities();

		//Static
		static void InitializeRenderer(SDL_Window* pWindow);
		static SDL_Renderer* GetSDLRenderer();
		static void DestroyRenderer();
		static void ClearScreen();

		//Render shapes, etc
		void RenderTexture(const Texture2D& texture, const Vector2& center) const;
		void RenderTexture(const Texture2D& texture, const Vector2& center, const Vector2& scale) const;
		void RenderTexture(const Texture2D& texture, const Vector2& center, const Rect& source) const;
		void RenderTexture(const Texture2D& texture, const Vector2& center, const Vector2& scale, const Rect& source) const;
		void RenderTexture(const Texture2D& texture, const Vector2& center, const Vector2& scale, float angle, const Vector2& around, FlipMode flip) const;
		void RenderTexture(const Texture2D& texture, const Vector2& center, const Vector2& scale, const Rect& source, float angle, const Vector2& around, FlipMode flip) const;
		void RenderTexture(const Texture2D& texture, const Rect& dst, const Rect& src);


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
}