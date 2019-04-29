#include "MiniginPCH.h"
#include "RenderManager.h"
#include "Renderable.h"
#include <algorithm>
#include <SDL.h>
#include "Texture2D.h"
#include "Rect.h"
#include "Vector2.h"


SDL_Renderer* RenderManager::m_pRenderer = nullptr;

SDL_Renderer* RenderManager::GetSDLRenderer()
{
	return m_pRenderer;
}
void RenderManager::InitializeRenderer(SDL_Window* pWindow)
{
	m_pRenderer = SDL_CreateRenderer(pWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (m_pRenderer == nullptr)
	{
		throw std::runtime_error(std::string("RenderManager::InitializeRenderer > ") + SDL_GetError());
	}
}


void RenderManager::Update()
{
	//Remove all nullptr
	auto i = std::remove_if(m_pRenderables.begin(), m_pRenderables.end(),
		[](const Renderable* r)
	{
		return r == nullptr;
	});
	m_pRenderables.erase(i, m_pRenderables.end());
}
void RenderManager::Render() const
{
	if (m_pRenderer == nullptr) return;
	SDL_RenderClear(m_pRenderer);

	for (const auto* pRenderable: m_pRenderables)
	{
		if (pRenderable == nullptr) continue;
		pRenderable->Render(*this);
	}

	SDL_RenderPresent(m_pRenderer);
}

void RenderManager::AddRenderable(const Renderable& renderable)
{
	if (std::find(m_pRenderables.begin(), m_pRenderables.end(), &renderable) == m_pRenderables.end())
	{
		m_pRenderables.push_back(&renderable);
	}
	UpdatePriorities();
}
void RenderManager::RemoveRenderable(const Renderable& renderable)
{
	auto i = std::find(m_pRenderables.begin(), m_pRenderables.end(), &renderable);

	if (i != m_pRenderables.end())
	{
		m_pRenderables.erase(i);
	}
}
void RenderManager::UpdatePriorities()
{
	//Sort by priority
		std::sort(m_pRenderables.begin(), m_pRenderables.end(), [](const Renderable* a, const Renderable* b)
	{
		return a->GetRenderPriority() < b->GetRenderPriority();
	});
}


void RenderManager::RenderTexture(const Texture2D& texture, const float x, const float y) const
{
	SDL_Rect dst;
	dst.w = texture.GetWidth();
	dst.h = texture.GetHeight();
	dst.x = static_cast<int>(x) - dst.w / 2;
	dst.y = static_cast<int>(y) - dst.h / 2;

	SDL_RenderCopy(GetSDLRenderer(), texture.GetSDLTexture(), nullptr, &dst);
}
void RenderManager::RenderTexture(const Texture2D& texture, float x, float y, float scalex, float scaley) const
{
	SDL_Rect dst;
	dst.w = static_cast<int>(texture.GetWidth() * scalex);
	dst.h = static_cast<int>(texture.GetHeight() * scaley);
	dst.x = static_cast<int>(x) - dst.w / 2;
	dst.y = static_cast<int>(y) - dst.h / 2;

	SDL_RenderCopy(GetSDLRenderer(), texture.GetSDLTexture(), nullptr, &dst);
}
void RenderManager::RenderTexture(const Texture2D& texture, const Rect& dstRect) const
{
	SDL_Rect dst;
	dst.x = static_cast<int>(dstRect.x);
	dst.y = static_cast<int>(dstRect.y);
	dst.w = static_cast<int>(dstRect.width);
	dst.h = static_cast<int>(dstRect.height);

	SDL_RenderCopy(GetSDLRenderer(), texture.GetSDLTexture(), nullptr, &dst);
}
void RenderManager::RenderTexture(const Texture2D& texture, const Rect& srcRect, const Rect& dstRect) const
{
	SDL_Rect src;
	src.x = static_cast<int>(srcRect.x);
	src.y = static_cast<int>(srcRect.y);
	src.w = static_cast<int>(srcRect.width);
	src.h = static_cast<int>(srcRect.height);

	SDL_Rect dst;
	dst.x = static_cast<int>(dstRect.x);
	dst.y = static_cast<int>(dstRect.y);
	dst.w = static_cast<int>(dstRect.width);
	dst.h = static_cast<int>(dstRect.height);

	SDL_RenderCopy(GetSDLRenderer(), texture.GetSDLTexture(), &src, &dst);
}


void RenderManager::RenderLine(const Vector2& a, const Vector2& b) const
{
	SDL_RenderDrawLine(m_pRenderer, int(a.x), int(a.y), int(b.x), int(b.y));
}
void RenderManager::RenderRect(const Rect& r) const
{
	SDL_Rect sdlRect{ int(r.x), int(r.y), int(r.width), int(r.height) };
	SDL_RenderDrawRect(m_pRenderer, &sdlRect);
}
void RenderManager::RenderPoint(const Vector2& p) const
{
	SDL_RenderDrawPoint(m_pRenderer, int(p.x), int(p.y));
}

void RenderManager::RenderPoint(const Vector2& p, float r) const
{
	Rect rect(p.x - r, p.y - r, r * 2, r * 2);
	RenderRect(rect);
}

void RenderManager::SetRenderColor(UINT8 r, UINT8 g, UINT8 b, UINT8 a) const
{
	SDL_SetRenderDrawColor(m_pRenderer, r, g, b, a);
}
void RenderManager::ClearRenderColor() const
{
	SetRenderColor(0, 0, 0, 255);
}
