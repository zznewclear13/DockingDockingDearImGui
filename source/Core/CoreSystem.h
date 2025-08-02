#pragma once

#include <memory>
#include "Support/Singleton.h"

namespace DockingImGui
{
	class CoreSystem : public std::enable_shared_from_this<CoreSystem>
	{
	public:
		CoreSystem() = default;
		~CoreSystem() = default;

		int Run();

	private:
		bool Update(float dt);
		float m_framePerSecond{ 60.0f };
		float m_desiredFrameTimeMs { 1000.0f / m_framePerSecond };
	};
}
