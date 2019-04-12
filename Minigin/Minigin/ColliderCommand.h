#pragma once

class AABBCollisionComponent;
class ColliderCommand abstract
{
public:
	explicit ColliderCommand() = default;
	virtual ~ColliderCommand() = default;

	ColliderCommand(const ColliderCommand& other) = delete;
	ColliderCommand(ColliderCommand&& other) noexcept = delete;
	ColliderCommand& operator=(const ColliderCommand& other) = delete;
	ColliderCommand& operator=(ColliderCommand&& other) noexcept = delete;

	virtual void Execute(AABBCollisionComponent* pOther) = 0;

private:


};