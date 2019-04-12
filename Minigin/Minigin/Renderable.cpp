#include "MiniginPCH.h"
#include "Renderable.h"
#include "RenderManager.h"

Renderable::Renderable(int priority)
	: m_Priority(priority)
{
}

int Renderable::GetRenderPriority() const
{
	return m_Priority;
}

void Renderable::RegisterRenderable(RenderManager* rendermanager)
{
	rendermanager->AddRenderable(*this);
}
void Renderable::UnRegisterRenderable(RenderManager* rendermanager)
{
	rendermanager->RemoveRenderable(*this);
}
