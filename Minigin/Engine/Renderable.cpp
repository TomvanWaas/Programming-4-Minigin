#include "stdafx.h"
#include "Renderable.h"
#include "RenderManager.h"
#include "SceneData.h"

using namespace Minigin;

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
	sceneData.GetRender()->UpdatePriorities();
}

void Renderable::RegisterRenderable(RenderManager* rendermanager)
{
	rendermanager->AddRenderable(*this);
}
void Renderable::UnRegisterRenderable(RenderManager* rendermanager)
{
	rendermanager->RemoveRenderable(*this);
}
