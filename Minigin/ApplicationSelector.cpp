#include "MiniginPCH.h"
#include "ApplicationSelector.h"

#define DigDug

#ifdef DigDug
#include "DigDugApplication.h"
#endif


Application* Selector::GetApplication()
{
#ifdef DigDug
	return new DigDugApplication();
#else
	return nullptr;
#endif
}
