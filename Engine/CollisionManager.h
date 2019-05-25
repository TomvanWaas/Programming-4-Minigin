#pragma once
#include "Manager.h"
namespace Minigin
{

	class SceneData;
	class GameObject;
	struct Rect;
	class AABBCollisionComponent;
	class MovementComponent;
#ifdef Debug
#include "Renderable.h"
	class CollisionManager final : public Renderable, public Manager
#else
	class CollisionManager final : public Manager
#endif
	{
		struct CollPair
		{
			AABBCollisionComponent* pFirst = nullptr;
			AABBCollisionComponent* pSecond = nullptr;
			bool operator==(const CollPair& rhs) const
			{
				return ((pFirst == rhs.pFirst && pSecond == rhs.pSecond)
					|| (pFirst == rhs.pSecond && pSecond == rhs.pFirst));
			}
			bool operator!=(const CollPair& rhs) const
			{
				return !(*this == rhs);
			}
		};

	public:
#ifdef Debug
		explicit CollisionManager();
#else
		explicit CollisionManager() = default;
#endif
		virtual ~CollisionManager();

		CollisionManager(const CollisionManager& other) = delete;
		CollisionManager(CollisionManager&& other) noexcept = delete;
		CollisionManager& operator=(const CollisionManager& other) = delete;
		CollisionManager& operator=(CollisionManager&& other) noexcept = delete;

		virtual void Initialize(const SceneData& sceneData) override;
		virtual void Update(const SceneData& sceneData) override;

#ifdef Debug
		virtual void Render(const RenderManager& renderer) const override;
#endif

		void RegisterCollision(AABBCollisionComponent& col);
		void UnRegisterCollision(AABBCollisionComponent& col);

		bool Collides(AABBCollisionComponent* pCollider) const;
		bool Collides(const Rect& a, const Rect& b) const;



	private:
		//Registered colliders
		std::vector<AABBCollisionComponent*> m_pColliders;

		//Buffers to check exits &| enters
		std::vector<CollPair>* m_pPairsBuffer1;
		std::vector<CollPair>* m_pPairsBuffer2;


	};

}