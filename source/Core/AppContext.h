#pragma once

#include <memory>

#include "Support/Singleton.h"
#include "Core/CoreSystem.h"
#include "Core/SystemWindow.h"
#include "Core/ImGuiSystem.h"

namespace DockingImGui
{
	class AppContext : public Singleton<AppContext>
	{
		friend class Singleton<AppContext>;

	public:
		bool Initialize();
		void Shutdown();

	public:
		std::shared_ptr<CoreSystem> m_coreSystem;
		std::shared_ptr<SystemWindow> m_systemWindow;
		std::shared_ptr<ImGuiSystem> m_imGuiSystem;
	};
}
