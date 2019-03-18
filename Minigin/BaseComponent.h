#pragma once
namespace dae
{
	class GameObject;
}

class BaseComponent
{
public:
	explicit BaseComponent() = default;
	virtual ~BaseComponent() = default;

	BaseComponent(const BaseComponent& other) = delete;
	BaseComponent(BaseComponent&& other) noexcept = delete;
	BaseComponent& operator=(const BaseComponent& other) = delete;
	BaseComponent& operator=(BaseComponent&& other) noexcept = delete;

	virtual void Initialize();
	virtual void Update();
	virtual void Render() const;
	void SetOwner(dae::GameObject* owner);

protected:
	dae::GameObject* m_pOwner;



};

