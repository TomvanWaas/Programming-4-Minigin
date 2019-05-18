#pragma once
#include "BaseComponent.h"
#include "Vector2.h"
#include "Grid.h"
struct Rect;
class MultiRenderComponent;
#ifdef Debug
#include "Renderable.h"
class DigDugGridComponent final : public BaseComponent, public Renderable
#else
class DigDugGridComponent final : public BaseComponent
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
	virtual void InitializeOverride(const SceneData& sceneData) override;



	bool IsOnPoint(const Vector2& p, float epsilon = 0.5f) const;
	bool IsOnLine(const Vector2& p, float epsilon = 0.5f) const;
	bool IsOnGrid(const Vector2& p, float epsilon = 0.5f) const;
	bool IsOnWalkablePoint(const Vector2& p, float epsilon = 0.5f) const;
	bool IsOnWalkableLine(const Vector2& p, float epsilon = 0.5f) const;
	bool IsOnWalkableGrid(const Vector2& p, float epsilon = 0.5f) const;
	Vector2 ClosestPoint(const Vector2& p) const;
	Vector2 ClosestLine(const Vector2& p) const;
	Vector2 ClosestGrid(const Vector2& p) const;
	Vector2 ClosestWalkablePoint(const Vector2& p) const;
	Vector2 ClosestWalkableLine(const Vector2& p) const;
	Vector2 ClosestWalkableGrid(const Vector2& p) const;


	bool IsMarked(const Vector2& p, float epsilon = 0.001f) const;
	void Mark(const Vector2& p, float epsilon = 0.001f);
	Vector2 GetPosition(unsigned int w, unsigned int h) const;
	Vector2 GetWalkablePosition(unsigned int w, unsigned int h)const;
	unsigned int GetIndex(const Vector2& v) const;


	void SetOffset(const Vector2& o);
	void SetWidth(unsigned int w);
	void SetHeight(unsigned int h);
	Vector2 GetWalkableOffset() const;
	Vector2 GetOffset() const { return m_Offset; }
	unsigned int GetWidth() const;
	unsigned int GetHeight() const;



	
private:
	Grid<bool> m_GridMarks;
	Vector2 m_Offset;
	MultiRenderComponent* m_pMultiRenderer;



	//Helpers
	void UpdateRender(unsigned int idx);
	void UpdateSurroundings(unsigned int w, unsigned int h);
	void UpdateSurroundings(unsigned int i);

	Vector2 WorldToGrid(const Vector2& p) const;
	Vector2 GridToWorld(const Vector2& p) const;

	Vector2 GetWorld(unsigned int w, unsigned int h) const;
	Vector2 GetLocal(unsigned int w, unsigned int h) const;
	int GetIdx(const Vector2& world, float epsilon = 0.001f) const;
	int CloseIdx(const Vector2& world) const;


};

