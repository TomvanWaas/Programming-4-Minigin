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

class MultiColliderCommand final : public ColliderCommand
{
public:
	explicit MultiColliderCommand(const std::vector<std::shared_ptr<ColliderCommand>>& pCommands);
	virtual ~MultiColliderCommand() = default;

	MultiColliderCommand(const MultiColliderCommand& other) = delete;
	MultiColliderCommand(MultiColliderCommand&& other) noexcept = delete;
	MultiColliderCommand& operator=(const MultiColliderCommand& other) = delete;
	MultiColliderCommand& operator=(MultiColliderCommand&& other) noexcept = delete;

	virtual void Execute(AABBCollisionComponent* pOther) override
	{
		for (auto& pCommand : m_pCommands)
		{
			pCommand->Execute(pOther);
		}
	}
private:
	std::vector<std::shared_ptr<ColliderCommand>> m_pCommands;
};