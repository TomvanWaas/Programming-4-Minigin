#include "MiniginPCH.h"
#include "Renderable.h"
#include "RenderManager.h"
#include "SceneData.h"

Renderable::Renderable(int priority)
	: m_Priority(priority)
{
}

int Renderable::GetRenderPriority() const
{
	return m_Priority;
}

void Renderable::SetRenderPriority(int p, const SceneData& sceneData)
{
	m_Priority = p;
	sceneData.pRenderManager->UpdatePriorities();
}

void Renderable::RegisterRenderable(RenderManager* rendermanager)
{
	rendermanager->AddRenderable(*this);
}
void Renderable::UnRegisterRenderable(RenderManager* rendermanager)
{
	rendermanager->RemoveRenderable(*this);
}
