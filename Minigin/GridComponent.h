#pragma once
#include "BaseComponent.h"
#include "Grid.h"

template <class T>
class GridComponent final : public Grid<T>, public BaseComponent
{
public:
	explicit GridComponent(GameObject& gameObject)
		: BaseComponent(gameObject)
	{
	}
	virtual ~GridComponent() = default;

	GridComponent(const GridComponent& other) = delete;
	GridComponent(GridComponent&& other) noexcept = delete;
	GridComponent& operator=(const GridComponent& other) = delete;
	GridComponent& operator=(GridComponent&& other) noexcept = delete;

	virtual void Initialize(const SceneData& sceneData) override
	{
		UNREFERENCED_PARAMETER(sceneData);
		this->Make();
	}
	


private:



};

