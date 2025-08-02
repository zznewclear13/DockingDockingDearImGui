#include "AppContext.h"

namespace DockingImGui
{
	bool AppContext::Initialize()
	{
		bool result = true;

		m_systemWindow = std::make_shared<SystemWindow>();
		m_systemWindow->Initialize();

		m_imGuiSystem = std::make_shared<ImGuiSystem>();
		result &= m_imGuiSystem->Initialize();

		return result;
	}

	void AppContext::Shutdown()
	{
		if (m_imGuiSystem)
		{
			m_imGuiSystem->Shutdown();
			m_imGuiSystem.reset();
		}

		if (m_systemWindow)
		{
			m_systemWindow->Shutdown();
			m_systemWindow.reset();
		}
	}
}