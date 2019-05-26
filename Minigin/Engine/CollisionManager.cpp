#include "stdafx.h"
#include "CollisionManager.h"
#include "Rect.h"
#include "AABBCollisionComponent.h"
#include <algorithm>
#include "GameObject.h"
#include "SceneData.h"
#include "ObservedData.h"

using namespace Engine;

#ifdef Debug
Minigin::CollisionManager::CollisionManager()
	: Renderable(DebugPriority)
{}
#endif

Engine::CollisionManager::~CollisionManager()
{
	SAFE_DELETE(m_pPairsBuffer1);
	SAFE_DELETE(m_pPairsBuffer2);
}
void Engine::CollisionManager::Initialize(const SceneData& sceneData)
{
	UNREFERENCED_PARAMETER(sceneData);
	SAFE_DELETE(m_pPairsBuffer1);
	SAFE_DELETE(m_pPairsBuffer2);
	m_pPairsBuffer1 = new std::vector<CollPair>();
	m_pPairsBuffer2 = new std::vector<CollPair>();
#ifdef Debug
	RegisterRenderable(sceneData.GetRender());
#endif
}


//Return true if pCollider collides, else false
bool Engine::CollisionManager::Collides(AABBCollisionComponent* pCollider) const
{
	if (pCollider == nullptr || pCollider->IsEnabled() == false) return false;

	for (AABBCollisionComponent* pColl: m_pColliders)
	{
		if (pColl != nullptr && pCollider->CollidesWith(pColl))
		{
			return true;
		}
	}
	return false;
}
bool Engine::CollisionManager::Collides(const Rect& a, const Rect& b) const
{
	return (a.x <= b.x + b.width &&
		a.x + a.width >= b.x &&
		a.y <= b.y + b.height &&
		a.y + a.height >= b.y);
}







//Calculate for enters/exits
void Engine::CollisionManager::Update(const SceneData& sceneData)
{
	UNREFERENCED_PARAMETER(sceneData);
	//////////////////////////////////////
	//Calc Collisions : Fill buffer 1
	//////////////////////////////////////
	m_pPairsBuffer1->clear();
	//Filter nullptr
	m_pColliders.erase(std::remove_if(m_pColliders.begin(), m_pColliders.end(), [](AABBCollisionComponent* pCol)
	{
		return pCol == nullptr;
	}), m_pColliders.end());


	for (size_t i = 0, j = m_pColliders.size(); i < j; ++i)
	{
		auto* pColliderA = m_pColliders[i];

		//Valid Collider?
		if (!pColliderA || !pColliderA->IsEnabled() || !pColliderA->GetGameObject()) continue;

		for (size_t k = 0; k < j; ++k)
		{
			auto* pColliderB = m_pColliders[k];

			//Valid Collider?
			if (!pColliderB || !pColliderB->IsEnabled() || !pColliderB->GetGameObject()
				|| &pColliderB->GetGameObject()->GetRoot() == &pColliderA->GetGameObject()->GetRoot()) continue;

			//Else if collides
			if (Collides(pColliderA->GetCollider(), pColliderB->GetCollider()))
			{
				CollPair pair{};
				pair.pFirst = pColliderA;
				pair.pSecond = pColliderB;
				m_pPairsBuffer1->push_back(pair);
			}
		}
	}

	////////////////////////////////////////////
	// Compare buffer 1 & 2 for enters &| exits
	////////////////////////////////////////////

	//Enters
	for (size_t i = 0, j = m_pPairsBuffer1->size(); i < j; ++i)
	{
		auto& pair = (*m_pPairsBuffer1)[i];
		if (!pair.pSecond || !pair.pSecond) continue;

		auto it = std::find_if(m_pPairsBuffer2->begin(), m_pPairsBuffer2->end(), [&pair](const CollPair& other)
		{
			return pair == other;
		});
		if (it == m_pPairsBuffer2->end())
		{
			auto* pObjFirst = pair.pFirst->GetGameObject();
			auto* pObjSecond = pair.pSecond->GetGameObject();

			if (pObjFirst)
			{
				ObservedData d{};
				d.AddData<AABBCollisionComponent*>("Collider", pair.pSecond);
				pObjFirst->Notify(ObservedEvent::ColliderEntered, d);
			}
			if (pObjSecond)
			{
				ObservedData d{};
				d.AddData<AABBCollisionComponent*>("Collider", pair.pFirst);
				pObjSecond->Notify(ObservedEvent::ColliderEntered, d);
			}
		}
	}

	//Exits
	for (size_t i = 0, j = m_pPairsBuffer2->size(); i < j; ++i)
	{
		auto& pair = (*m_pPairsBuffer2)[i];

		if (!pair.pSecond || !pair.pSecond) continue;

		auto it = std::find_if(m_pPairsBuffer1->begin(), m_pPairsBuffer1->end(), [&pair](const CollPair& other)
		{
			return pair == other;
		});
		if (it == m_pPairsBuffer1->end())
		{
			auto* pObjFirst = pair.pFirst->GetGameObject();
			auto* pObjSecond = pair.pSecond->GetGameObject();

			if (pObjFirst)
			{
				ObservedData d{};
				d.AddData<AABBCollisionComponent*>("Collider", pair.pSecond);
				pObjFirst->Notify(ObservedEvent::ColliderExited, d);
			}
			if (pObjSecond)
			{
				ObservedData d{};
				d.AddData<AABBCollisionComponent*>("Collider", pair.pFirst);
				pObjSecond->Notify(ObservedEvent::ColliderExited, d);
			}
		}
	}


	/////////////////
	////Swap buffers
	/////////////////
	auto pBuffer = m_pPairsBuffer2;
	m_pPairsBuffer2 = m_pPairsBuffer1;
	m_pPairsBuffer1 = pBuffer;

}





#ifdef Debug
#include "RenderManager.h"
void Minigin::CollisionManager::Render(const RenderManager& renderer) const
{
	renderer.SetRenderColor(0, 255, 0, 255);
	for (const AABBCollisionComponent* pCol: m_pColliders)
	{
		if (pCol != nullptr && pCol->IsEnabled())
		{
			renderer.RenderRect(pCol->GetCollider());
		}
	}
	renderer.ClearRenderColor();
}
#endif

void Engine::CollisionManager::RegisterCollision(AABBCollisionComponent& col)
{
	auto i = std::find(m_pColliders.begin(), m_pColliders.end(), &col);
	if (i == m_pColliders.end())
	{
		m_pColliders.push_back(&col);
	}
}

void Engine::CollisionManager::UnRegisterCollision(AABBCollisionComponent& col)
{
	//In Vector
	auto i = std::find(m_pColliders.begin(), m_pColliders.end(), &col);
	if (i != m_pColliders.end())
	{
		(*i) = nullptr;
	}

	//In Buffers
	for (size_t j = 0, k = m_pPairsBuffer1->size(); j < k; ++j)
	{
		auto& pair = (*m_pPairsBuffer1)[j];
		if (pair.pFirst == &col || pair.pSecond == &col)
		{
			pair.pFirst = nullptr;
			pair.pSecond = nullptr;
		}
	}
	for (size_t j = 0, k = m_pPairsBuffer2->size(); j < k; ++j)
	{
		auto& pair = (*m_pPairsBuffer2)[j];
		if (pair.pFirst == &col || pair.pSecond == &col)
		{
			pair.pFirst = nullptr;
			pair.pSecond = nullptr;
		}
	}

}








