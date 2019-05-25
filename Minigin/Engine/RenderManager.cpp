#include "stdafx.h"
#include "RenderManager.h"
#include "Renderable.h"
#include <algorithm>
#include <SDL.h>
#include "Texture2D.h"
#include "Rect.h"
#include "Vector2.h"
#include "Logger.h"

using namespace Minigin;

SDL_Renderer* Minigin::RenderManager::m_pRenderer = nullptr;

SDL_Renderer* Minigin::RenderManager::GetSDLRenderer()
{
	return m_pRenderer;
}
void Minigin::RenderManager::DestroyRenderer()
{
	if (m_pRenderer)
	{
		SDL_DestroyRenderer(m_pRenderer);
		m_pRenderer = nullptr;
	}
	Logger::GetInstance().LogInfo("RenderManager::DestroyRenderer > Succeeded");
}

void Minigin::RenderManager::ClearScreen()
{
	if (m_pRenderer == nullptr) return;
	SDL_RenderClear(m_pRenderer);
	SDL_RenderPresent(m_pRenderer);
}

void Minigin::RenderManager::InitializeRenderer(SDL_Window* pWindow)
{
	m_pRenderer = SDL_CreateRenderer(pWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (m_pRenderer == nullptr)
	{
		throw std::runtime_error(std::string("RenderManager::InitializeRenderer > ") + SDL_GetError());
	}
	Logger::GetInstance().LogInfo("RenderManager::InitializeRenderer > Succeeded");
}



void Minigin::RenderManager::Render(const Vector2& scale) const
{
	if (m_pRenderer == nullptr) return;
	SDL_RenderClear(m_pRenderer);
	SDL_RenderSetScale(m_pRenderer, scale.x, scale.y);

	for (const auto* pRenderable: m_pRenderables)
	{
		if (pRenderable == nullptr) continue;
		pRenderable->Render(*this);
	}

	SDL_RenderPresent(m_pRenderer);
	SDL_RenderSetScale(m_pRenderer, 1, 1);
}



void Minigin::RenderManager::AddRenderable(const Renderable& renderable)
{
	if (std::find(m_pRenderables.begin(), m_pRenderables.end(), &renderable) == m_pRenderables.end())
	{
		m_pRenderables.push_back(&renderable);
	}
	UpdatePriorities();
}
void Minigin::RenderManager::RemoveRenderable(const Renderable& renderable)
{
	auto i = std::find(m_pRenderables.begin(), m_pRenderables.end(), &renderable);

	if (i != m_pRenderables.end())
	{
		m_pRenderables.erase(i);
	}
}
void Minigin::RenderManager::UpdatePriorities()
{
	//Sort by priority
	std::sort(m_pRenderables.begin(), m_pRenderables.end(), [](const Renderable* a, const Renderable* b)
	{
		return (a && b && a->GetRenderPriority() < b->GetRenderPriority());
	});
}


void Minigin::RenderManager::RenderTexture(const Texture2D& texture, const Vector2& center) const
{
	SDL_Rect dst{};
	dst.w = texture.GetWidth();
	dst.h = texture.GetHeight();
	dst.x = int(center.x - dst.w * 0.5f);
	dst.y = int(center.y - dst.h * 0.5f);
	SDL_RenderCopy(GetSDLRenderer(), texture.GetSDLTexture(), nullptr, &dst);
}
void Minigin::RenderManager::RenderTexture(const Texture2D& texture, const Vector2& center, const Vector2& scale) const
{
	SDL_Rect dst{};
	dst.w = int(texture.GetWidth() * scale.x);
	dst.h = int(texture.GetHeight() * scale.y);
	dst.x = int(center.x - dst.w * 0.5f);
	dst.y = int(center.y - dst.h * 0.5f);
	SDL_RenderCopy(GetSDLRenderer(), texture.GetSDLTexture(), nullptr, &dst);
}
void Minigin::RenderManager::RenderTexture(const Texture2D& texture, const Vector2& center, const Rect& source) const
{
	SDL_Rect src{ int(source.x), int(source.y), int(source.width), int(source.height) };
	SDL_Rect dst{};
	dst.w = int(source.width);
	dst.h = int(source.height);
	dst.x = int(center.x - dst.w * 0.5f);
	dst.y = int(center.y - dst.h * 0.5f);
	SDL_RenderCopy(GetSDLRenderer(), texture.GetSDLTexture(), &src, &dst);
}
void Minigin::RenderManager::RenderTexture(const Texture2D& texture, const Vector2& center, const Vector2& scale, const Rect& source) const
{
	SDL_Rect src{ int(source.x), int(source.y), int(source.width), int(source.height) };
	SDL_Rect dst{};
	dst.w = int(source.width * scale.x);
	dst.h = int(source.height * scale.y);
	dst.x = int(center.x - dst.w * 0.5f);
	dst.y = int(center.y - dst.h * 0.5f);
	SDL_RenderCopy(GetSDLRenderer(), texture.GetSDLTexture(), &src, &dst);
}
void Minigin::RenderManager::RenderTexture(const Texture2D& texture, const Vector2& center, const Vector2& scale, float angle, const Vector2& around, FlipMode flip) const
{
	SDL_Rect dst{};
	dst.w = int(texture.GetWidth() * scale.x);
	dst.h = int(texture.GetHeight() * scale.y);
	dst.x = int(center.x - dst.w * 0.5f);
	dst.y = int(center.y - dst.h * 0.5f);
	SDL_Point ar{ int(around.x), int(around.y) };
	SDL_RenderCopyEx(GetSDLRenderer(), texture.GetSDLTexture(), nullptr, &dst, double(angle), &ar, SDL_RendererFlip(flip));
}
void Minigin::RenderManager::RenderTexture(const Texture2D& texture, const Vector2& center, const Vector2& scale, const Rect& source, float angle, const Vector2& around, FlipMode flip) const
{
	SDL_Rect src{ int(source.x), int(source.y), int(source.width), int(source.height) };
	SDL_Rect dst{};
	dst.w = int(source.width * scale.x);
	dst.h = int(source.height * scale.y);
	dst.x = int(center.x - dst.w * 0.5f);
	dst.y = int(center.y - dst.h * 0.5f);
	SDL_Point ar{ int(around.x), int(around.y) };
	SDL_RenderCopyEx(GetSDLRenderer(), texture.GetSDLTexture(), &src, &dst, double(angle), &ar, SDL_RendererFlip(flip));
}

void Minigin::RenderManager::RenderTexture(const Texture2D& texture, const Rect& dst, const Rect& src)
{
	SDL_Rect srcSDL{};
	srcSDL.x = int(src.x);
	srcSDL.y = int(src.y);
	srcSDL.w = int(src.width);
	srcSDL.h = int(src.height);
	SDL_Rect dstSDL{};
	dstSDL.x = int(dst.x);
	dstSDL.y = int(dst.y);
	dstSDL.w = int(dst.width);
	dstSDL.h = int(dst.height);
	SDL_RenderCopy(GetSDLRenderer(), texture.GetSDLTexture(), &srcSDL, &dstSDL);
}


void Minigin::RenderManager::RenderLine(const Vector2& a, const Vector2& b) const
{
	SDL_RenderDrawLine(m_pRenderer, int(a.x), int(a.y), int(b.x), int(b.y));
}
void Minigin::RenderManager::RenderRect(const Rect& r) const
{
	SDL_Rect sdlRect{ int(r.x), int(r.y), int(r.width), int(r.height) };
	SDL_RenderDrawRect(m_pRenderer, &sdlRect);
}
void Minigin::RenderManager::RenderPoint(const Vector2& p) const
{
	SDL_RenderDrawPoint(m_pRenderer, int(p.x), int(p.y));
}
void Minigin::RenderManager::RenderPoint(const Vector2& p, float r) const
{
	Rect rect(p.x - r, p.y - r, r * 2, r * 2);
	RenderRect(rect);
}




void Minigin::RenderManager::SetRenderColor(UINT8 r, UINT8 g, UINT8 b, UINT8 a) const
{
	SDL_SetRenderDrawColor(m_pRenderer, r, g, b, a);
}
void Minigin::RenderManager::ClearRenderColor() const
{
	SetRenderColor(0, 0, 0, 255);
}
