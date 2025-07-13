#pragma once

/*
* Version 0.1.0-alpha
* This is the main header file for the Dark-Render library.
* It includes all the necessary headers for the library.
*/

#define DKRENDER_VERSION "0.1.0-alpha"

#define DKRENDER_VERSION_MAJOR 0
#define DKRENDER_VERSION_MINOR 1
#define DKRENDER_VERSION_PATCH 0

// Include the render graph headers
#include <RenderGraph/RenderGraph.h>
#include <RenderGraph/RenderPass.h>

// Include the render backend headers
#include <RenderBackend/ICommandBuffer.h>
#include <RenderBackend/IRenderBackend.h>
#include <RenderBackend/RenderBackendFactory.h>

// Include the render resources headers
#include <RenderResources/TextureFactory.h>