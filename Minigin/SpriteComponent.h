#pragma once
#include "BaseComponent.h"
#include <map>
#include "Rect.h"

class Texture2D;


struct SpriteSource abstract
{
	SpriteSource(bool looped = true)
		: looped(looped)
	{}
	virtual ~SpriteSource() = default;
	virtual Rect GetSource(float& accuTime, float speed) = 0;

	bool looped;
};

struct TickSource final : public SpriteSource
{
	enum class Flag : char
	{
		X = 1,
		Y = 2,
		W = 4,
		H = 8
	};
	TickSource(const Rect& start, int amount, bool looped = true, char flags = 1)
		: SpriteSource(looped)
		, start(start)
		, amount(amount)
		, flags(flags)
	{}
	virtual ~TickSource() = default;

	virtual Rect GetSource(float& accuTime, float speed) override;

	Rect start;
	int amount;
	char flags;
};

struct SweepSource final : public SpriteSource
{
	SweepSource(const Rect& start, const Rect& end, unsigned int amount, bool looped = true)
		: SpriteSource(looped)
		, start(start)
		, end (end)
		, amount(amount)
	{}

	virtual ~SweepSource() = default;
	virtual Rect GetSource(float& accuTime, float speed) override;

	Rect start;
	Rect end;
	unsigned int amount;
};



class SpriteComponent final : public BaseComponent
{
public:
	explicit SpriteComponent(GameObject& gameObject);
	virtual ~SpriteComponent();

	SpriteComponent(const SpriteComponent& other) = delete;
	SpriteComponent(SpriteComponent&& other) noexcept = delete;
	SpriteComponent& operator=(const SpriteComponent& other) = delete;
	SpriteComponent& operator=(SpriteComponent&& other) noexcept = delete;

	virtual void UpdateFirst(const SceneData& sceneData) override;

	void SetTexture(const std::shared_ptr<Texture2D>& pTexture);
	void SetTexture(const std::string& path);
	const std::shared_ptr<Texture2D>& GetTexture() const;

	void AddSprite(unsigned int id, SpriteSource* pSprite);
	void RemoveSprite(unsigned int id);
	const SpriteSource* GetSprite(unsigned int id) const;

	void SetCurrentSource(unsigned int id);
	Rect GetCurrentSource() const;

	void SetSpriteSpeed(float speed);
	float GetSpriteSpeed() const;


private:
	std::shared_ptr<Texture2D> m_pTexture;
	std::map<unsigned int, SpriteSource*> m_pSources;

	SpriteSource* m_pCurrentSource;
	float m_CurrentAmount;
	float m_SpriteSpeed;
	float m_AccuTime;


};

