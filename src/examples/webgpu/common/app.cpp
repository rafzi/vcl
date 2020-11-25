/*
 * This file is part of the Visual Computing Library (VCL) release under the
 * MIT license.
 *
 * Copyright (c) 2020 Basil Fierz
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#include "app.h"

// C++ standard library
#include <iostream>
#include <exception>
#include <stdexcept>

#ifndef VCL_ARCH_WEBASM
// GLFW
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

// Dawn
#include <dawn/dawn_proc.h>
#else
// Emscripten
#include <emscripten.h>
#include <emscripten/html5.h>
#include <emscripten/html5_webgpu.h>
#endif

// VCL

const int Application::NumberOfFrames = 3;

static void printGlfwError(int error, const char* description)
{
	fprintf(stdout, "Glfw Error %d: %s\n", error, description);
}

static void printDeviceError(WGPUErrorType errorType, const char* message, void*)
{
	const char* error_type = "";
	switch (errorType) {
	case WGPUErrorType_Validation:
		error_type = "Validation";
		break;
	case WGPUErrorType_OutOfMemory:
		error_type = "Out of memory";
		break;
	case WGPUErrorType_Unknown:
		error_type = "Unknown";
		break;
	case WGPUErrorType_DeviceLost:
		error_type = "Device lost";
		break;
	default:
		std::cout << "Unknown" << std::endl;
	}

	std::cout << error_type << " error: " << message << std::endl;
}


Application::Application(const char* title)
{
	glfwSetErrorCallback(printGlfwError);
	glfwInit();

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	_windowHandle = glfwCreateWindow(1280, 720, title, NULL, NULL);

	if (!initWebGpu(_windowHandle))
	{
		if (_windowHandle)
			glfwDestroyWindow(_windowHandle);
		glfwTerminate();
		throw std::runtime_error("WebGPU failed to initialize");
	}
}

Application::~Application()
{
	if (_windowHandle)
		glfwDestroyWindow(_windowHandle);
	glfwTerminate();
}

void Application::step()
{
	glfwPollEvents();

	int width, height;
	glfwGetFramebufferSize(_windowHandle, &width, &height);

	if (width != _swapChainSize.first && height != _swapChainSize.second)
	{
		resizeSwapChain(_windowHandle, width, height);
	}

	// Allow to update the state of objects before waiting for the GPU
	updateFrame();
	
	auto back_buffer = _swapChain.GetCurrentTextureView();
	renderFrame(back_buffer.Get());
#ifndef VCL_ARCH_WEBASM
	_swapChain.Present();
#endif
	wgpuQueueSignal(wgpuDeviceGetDefaultQueue(_wgpuDevice), _swapChainFence, ++_frameCounter);
}

void mainLoop(void* self)
{
	reinterpret_cast<Application*>(self)->step();
}

int Application::run()
{
	// Show the window
	if (_windowHandle)
		glfwShowWindow(_windowHandle);

#ifdef VCL_ARCH_WEBASM
	emscripten_set_main_loop_arg(mainLoop, this, 0, false);
#else
	while (!glfwWindowShouldClose(_windowHandle))
		step();
#endif

	return EXIT_SUCCESS;
}

void Application::invalidateDeviceObjects()
{
}

void Application::createDeviceObjects()
{
}

bool Application::initWebGpu(GLFWwindow* window)
{
#ifdef VCL_ARCH_WEBASM
	_wgpuDevice = emscripten_webgpu_get_device();
	wgpuDeviceSetUncapturedErrorCallback(_wgpuDevice, printDeviceError, nullptr);

	// Use C++ wrapper due to malbehaviour in Emscripten.
	// Some offset computation for wgpuInstanceCreateSurface in JavaScript
	// seem to be inline with struct alignments in the C++ structure
	wgpu::SurfaceDescriptorFromCanvasHTMLSelector html_surface_desc{};
	html_surface_desc.selector = "#canvas";

	wgpu::SurfaceDescriptor surface_desc{};
	surface_desc.nextInChain = &html_surface_desc;

	// Use 'null' instance
	wgpu::Instance instance{};
	_wgpuSurface = instance.CreateSurface(&surface_desc).Release();
#else
	_wgpuInstance = std::make_unique<dawn_native::Instance>();
#ifdef VCL_DEBUG
	_wgpuInstance->EnableBackendValidation(true);
#endif
	_wgpuInstance->DiscoverDefaultAdapters();
	dawn_native::Adapter adapter = _wgpuInstance->GetDefaultAdapter();
	_wgpuDevice = adapter.CreateDevice();

	DawnProcTable procs = dawn_native::GetProcs();
	dawnProcSetProcs(&procs);
	wgpuDeviceSetUncapturedErrorCallback(_wgpuDevice, printDeviceError, nullptr);

	HWND hWnd = glfwGetWin32Window(window);

	WGPUSurfaceDescriptorFromWindowsHWND hwnd_surface_desc = {};
	hwnd_surface_desc.chain.sType = WGPUSType_SurfaceDescriptorFromWindowsHWND;
	hwnd_surface_desc.hinstance = nullptr;
	hwnd_surface_desc.hwnd = hWnd;

	WGPUSurfaceDescriptor surface_desc = {};
	surface_desc.nextInChain = reinterpret_cast<WGPUChainedStruct*>(&hwnd_surface_desc);
	_wgpuSurface = wgpuInstanceCreateSurface(_wgpuInstance->Get(), &surface_desc);
#endif

	WGPUFenceDescriptor fence_desc = { nullptr, nullptr, 0 };
	_swapChainFence = wgpuQueueCreateFence(wgpuDeviceGetDefaultQueue(_wgpuDevice), &fence_desc);

	return true;
}

void Application::resizeSwapChain(GLFWwindow* window, unsigned int width, unsigned int height)
{
#ifndef VCL_ARCH_WEBASM
	// Wait for the last pending presentation
	while (wgpuFenceGetCompletedValue(_swapChainFence) < _frameCounter)
		wgpuDeviceTick(_wgpuDevice);
#endif

	invalidateDeviceObjects();

	_swapChainSize = { width, height };

#ifdef VCL_ARCH_WEBASM
	WGPUSwapChainDescriptor desc = {};
	desc.usage = WGPUTextureUsage_OutputAttachment;
	desc.format = WGPUTextureFormat_RGBA8Unorm;
	desc.width = width;
	desc.height = height;
	desc.presentMode = WGPUPresentMode_Fifo;
	_swapChain = wgpu::SwapChain::Acquire(wgpuDeviceCreateSwapChain(_wgpuDevice, _wgpuSurface, &desc));

#else
	_swapChainImpl = dawn_native::d3d12::CreateNativeSwapChainImpl(_wgpuDevice, glfwGetWin32Window(window));
	WGPUSwapChainDescriptor desc = {};
	desc.implementation = reinterpret_cast<uint64_t>(&_swapChainImpl);
	_swapChain = wgpu::SwapChain::Acquire(wgpuDeviceCreateSwapChain(_wgpuDevice, nullptr, &desc));

	wgpuSwapChainConfigure
	(
		_swapChain.Get(),
		dawn_native::d3d12::GetNativeSwapChainPreferredFormat(&_swapChainImpl),
		WGPUTextureUsage_OutputAttachment,
		_swapChainSize.first, _swapChainSize.second
	);
#endif
	createDeviceObjects();
}
