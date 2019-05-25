#include "stdafx.h"
#include "Font.h"
#include "Exceptions.h"

using namespace Minigin;

TTF_Font* Font::GetFont() const {
	return m_pFont;
}

Font::Font(const std::string& fullPath, unsigned size) : m_pFont(nullptr), m_Size(size)
{
	m_pFont = TTF_OpenFont(fullPath.c_str(), size);
	if (m_pFont == nullptr) 
	{
		std::stringstream msg{};
		msg << "Font > Failed to load font: " << fullPath << " : " << size;
		Logger::GetInstance().LogError(msg.str());
		throw ResourceException(fullPath);
	}
}

Font::~Font()
{
	TTF_CloseFont(m_pFont);
}
