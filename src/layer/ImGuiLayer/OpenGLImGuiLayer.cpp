#include "engine/pch.hpp"
#include "engine/core.hpp"

/**
 * code mostly taken from the glfw_opengl3 ImGui exemple
 */

#include "engine/debug/Instrumentor.hpp"
#include "engine/debug/Log.hpp"
#include "engine/layer/ImGuiLayer/OpenGlImGuiLayer.hpp"
#include "engine/Application.hpp"
#include "engine/windows/Window.hpp"
#include "engine/windows/SDL2Window.hpp"

#include "engine/OpenGlAL/OpenGlContext.hpp"

#include "engine/ImGui_impl/imgui_impl_glfw.h"
#include "engine/ImGui_impl/imgui_impl_sdl.h"
#include "engine/ImGui_impl/imgui_impl_opengl3.h"

#include <libs/ImGui/imgui.h>
#include <libs/GLFW/glfw3.h>
#include <libs/SDL2/SDL.h>

#define USE_IMGUI_API
#include <libs/ImGuizmo/ImGuizmo.h>

namespace engine{

	Ref<OpenGlImGuiLayer> OpenGlImGuiLayer::create(){
		ENGINE_PROFILE_FUNCTION();
		switch (Window::getWindowAPI()){
			case Window::WindowAPI::GLFW: return createRef<GLFWOpenGlImGuiLayer>();
			case Window::WindowAPI::SDL2: return createRef<SDL2OpenGlImGuiLayer>();
		}
		ENGINE_CORE_ASSERT(false, "unknown window API");
		return nullptr;
	}

	static void setImGuiStyle(){
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
		// io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoTaskBarIcons;
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoMerge;

		// io.BackendFlags |= ImGuiBackendFlags_PlatformHasViewports;
		// io.BackendFlags |= ImGuiBackendFlags_RendererHasViewports;

		io.Fonts->AddFontFromFileTTF("assets/fonts/lato/Lato-Bold.ttf", 14.f);
		io.FontDefault = io.Fonts->AddFontFromFileTTF("assets/fonts/lato/Lato-Regular.ttf", 14.f);
		
		ImGui::StyleColorsDark();

		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable){
			style.WindowRounding = 5.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		ImGuiLayer::setStyle();
	}

	// GLFW

	void GLFWOpenGlImGuiLayer::OnAttach(){
		ENGINE_PROFILE_FUNCTION();
		
		glfwMakeContextCurrent(static_cast<GLFWwindow*>(app->getDisplay().getNativeWindow()));
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();

		setImGuiStyle();

		ImGui_ImplGlfw_InitForOpenGL(static_cast<GLFWwindow*>(app->getDisplay().getNativeWindow()), true);
		ImGui_ImplOpenGL3_Init("#version 410");
	}

	void GLFWOpenGlImGuiLayer::OnDetach(){
		ENGINE_PROFILE_FUNCTION();
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void GLFWOpenGlImGuiLayer::begin(){
		ENGINE_PROFILE_FUNCTION();
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGuizmo::BeginFrame();
	}

	void GLFWOpenGlImGuiLayer::end(){
		ENGINE_PROFILE_FUNCTION();
		ImGuiIO &io = ImGui::GetIO();

		io.DisplaySize = ImVec2(app->getDisplay().getWidth(), app->getDisplay().getHeight());
		
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable){
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}
	}

	// SDL2

	#define SDL_GL_CONTEXT static_cast<OpenGl::SDL2OpenGLContext*>(app->getDisplay().getContext().get())->getContext()
	#define SDL_WINDOW static_cast<SDL_Window*>(app->getDisplay().getNativeWindow())

	void SDL2OpenGlImGuiLayer::OnAttach(){
		ENGINE_PROFILE_FUNCTION();

		static_cast<windows::SDL2Window*>(&app->getDisplay())->setSDLEventCallback(ImGui_ImplSDL2_ProcessEvent);
		
		SDL_GL_MakeCurrent(SDL_WINDOW, SDL_GL_CONTEXT);
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();

		setImGuiStyle();

		ImGui_ImplSDL2_InitForOpenGL(SDL_WINDOW, SDL_GL_CONTEXT);
		ImGui_ImplOpenGL3_Init("#version 410");
	}

	void SDL2OpenGlImGuiLayer::OnDetach(){
		ENGINE_PROFILE_FUNCTION();
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplSDL2_Shutdown();
		ImGui::DestroyContext();
	}

	void SDL2OpenGlImGuiLayer::begin(){
		ENGINE_PROFILE_FUNCTION();
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplSDL2_NewFrame();
		ImGui::NewFrame();
		ImGuizmo::BeginFrame();
	}

	void SDL2OpenGlImGuiLayer::end(){
		ENGINE_PROFILE_FUNCTION();
		ImGuiIO &io = ImGui::GetIO();

		io.DisplaySize = ImVec2(app->getDisplay().getWidth(), app->getDisplay().getHeight());
		
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable){
            SDL_Window* backup_current_window = SDL_GL_GetCurrentWindow();
            SDL_GLContext backup_current_context = SDL_GL_GetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            SDL_GL_MakeCurrent(backup_current_window, backup_current_context);
        }
	}
}