#include "CoreSystem.h"

#include <chrono>

#include "SystemWindow.h"
#include "Core/AppContext.h"

namespace DockingImGui
{
	int CoreSystem::Run()
	{
		AppContext::Get().m_coreSystem = shared_from_this();
		AppContext::Get().Initialize();

		auto startTime = std::chrono::steady_clock::now();
		auto prevTime = startTime;
		float accumulator = 0.0f;

		bool shouldExit = false;
		while (!shouldExit)
		{
			auto currentTime = std::chrono::steady_clock::now();
			float deltaTime = std::chrono::duration<float, std::milli>(currentTime - prevTime).count();
			prevTime = currentTime;

			accumulator += deltaTime;
			if (accumulator > 4.0f * m_desiredFrameTimeMs)
			{
				accumulator = 0.0f;
				deltaTime = m_desiredFrameTimeMs;
			}

			AppContext::Get().m_systemWindow->Update();
			shouldExit |= AppContext::Get().m_systemWindow->CloseRequested();
			shouldExit |= AppContext::Get().m_imGuiSystem->CloseRequested();
	
			while (accumulator > m_desiredFrameTimeMs)
			{
				if (!Update(deltaTime))
				{
					shouldExit = true;
					return 1;
				}

				AppContext::Get().m_imGuiSystem->Update();

				accumulator -= m_desiredFrameTimeMs;
			}
		}

		AppContext::Get().Shutdown();
		AppContext::Get().m_coreSystem.reset();
		return 0;
	}

	bool CoreSystem::Update(float deltaTime)
	{
		return true;
	}
}