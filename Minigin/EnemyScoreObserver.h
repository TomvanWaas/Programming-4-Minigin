#pragma once
#include "Observer.h"
#include "Color.h"

struct Vector2;
class Font;
class Scene;

class EnemyScoreObserver final : public Observer, public Subject
{
	
public:
	enum class Type
	{
		Pooka,
		Fygar
	};
	explicit EnemyScoreObserver(Type type);
	virtual ~EnemyScoreObserver() = default;
	virtual void Notify(ObservedEvent event, const ObservedData& data) override;
private:
	int GetScore(const Vector2& worldPos);

	Type m_Type;

};

