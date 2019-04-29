#pragma once
#include "BaseComponent.h"
#include "Vector2.h"
#include "Grid.h"
class MultiRenderComponent;
#ifdef Debug
#include "Renderable.h"
class DigDugGridComponent final : public BaseComponent, public Renderable
#else
class DigDugGridComponent final : public BaseComponent
#endif
{
public:
	DigDugGridComponent(GameObject& gameObject);
	virtual ~DigDugGridComponent() = default;
	
	DigDugGridComponent(const DigDugGridComponent& other) = delete;
	DigDugGridComponent(DigDugGridComponent&& other) noexcept = delete;
	DigDugGridComponent& operator=(const DigDugGridComponent& other) = delete;
	DigDugGridComponent& operator=(DigDugGridComponent&& other) noexcept = delete;

#ifdef Debug
	virtual void Render(const RenderManager& renderer) const override;
#endif
	virtual void Initialize(const SceneData& sceneData) override;


	bool IsOnPoint(const Vector2& p) const;
	bool IsMarked(const Vector2& p) const;
	void Mark(const Vector2& p);
	Vector2 GetPosition(unsigned int w, unsigned int h) const;



	void SetOffset(const Vector2& o);
	void SetWidth(unsigned int w);
	void SetHeight(unsigned int h);
	const Vector2& GetOffset() const;
	unsigned int GetWidth() const;
	unsigned int GetHeight() const;

private:
	Grid<bool> m_GridMarks;
	Grid<Vector2> m_GridPositions;
	Vector2 m_PositionOffset;
	MultiRenderComponent* m_pMultiRenderer;


	void UpdateRender(unsigned int idx);
	void UpdateSurroundings();

	Vector2 GetOffsetFromTransform(unsigned int i) const;

};

