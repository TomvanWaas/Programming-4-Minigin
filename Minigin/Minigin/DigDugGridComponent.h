#pragma once
#include "BaseComponent.h"
#include "Vector2.h"
#include "Grid.h"

namespace Minigin
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
	class DigDugGridComponent final : public Minigin::BaseComponent
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



		bool IsOnPoint(const Minigin::Vector2& p, float epsilon = 0.5f) const;
		bool IsOnLine(const Minigin::Vector2& p, float epsilon = 0.5f) const;
		bool IsOnGrid(const Minigin::Vector2& p, float epsilon = 0.5f) const;
		bool IsOnWalkablePoint(const Minigin::Vector2& p, float epsilon = 0.5f) const;
		bool IsOnWalkableLine(const Minigin::Vector2& p, float epsilon = 0.5f) const;
		bool IsOnWalkableGrid(const Minigin::Vector2& p, float epsilon = 0.5f) const;
		Minigin::Vector2 ClosestPoint(const Minigin::Vector2& p) const;
		Minigin::Vector2 ClosestLine(const Minigin::Vector2& p) const;
		Minigin::Vector2 ClosestGrid(const Minigin::Vector2& p) const;
		Minigin::Vector2 ClosestWalkablePoint(const Minigin::Vector2& p) const;
		Minigin::Vector2 ClosestWalkableLine(const Minigin::Vector2& p) const;
		Minigin::Vector2 ClosestWalkableGrid(const Minigin::Vector2& p) const;


		bool IsMarked(const Minigin::Vector2& p, float epsilon = 0.001f) const;
		void Mark(const Minigin::Vector2& p, float epsilon = 0.001f);
		Minigin::Vector2 GetPosition(unsigned int w, unsigned int h) const;
		Minigin::Vector2 GetWalkablePosition(unsigned int w, unsigned int h)const;
		unsigned int GetIndex(const Minigin::Vector2& v) const;


		void SetOffset(const Minigin::Vector2& o);
		void SetWidth(unsigned int w);
		void SetHeight(unsigned int h);
		Minigin::Vector2 GetWalkableOffset() const;
		const Minigin::Vector2& GetOffset() const { return m_Offset; }
		unsigned int GetWidth() const;
		unsigned int GetHeight() const;


	protected:
		virtual void InitializeOverride(const Minigin::SceneData& sceneData) override;

	private:
		Minigin::Grid<bool> m_GridMarks;
		Minigin::Vector2 m_Offset;
		Minigin::MultiRenderComponent* m_pMultiRenderer;



		//Helpers
		void UpdateRender(unsigned int idx);
		void UpdateSurroundings(unsigned int w, unsigned int h);
		void UpdateSurroundings(unsigned int i);

		Minigin::Vector2 WorldToGrid(const Minigin::Vector2& p) const;
		Minigin::Vector2 GridToWorld(const Minigin::Vector2& p) const;

		Minigin::Vector2 GetWorld(unsigned int w, unsigned int h) const;
		Minigin::Vector2 GetLocal(unsigned int w, unsigned int h) const;
		int GetIdx(const Minigin::Vector2& world, float epsilon = 0.001f) const;
		int CloseIdx(const Minigin::Vector2& world) const;


	};
}
