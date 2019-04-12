#pragma once
struct Rect;
class AABBCollisionComponent;
class CollisionManager final
{
public:
	explicit CollisionManager();
	~CollisionManager();

	CollisionManager(const CollisionManager& other) = delete;
	CollisionManager(CollisionManager&& other) noexcept = delete;
	CollisionManager& operator=(const CollisionManager& other) = delete;
	CollisionManager& operator=(CollisionManager&& other) noexcept = delete;

	bool CollidesNow(AABBCollisionComponent* pCollider);

	void RegisterAABBCollision(AABBCollisionComponent* pCollider);
	void UnRegisterAABBCollision(AABBCollisionComponent* pCollider);

	void Update();

private:
	std::vector<AABBCollisionComponent*> m_pColliders;

	std::vector<std::pair<AABBCollisionComponent*, AABBCollisionComponent*>>* m_pPairsBuffer1;
	std::vector<std::pair<AABBCollisionComponent*, AABBCollisionComponent*>>* m_pPairsBuffer2;

	bool Collides(const Rect& a, const Rect& b) const;


	void CalculateCollisions();
	void CalculateColliders();
	void CheckBuffers();
};

