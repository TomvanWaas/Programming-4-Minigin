#pragma once
#include "BaseComponent.h"
#include <map>
#include "Rect.h"

class Texture2D;

struct SpriteSource
{
	Rect sourceStart;
	unsigned int nrSourcesHor;
};


class SpriteComponent final : public BaseComponent
{
public:
	explicit SpriteComponent();
	explicit SpriteComponent(const std::shared_ptr<Texture2D>& pTexture, float speed);
	explicit SpriteComponent(const std::string& path, float speed);
	virtual ~SpriteComponent() = default;

	SpriteComponent(const SpriteComponent& other) = delete;
	SpriteComponent(SpriteComponent&& other) noexcept = delete;
	SpriteComponent& operator=(const SpriteComponent& other) = delete;
	SpriteComponent& operator=(SpriteComponent&& other) noexcept = delete;

	virtual void Update(const SceneData& sceneData);

	void SetTexture(const std::shared_ptr<Texture2D>& pTexture);
	void SetTexture(const std::string& path);
	const std::shared_ptr<Texture2D>& GetTexture() const;

	void AddSprite(unsigned int id, const SpriteSource& sprite);
	void RemoveSprite(unsigned int id);
	const SpriteSource* GetSprite(unsigned int id) const;

	void SetCurrentSource(unsigned int id);
	const SpriteSource* GetCurrentSource() const;

	void SetSpriteSpeed(float speed);
	float GetSpriteSpeed() const;

private:
	std::shared_ptr<Texture2D> m_pTexture;
	std::map<unsigned int, SpriteSource> m_pSources;

	unsigned int m_CurrentRect;
	SpriteSource* m_pCurrentSource;
	float m_SpriteSpeed;
	float m_AccuTime;

};

