#pragma once
#include "Observer.h"

namespace Minigin
{
	class ObservedData;
	class SceneData;

	class Manager abstract : public Observer
	{
	public:
		explicit Manager() = default;
		virtual ~Manager() = default;

		Manager(const Manager& other) = delete;
		Manager(Manager&& other) noexcept = delete;
		Manager& operator=(const Manager& other) = delete;
		Manager& operator=(Manager&& other) noexcept = delete;

		virtual void Initialize(const SceneData& sceneData) { UNREFERENCED_PARAMETER(sceneData); }
		virtual void Update(const SceneData& sceneData) { UNREFERENCED_PARAMETER(sceneData); }
		virtual void LateInitialize(const SceneData& sceneData) { UNREFERENCED_PARAMETER(sceneData); }
	};
}