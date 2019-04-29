#pragma once
struct SceneData;
class GameObject;
struct Rect;
class AABBCollisionComponent;
class MovementComponent;
#ifdef Debug
#include "Renderable.h"
class CollisionManager final : public Renderable
#else
class CollisionManager final
#endif
{
	struct CollPair
	{
		AABBCollisionComponent* pFirst;
		AABBCollisionComponent* pSecond;
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

	void Initialize(const SceneData& sceneData);
	void UpdateBuffers();

#ifdef Debug
	virtual void Render(const RenderManager& renderer) const override;
#endif

	void RegisterCollision(AABBCollisionComponent& col);


	bool Collides(AABBCollisionComponent* pCollider) const;
	bool Collides(const Rect& a, const Rect& b) const;

	

private:
	//Registered colliders
	std::vector<AABBCollisionComponent*> m_pColliders;

	//Buffers to check exits &| enters
	std::vector<CollPair>* m_pPairsBuffer1;
	std::vector<CollPair>* m_pPairsBuffer2;


};

