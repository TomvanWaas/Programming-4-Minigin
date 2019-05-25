#include "stdafx.h"
#include "Texture2D.h"
#include <SDL.h>
#include "Logger.h"

using namespace Minigin;

Texture2D::Texture2D(SDL_Texture* texture)
{
	m_pTexture = texture;
	if (SDL_QueryTexture(texture, nullptr, nullptr, &m_Width, &m_Height) != 0)
	{
		Logger::GetInstance().LogWarning("Texture2D::Texture2D > Invalid Texture");
	}
}
Texture2D::~Texture2D()
{
	SDL_DestroyTexture(m_pTexture);
}


SDL_Texture* Texture2D::GetSDLTexture() const
{
	return m_pTexture;
}
int Texture2D::GetWidth() const
{
	return m_Width;
}
int Texture2D::GetHeight() const
{
	return m_Height;
}

