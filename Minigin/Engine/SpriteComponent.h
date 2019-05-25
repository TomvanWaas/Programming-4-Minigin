#pragma once
#include "BaseComponent.h"
#include <map>
#include "Rect.h"


namespace Minigin
{
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
			, end(end)
			, amount(amount)
		{}

		virtual ~SweepSource() = default;
		virtual Rect GetSource(float& accuTime, float speed) override;

		Rect start;
		Rect end;
		unsigned int amount;
	};

	struct FixedSource final : public SpriteSource
	{
		FixedSource(const Rect& source)
			: SpriteSource(false)
			, source(source)
		{}
		virtual ~FixedSource() = default;
		virtual Rect GetSource(float& accuTime, float speed) override;
		Rect source;
	};

	struct GrowSource final : public SpriteSource
	{
		GrowSource(bool looped) : SpriteSource(looped) {}
		virtual ~GrowSource() = default;
		virtual Rect GetSource(float& accuTime, float speed) override;
		std::vector<Rect> sources;
	};


	class SpriteComponent final : public BaseComponent
	{
	public:
		explicit SpriteComponent(const std::shared_ptr<Texture2D>& pTexture = nullptr, float speed = 0.2f);
		explicit SpriteComponent(const std::string& path, float speed = 0.2f);
		virtual ~SpriteComponent();

		SpriteComponent(const SpriteComponent& other) = delete;
		SpriteComponent(SpriteComponent&& other) noexcept = delete;
		SpriteComponent& operator=(const SpriteComponent& other) = delete;
		SpriteComponent& operator=(SpriteComponent&& other) noexcept = delete;

		virtual void UpdateFirstOverride(const SceneData& sceneData) override;

		void SetTexture(const std::shared_ptr<Texture2D>& pTexture);
		void SetTexture(const std::string& path);
		const std::shared_ptr<Texture2D>& GetTexture() const;

		void AddSprite(unsigned int id, SpriteSource* pSprite);
		void RemoveSprite(unsigned int id);
		const SpriteSource* GetSprite(unsigned int id) const;

		void SetCurrentSprite(unsigned int id, bool force = true);
		const Rect& GetCurrentSource() const { return m_CurrentSource; };

		void SetSpriteSpeed(float speed);
		float GetSpriteSpeed() const;

		bool IsFreezed() const { return m_IsFreezed; }
		void SetFreezed(bool f) { m_IsFreezed = f; }
	private:
		std::shared_ptr<Texture2D> m_pTexture;
		std::map<unsigned int, SpriteSource*> m_pSources;

		SpriteSource* m_pCurrentSprite;
		Rect m_CurrentSource;
		float m_SpriteSpeed;
		float m_AccuTime;
		bool m_IsFreezed = false;

	};
}
