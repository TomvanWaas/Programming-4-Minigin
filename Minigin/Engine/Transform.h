#pragma once
#include "Vector2.h"
namespace Engine
{
	class ObservedEvent;
	class ObservedData;
	class GameObject;
	class Transform final
	{
	public:
		explicit Transform(GameObject& gameObject);
		~Transform() = default;

		Transform(const Transform& other) = delete;
		Transform(Transform&& other) noexcept = delete;
		Transform& operator=(const Transform& other) = delete;
		Transform& operator=(Transform&& other) noexcept = delete;

		const GameObject* GetGameObject() const;
		void OnNotify(ObservedEvent event, const ObservedData& data);

		void SetWorldPosition(float x, float y);
		void SetWorldPosition(const Vector2& p);
		const Vector2& GetWorldPosition() const;
		void SetLocalPosition(float x, float y);
		void SetLocalPosition(const Vector2& p);
		const Vector2& GetLocalPosition() const;

		void SetWorldRotation(float rot, bool isdegrees = true);
		float GetWorldRotation(bool isdegrees = true) const;
		void SetLocalRotation(float rot, bool isdegrees = true);
		float GetLocalRotation(bool isdegrees = true) const;

		void SetWorldScale(Vector2 s);
		void SetWorldScale(float x, float y);
		const Vector2& GetWorldScale() const;
		void SetLocalScale(const Vector2& s);
		void SetLocalScale(float x, float y);
		const Vector2& GetLocalScale() const;

		void SetWorldTransform(const Vector2& pos, const Vector2& scale, float rot = 0, bool isdegrees = true);
		void SetLocalTransform(const Vector2& pos, const Vector2& scale, float rot = 0, bool isdegrees = true);

	private:
		GameObject* m_pGameObject;
		Vector2 m_LocalPosition;
		Vector2 m_LocalScale;
		float m_LocalRotationDegrees;

		Vector2 m_WorldPosition;
		Vector2 m_WorldScale;
		float m_WorldRotationDegrees;


	};

}