/*
 * vk_mvk_macos_surface.h
 *
 * Copyright (c) 2014-2016 The Brenwill Workshop Ltd. All rights reserved.
 * http://www.brenwill.com
 *
 * Use of this document is governed by the Molten License Agreement, as included
 * in the MoltenVK distribution package. CAREFULLY READ THAT LICENSE AGREEMENT BEFORE
 * READING AND USING THIS DOCUMENT. BY READING OR OTHERWISE USING THIS DOCUMENT,
 * YOU ACCEPT AND AGREE TO BE BOUND BY THE TERMS AND CONDITIONS OF THAT LICENSE
 * AGREEMENT. IF YOU DO NOT ACCEPT THE TERMS AND CONDITIONS OF THAT LICENSE AGREEMENT,
 * DO NOT READ OR USE THIS DOCUMENT.
 */


/**
 * Vulkan extension VK_MVK_macos_surface.
 *
 * This extension supports creating Vulkan surfaces on macOS devices
 * for the MoltenVK implementation of Vulkan.
 *
 * This extension requires the VK_KHR_surface extension.
 */

#ifndef __vk_mvk_macos_surface_h_
#define __vk_mvk_macos_surface_h_ 1

#ifdef __cplusplus
extern "C" {
#endif	//  __cplusplus
	
#include <MoltenVK/vulkan/vulkan.h>


#ifdef VK_USE_PLATFORM_MACOS_MVK

#define VK_MVK_macos_surface								1

#define VK_MVK_MACOS_SURFACE_SPEC_VERSION					1
#define VK_MVK_MACOS_SURFACE_EXTENSION_NAME                 "VK_MVK_macos_surface"
#define VK_MVK_MACOS_SURFACE_EXTENSION_NUMBER				53
#define	VK_STRUCTURE_TYPE_MACOS_SURFACE_CREATE_INFO_MVK     ((VkStructureType)(1000000000 + (VK_MVK_MACOS_SURFACE_EXTENSION_NUMBER * 1000)))


/** Flags used to create a WSI surface on the macOS platform. */
typedef VkFlags VkMacOSSurfaceCreateFlagsMVK;

/**
 * The parameters affecting creation of a WSI surface on the macOS platform.
 *
 *   - sType is the type of this structure. This value must be VK_STRUCTURE_TYPE_MACOS_SURFACE_CREATE_INFO_MVK.
 *   - pNext is NULL or a pointer to an extension-specific structure.
 *   - flags is reserved for future use. This value must be zero.
 *   - pView is a reference to a NSView object which will display this surface. 
 *           Under MoltenVK, this NSView must be backed by a CALayer instance of type CAMetalLayer.
 */
typedef struct VkMacOSSurfaceCreateInfoMVK {
	VkStructureType                         sType;
	const void*                             pNext;
	VkMacOSSurfaceCreateFlagsMVK            flags;
	const void*                             pView;
} VkMacOSSurfaceCreateInfoMVK;

typedef VkResult (VKAPI_PTR *PFN_vkCreateMacOSSurfaceMVK)(VkInstance instance, const VkMacOSSurfaceCreateInfoMVK* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);

#ifndef VK_NO_PROTOTYPES

/**
 * Function to create a VkSurfaceKHR for an macOS NSView.
 *
 *   - instance is the VkInstance to associate the surface with.
 *   - pCreateInfo is a pointer to an instance of the VkMacOSSurfaceCreateInfoMVK
 *     structure containing the parameters affecting the creation of the surface object.
 *   - pAllocator is the allocator used for host memory allocated for the surface object
 *     when there is no more specific allocator available (see Memory Allocation).
 *     Under MoltenVK, this parameter is ignored, and may be NULL.
 *
 * The resulting surface object handle is returned in pSurface.
 */
VKAPI_ATTR VkResult VKAPI_CALL vkCreateMacOSSurfaceMVK(
	VkInstance                              instance,
	const VkMacOSSurfaceCreateInfoMVK*      pCreateInfo,
	const VkAllocationCallbacks*            pAllocator,
	VkSurfaceKHR*                           pSurface);

#endif	// VK_NO_PROTOTYPES

#endif	// VK_USE_PLATFORM_MACOS_MVK


#ifdef __cplusplus
}
#endif	//  __cplusplus

#endif
