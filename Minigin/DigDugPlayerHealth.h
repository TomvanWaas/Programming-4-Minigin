#pragma once
#include "BaseComponent.h"

class DigDugPlayerHealth final : public BaseComponent
{
public:
	enum class Health
	{
		Alive,
		Crushed,
		Dead
	};

	explicit DigDugPlayerHealth() = default;
	virtual ~DigDugPlayerHealth() = default;

	void SetHealth(Health h) { m_Health = h; }
	const Health& GetHealth() { return m_Health; }

private:
	Health m_Health;

};

