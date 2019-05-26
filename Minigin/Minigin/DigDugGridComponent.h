#pragma once
#include "BaseComponent.h"
#include "Vector2.h"
#include "Grid.h"

namespace Engine
{
	struct Rect;
	class MultiRenderComponent;
}

namespace DigDug
{

#ifdef Debug
#include "Renderable.h"
	class DigDugGridComponent final : public Minigin::BaseComponent, public Minigin::Renderable
#else
	class DigDugGridComponent final : public Engine::BaseComponent
#endif
	{
	public:
		DigDugGridComponent(unsigned int w = 1, unsigned int h = 1);
		virtual ~DigDugGridComponent() = default;

		DigDugGridComponent(const DigDugGridComponent& other) = delete;
		DigDugGridComponent(DigDugGridComponent&& other) noexcept = delete;
		DigDugGridComponent& operator=(const DigDugGridComponent& other) = delete;
		DigDugGridComponent& operator=(DigDugGridComponent&& other) noexcept = delete;

#ifdef Debug
		virtual void Render(const RenderManager& renderer) const override;
#endif



		bool IsOnPoint(const Engine::Vector2& p, float epsilon = 0.5f) const;
		bool IsOnLine(const Engine::Vector2& p, float epsilon = 0.5f) const;
		bool IsOnGrid(const Engine::Vector2& p, float epsilon = 0.5f) const;
		bool IsOnWalkablePoint(const Engine::Vector2& p, float epsilon = 0.5f) const;
		bool IsOnWalkableLine(const Engine::Vector2& p, float epsilon = 0.5f) const;
		bool IsOnWalkableGrid(const Engine::Vector2& p, float epsilon = 0.5f) const;
		Engine::Vector2 ClosestPoint(const Engine::Vector2& p) const;
		Engine::Vector2 ClosestLine(const Engine::Vector2& p) const;
		Engine::Vector2 ClosestGrid(const Engine::Vector2& p) const;
		Engine::Vector2 ClosestWalkablePoint(const Engine::Vector2& p) const;
		Engine::Vector2 ClosestWalkableLine(const Engine::Vector2& p) const;
		Engine::Vector2 ClosestWalkableGrid(const Engine::Vector2& p) const;


		bool IsMarked(const Engine::Vector2& p, float epsilon = 0.001f) const;
		void Mark(const Engine::Vector2& p, float epsilon = 0.001f);
		Engine::Vector2 GetPosition(unsigned int w, unsigned int h) const;
		Engine::Vector2 GetWalkablePosition(unsigned int w, unsigned int h)const;
		unsigned int GetIndex(const Engine::Vector2& v) const;


		void SetOffset(const Engine::Vector2& o);
		void SetWidth(unsigned int w);
		void SetHeight(unsigned int h);
		Engine::Vector2 GetWalkableOffset() const;
		const Engine::Vector2& GetOffset() const { return m_Offset; }
		unsigned int GetWidth() const;
		unsigned int GetHeight() const;


	protected:
		virtual void InitializeOverride(const Engine::SceneData& sceneData) override;

	private:
		Engine::Grid<bool> m_GridMarks;
		Engine::Vector2 m_Offset;
		Engine::MultiRenderComponent* m_pMultiRenderer;



		//Helpers
		void UpdateRender(unsigned int idx);
		void UpdateSurroundings(unsigned int w, unsigned int h);
		void UpdateSurroundings(unsigned int i);

		Engine::Vector2 WorldToGrid(const Engine::Vector2& p) const;
		Engine::Vector2 GridToWorld(const Engine::Vector2& p) const;

		Engine::Vector2 GetWorld(unsigned int w, unsigned int h) const;
		Engine::Vector2 GetLocal(unsigned int w, unsigned int h) const;
		int GetIdx(const Engine::Vector2& world, float epsilon = 0.001f) const;
		int CloseIdx(const Engine::Vector2& world) const;


	};
}
