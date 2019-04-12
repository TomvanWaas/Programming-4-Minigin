#include "MiniginPCH.h"
#include "CollisionManager.h"
#include "Rect.h"
#include "AABBCollisionComponent.h"
#include "Logger.h"
#include <algorithm>
#include "GameObject.h"

CollisionManager::CollisionManager()
{
	m_pPairsBuffer1 = new std::vector<std::pair<AABBCollisionComponent*, AABBCollisionComponent*>>();
	m_pPairsBuffer2 = new std::vector<std::pair<AABBCollisionComponent*, AABBCollisionComponent*>>();
}

CollisionManager::~CollisionManager()
{
	SAFE_DELETE(m_pPairsBuffer1);
	SAFE_DELETE(m_pPairsBuffer2);
}

//Return true if pCollider collides, else false
bool CollisionManager::CollidesNow(AABBCollisionComponent* pCollider)
{
	if (pCollider == nullptr || pCollider->IsTrigger()) return false;
	pCollider->CalculateCollider();

	for (auto* pCol: m_pColliders)
	{
		if (pCol->IsTrigger()) continue;

		if (pCol != nullptr && pCol->GetGameObject()->GetRoot() != pCollider->GetGameObject()->GetRoot())
		{
			if (Collides(pCol->GetCollider(), pCollider->GetCollider()))
			{
				return true;
			}
		}
	}
	return false;
}





void CollisionManager::RegisterAABBCollision(AABBCollisionComponent* pCollider)
{
	if (std::find(m_pColliders.begin(), m_pColliders.end(), pCollider) != m_pColliders.end())
	{
		Logger::GetInstance().LogWarning("CollisionManager::RegisterAABBCollision > Collider already registered");
		return;
	}
	m_pColliders.push_back(pCollider);
}
void CollisionManager::UnRegisterAABBCollision(AABBCollisionComponent* pCollider)
{
	auto i = std::find(m_pColliders.begin(), m_pColliders.end(), pCollider);

	if (i == m_pColliders.end())
	{
		Logger::GetInstance().LogWarning("CollisionManager::UnRegisterAABBCollision > Collider not registered");
		return;
	}
	m_pColliders.erase(i);
}



void CollisionManager::Update()
{
	//Calc colliders because their transform may have changed
	CalculateColliders();

	//Calculate Collisions
	CalculateCollisions();

	//Check buffers for exits or enters
	CheckBuffers();

	//Swap buffers
	auto pBuffer = m_pPairsBuffer2;
	m_pPairsBuffer2 = m_pPairsBuffer1;
	m_pPairsBuffer1 = pBuffer;
}


bool CollisionManager::Collides(const Rect& a, const Rect& b) const
{
	return (a.x <= b.x + b.width &&
		a.x + a.width >= b.x &&
		a.y <= b.y + b.height &&
		a.y + a.height >= b.y);
}

void CollisionManager::CalculateCollisions()
{
	m_pPairsBuffer1->clear();

	for (unsigned int i{0}; i < m_pColliders.size(); ++i)
	{
		for (unsigned int j{i+1}; j < m_pColliders.size(); ++j)
		{
			auto c1 = m_pColliders[i];
			auto c2 = m_pColliders[j];

			if (c1->GetGameObject() != nullptr && c2->GetGameObject() != nullptr && c1->GetGameObject()->GetRoot() != c2->GetGameObject()->GetRoot())
			{
				if (Collides(c1->GetCollider(), c2->GetCollider()))
				{
					m_pPairsBuffer1->push_back(std::pair<AABBCollisionComponent*, AABBCollisionComponent*>(c1, c2));
				}
			}
		}
	}
}

void CollisionManager::CalculateColliders()
{
	for (AABBCollisionComponent* pCollider: m_pColliders)
	{
		if (pCollider == nullptr) continue;
		pCollider->CalculateCollider();
	}
}

void CollisionManager::CheckBuffers()
{
	//Check for enters, exits, etc

	//If in current buffer, but not in previous => Enter
	for (auto& pA: *m_pPairsBuffer1)
	{
		bool found = false;
		for (auto& pB: *m_pPairsBuffer2)
		{
			if (((pA.first == pB.first && pA.second == pB.second)
				|| (pA.first == pB.second && pA.second == pB.first)))
			{
				found = true;
				break;
			}
		}

		if (found) continue;

		pA.first->Enter(pA.second);
		pA.second->Enter(pA.first);
	}

	//If in previous buffer, but not in current => Exit
	for (auto& pA : *m_pPairsBuffer2)
	{
		bool found = false;
		for (auto& pB : *m_pPairsBuffer1)
		{
			if (((pA.first == pB.first && pA.second == pB.second)
				|| (pA.first == pB.second && pA.second == pB.first)))
			{
				found = true;
				break;
			}
		}

		if (found) continue;

		pA.first->Exit(pA.second);
		pA.second->Exit(pA.first);
	}
}
