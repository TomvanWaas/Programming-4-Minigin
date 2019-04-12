#pragma once
#include "BaseComponent.h"
#include "Vector2.h"
#include "Renderable.h"
class PositionGridComponent final : public BaseComponent, public Renderable
{
public:
	explicit PositionGridComponent(unsigned int width, unsigned int height, const Vector2& offset);
	virtual ~PositionGridComponent() override = default;

	PositionGridComponent(const PositionGridComponent& other) = delete;
	PositionGridComponent(PositionGridComponent&& other) noexcept = delete;
	PositionGridComponent& operator=(const PositionGridComponent& other) = delete;
	PositionGridComponent& operator=(PositionGridComponent&& other) noexcept = delete;

	virtual void Initialize(const SceneData& sceneData) override;

	bool IsValid(unsigned int w, unsigned int h) const;
	Vector2 Get(unsigned int w, unsigned int h) const;

	bool IsInGrid(const Vector2& pos) const;
	Vector2 ClampToGrid(const Vector2& pos) const;

	bool IsOnPoint(const Vector2& pos, float off = 0.5f) const;
	Vector2 ClampToPoint(const Vector2& pos) const;

	bool IsOnLine(const Vector2& pos) const;
	Vector2 ClampToLine(const Vector2& pos) const;

	//TEST
	virtual void Render(const RenderManager& renderer)const override;

private:
	unsigned int m_Width;
	unsigned int m_Height;
	Vector2 m_Offset;

	Vector2 GetUnsafe(unsigned int w, unsigned int h) const;
};

