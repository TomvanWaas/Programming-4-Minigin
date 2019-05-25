#pragma once
#include "BaseComponent.h"
namespace DigDug
{
	class DigDugGridComponent;
	enum class Direction;
	class EnemyController final : public Minigin::BaseComponent
	{
		
	public:
		explicit EnemyController(DigDugGridComponent* pGrid, float fireInterval, float ghostInterval);
		virtual ~EnemyController() = default;
		virtual void UpdateFirstOverride(const Minigin::SceneData& sceneData) override;
	private:
		float m_FireAccu;
		float m_FireInterval;
		float m_GhostAccu;
		float m_GhostInterval;


		DigDugGridComponent* m_pGrid;
		Direction m_Current;
		

		void UpdateAsMove(const Minigin::SceneData& sceneData);
		void UpdateAsGhost(const Minigin::SceneData& sceneData);
	};

}