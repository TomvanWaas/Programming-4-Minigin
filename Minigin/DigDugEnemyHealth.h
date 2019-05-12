#pragma once
#include "BaseComponent.h"

class DigDugEnemyHealth final : public BaseComponent
{
public:
	enum class Health
	{
		Alive,
		Crushed,
		Pumped,
		Dead
	};

	explicit DigDugEnemyHealth() = default;
	virtual ~DigDugEnemyHealth() = default;

	void SetHealth(Health h) { m_Health = h; }
	Health GetHealth() const { return m_Health; }
private:
	Health m_Health = Health::Alive;

};

