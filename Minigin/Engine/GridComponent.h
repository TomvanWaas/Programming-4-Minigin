#pragma once
#include "BaseComponent.h"
#include "Grid.h"

namespace Minigin
{

	template <class T>
	class GridComponent final : public Grid<T>, public BaseComponent
	{
	public:
		explicit GridComponent(unsigned int w, unsigned int h)
			: Grid<T>(w, h)
		{
		}
		virtual ~GridComponent() = default;

		GridComponent(const GridComponent& other) = delete;
		GridComponent(GridComponent&& other) noexcept = delete;
		GridComponent& operator=(const GridComponent& other) = delete;
		GridComponent& operator=(GridComponent&& other) noexcept = delete;

		virtual void InitializeOverride(const SceneData& sceneData) override
		{
			UNREFERENCED_PARAMETER(sceneData);
			this->Make();
		}

	private:



	};

}