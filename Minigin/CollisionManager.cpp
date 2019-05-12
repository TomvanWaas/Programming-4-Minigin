#include "MiniginPCH.h"
#include "CollisionManager.h"
#include "Rect.h"
#include "AABBCollisionComponent.h"
#include "Logger.h"
#include <algorithm>
#include "GameObject.h"
#include "SceneData.h"
#include "ObservedData.h"

#ifdef Debug
CollisionManager::CollisionManager()
	: Renderable(DebugPriority)
{}
#endif

CollisionManager::~CollisionManager()
{
	SAFE_DELETE(m_pPairsBuffer1);
	SAFE_DELETE(m_pPairsBuffer2);
}
void CollisionManager::Initialize(const SceneData& sceneData)
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
bool CollisionManager::Collides(AABBCollisionComponent* pCollider) const
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
bool CollisionManager::Collides(const Rect& a, const Rect& b) const
{
	return (a.x <= b.x + b.width &&
		a.x + a.width >= b.x &&
		a.y <= b.y + b.height &&
		a.y + a.height >= b.y);
}







//Calculate for enters/exits
void CollisionManager::Update(const SceneData& sceneData)
{
	UNREFERENCED_PARAMETER(sceneData);

	//////////////////////
	//Calculate Collisions
	//////////////////////
	m_pPairsBuffer1->clear();
	for (unsigned int i = 0; i < m_pColliders.size(); ++i)
	{
		auto* pCollI = m_pColliders[i];
		if (pCollI == nullptr || pCollI->GetGameObject() == nullptr) continue;
		for (unsigned int j = 0; j < m_pColliders.size(); ++j)
		{
			auto* pCollJ = m_pColliders[j];
			if (pCollJ == nullptr || pCollJ->GetGameObject() == nullptr || pCollJ->IsEnabled() == false || pCollI->IsEnabled() == false ||
				&pCollJ->GetGameObject()->GetRoot() == &pCollI->GetGameObject()->GetRoot()) continue;

			if (Collides(pCollI->GetCollider(), pCollJ->GetCollider()))
			{
				CollPair p{};
				p.pFirst = pCollI;
				p.pSecond = pCollJ;
				m_pPairsBuffer1->push_back(p);
			}
		}
	}

	///////////////////////////////////
	//Check buffers for exits or enters
	///////////////////////////////////
	//Enters
	for (auto& pI: *m_pPairsBuffer1)
	{
		bool f = false;
		for (auto& pJ: *m_pPairsBuffer2)
		{
			if (pI == pJ)
			{
				f = true;
				break;
			}
		}

		if (f) continue;

		//Else Entered
		if (pI.pFirst && pI.pFirst->GetGameObject())
		{
			auto& root = pI.pFirst->GetGameObject()->GetRoot();
			ObservedData data{};
			data.AddData<AABBCollisionComponent*>("Collider", pI.pSecond);
			root.Notify(ObservedEvent::ColliderEntered, data);
			root.NotifyChildren(ObservedEvent::ColliderEntered, data);
		}
		if (pI.pSecond && pI.pSecond->GetGameObject())
		{
			auto& root = pI.pSecond->GetGameObject()->GetRoot();
			ObservedData data{};
			data.AddData<AABBCollisionComponent*>("Collider", pI.pFirst);
			root.Notify(ObservedEvent::ColliderEntered, data);
			root.NotifyChildren(ObservedEvent::ColliderEntered, data);
		}
	}

	//Exits
	for (auto& pI : *m_pPairsBuffer2)
	{
		bool f = false;
		for (auto& pJ : *m_pPairsBuffer1)
		{
			if (pI == pJ)
			{
				f = true;
				break;
			}
		}

		if (f) continue;
		
		//Else Entered
		if (pI.pFirst && pI.pFirst->GetGameObject())
		{
			auto& root = pI.pFirst->GetGameObject()->GetRoot();
			ObservedData data{};
			data.AddData<AABBCollisionComponent*>("Collider", pI.pSecond);
			root.Notify(ObservedEvent::ColliderExited, data);
			root.NotifyChildren(ObservedEvent::ColliderExited, data);
		}
		if (pI.pSecond && pI.pSecond->GetGameObject())
		{
			auto& root = pI.pSecond->GetGameObject()->GetRoot();
			ObservedData data{};
			data.AddData<AABBCollisionComponent*>("Collider", pI.pFirst);
			root.Notify(ObservedEvent::ColliderExited, data);
			root.NotifyChildren(ObservedEvent::ColliderExited, data);
		}
	}

	///////////////
	//Swap buffers
	///////////////
	auto pBuffer = m_pPairsBuffer2;
	m_pPairsBuffer2 = m_pPairsBuffer1;
	m_pPairsBuffer1 = pBuffer;
}

#ifdef Debug
#include "RenderManager.h"
void CollisionManager::Render(const RenderManager& renderer) const
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

void CollisionManager::RegisterCollision(AABBCollisionComponent& col)
{
	auto i = std::find(m_pColliders.begin(), m_pColliders.end(), &col);
	if (i == m_pColliders.end())
	{
		m_pColliders.push_back(&col);
	}
}

void CollisionManager::UnRegisterCollision(AABBCollisionComponent& col)
{
	//Find in Vector
	m_pColliders.erase(std::remove(m_pColliders.begin(), m_pColliders.end(), &col), m_pColliders.end());

	//Find in Buffers
	(*m_pPairsBuffer1).erase(std::remove_if(m_pPairsBuffer1->begin(), m_pPairsBuffer1->end(), [&col](CollPair& pair)
	{
		return (pair.pFirst == &col || pair.pSecond == &col);
	}), m_pPairsBuffer1->end());

	(*m_pPairsBuffer2).erase(std::remove_if(m_pPairsBuffer2->begin(), m_pPairsBuffer2->end(), [&col](CollPair& pair)
	{
		return (pair.pFirst == &col || pair.pSecond == &col);
	}), m_pPairsBuffer2->end());

}








