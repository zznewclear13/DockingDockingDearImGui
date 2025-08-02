#include <iostream>

#include <memory>

#include "Support/WinInclude.h"
#include "Core/CoreSystem.h"

int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE hInstPrev, PSTR cmdline, int cmdshow)
{   
	return std::make_shared<DockingImGui::CoreSystem>()->Run();
    // return MessageBox(NULL, "hello, world", "caption", 0);
}