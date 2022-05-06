/**
 * @file core.hpp
 * @author Aalexdev (aaleex3984@gmail.com)
 * @brief the core tile of the engine, contain preprocessor calculations
 * @version 0.1
 * @date 2022-03-11
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#pragma once
#include <memory>

// operating system
#if defined(_WIN32)
	#ifdef ENGINE_BUILD_DLL
		#define ENGINE_API __declspec(dllexport)
	#else
		#define ENGINE_API __declspec(dllimport)
	#endif

	// libs

	#ifndef YAML_CPP_API
		#define YAML_CPP_API ENGINE_API
	#endif

	#ifndef IMGUI_API
		#define IMGUI_API ENGINE_API
	#endif
	
	#ifndef IMGUIFILEDIALOG_API
		#define IMGUIFILEDIALOG_API ENGINE_API
	#endif

	// architectur

	#ifdef _WIN64
		#define ENGINE_64
	#else
		#define ENGINE_32
	#endif
#endif

#ifdef ENGINE_ASSERTS
    #define ENGINE_ASSERT(x, ...) {if(!(x)) {ENGINE_ERROR("Assertion Failed: ", __VA_ARGS__);__debugbreak();}}
    #define ENGINE_CORE_ASSERT(x, ...) {if(!(x)) {ENGINE_CORE_ERROR("Assertion Failed: ", __VA_ARGS__);__debugbreak();}}
#else
    #define ENGINE_ASSERT(x, ...)
    #define ENGINE_CORE_ASSERT(x, ...)
#endif


#define ENGINE_BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)
#define BIT(x) (1 << x)

namespace engine{
	template<typename T>
	using Scope = std::unique_ptr<T>;

	template<typename T>
	using Ref = std::shared_ptr<T>;

	template<typename T, typename ...Args>
    constexpr Scope<T> createScope(Args&&... args){
        return std::make_unique<T>(std::forward<Args>(args)...);
    }

	template<typename T, typename ...Args>
    constexpr Ref<T> createRef(Args&&... args){
        return std::make_shared<T>(std::forward<Args>(args)...);
    }
}