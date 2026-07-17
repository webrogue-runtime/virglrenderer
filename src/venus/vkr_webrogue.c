/*
 * Copyright 2020 Google LLC
 * SPDX-License-Identifier: MIT
 */

#include "vkr_webrogue.h"

#include "vkr_context.h"
#include "vkr_instance.h"
#include "vkr_image.h"
#include "vkr_webrogue_gen.h"
#include "vkr_physical_device.h"
#include "vkr_queue.h"

#include "venus-protocol/vn_protocol_renderer_device.h"
#include "venus-protocol/vn_protocol_renderer_queue.h"
#include "venus-protocol/vn_protocol_renderer_transport.h"

static void
vkr_dispatch_vkCreateSurfaceWEBROGUE(
   struct vn_dispatch_context *dispatch,
   struct vn_command_vkCreateSurfaceWEBROGUE *args)
{
   struct vkr_context *ctx = dispatch->data;
   struct vkr_surface *obj = vkr_surface_create(ctx, args);
   if (obj)
      vkr_context_add_object(ctx, &obj->base);
}

static void
vkr_dispatch_vkGetPhysicalDeviceSurfaceSupportKHR(
   UNUSED struct vn_dispatch_context *dispatch,
   struct vn_command_vkGetPhysicalDeviceSurfaceSupportKHR *args)
{
   struct vkr_physical_device *physical_dev =
      vkr_physical_device_from_handle(args->physicalDevice);
   struct vn_physical_device_proc_table *vk = &physical_dev->proc_table;

   vn_replace_vkGetPhysicalDeviceSurfaceSupportKHR_args_handle(args);
   args->ret = vk->GetPhysicalDeviceSurfaceSupportKHR(
      args->physicalDevice, args->queueFamilyIndex, args->surface,
      args->pSupported);
}

static void
vkr_dispatch_vkGetPhysicalDeviceSurfaceFormatsKHR(
   UNUSED struct vn_dispatch_context *dispatch,
   struct vn_command_vkGetPhysicalDeviceSurfaceFormatsKHR *args)
{
   struct vkr_physical_device *physical_dev =
      vkr_physical_device_from_handle(args->physicalDevice);
   struct vn_physical_device_proc_table *vk = &physical_dev->proc_table;

   vn_replace_vkGetPhysicalDeviceSurfaceFormatsKHR_args_handle(args);
   args->ret = vk->GetPhysicalDeviceSurfaceFormatsKHR(
      args->physicalDevice, args->surface, args->pSurfaceFormatCount,
      args->pSurfaceFormats);
}

static void
vkr_dispatch_vkGetPhysicalDeviceSurfaceFormats2KHR(
   UNUSED struct vn_dispatch_context *dispatch,
   struct vn_command_vkGetPhysicalDeviceSurfaceFormats2KHR *args)
{
   struct vkr_physical_device *physical_dev =
      vkr_physical_device_from_handle(args->physicalDevice);
   struct vn_physical_device_proc_table *vk = &physical_dev->proc_table;

   vn_replace_vkGetPhysicalDeviceSurfaceFormats2KHR_args_handle(args);
   args->ret = vk->GetPhysicalDeviceSurfaceFormats2KHR(
      args->physicalDevice, args->pSurfaceInfo, args->pSurfaceFormatCount,
      args->pSurfaceFormats);
}

static void
vkr_dispatch_vkGetPhysicalDeviceSurfaceCapabilitiesKHR(
   UNUSED struct vn_dispatch_context *dispatch,
   struct vn_command_vkGetPhysicalDeviceSurfaceCapabilitiesKHR *args)
{
   struct vkr_physical_device *physical_dev =
      vkr_physical_device_from_handle(args->physicalDevice);
   struct vn_physical_device_proc_table *vk = &physical_dev->proc_table;

   vn_replace_vkGetPhysicalDeviceSurfaceCapabilitiesKHR_args_handle(args);
   args->ret = vk->GetPhysicalDeviceSurfaceCapabilitiesKHR(
      args->physicalDevice, args->surface, args->pSurfaceCapabilities);
}

static void
vkr_dispatch_vkGetPhysicalDeviceSurfaceCapabilities2KHR(
   UNUSED struct vn_dispatch_context *dispatch,
   struct vn_command_vkGetPhysicalDeviceSurfaceCapabilities2KHR *args)
{
   struct vkr_physical_device *physical_dev =
      vkr_physical_device_from_handle(args->physicalDevice);
   struct vn_physical_device_proc_table *vk = &physical_dev->proc_table;

   vn_replace_vkGetPhysicalDeviceSurfaceCapabilities2KHR_args_handle(args);
   args->ret = vk->GetPhysicalDeviceSurfaceCapabilities2KHR(
      args->physicalDevice, args->pSurfaceInfo, args->pSurfaceCapabilities);
}

static void
vkr_dispatch_vkCreateSwapchainKHR(
   struct vn_dispatch_context *dispatch,
   struct vn_command_vkCreateSwapchainKHR *args)
{
   struct vkr_context *ctx = dispatch->data;
   struct vkr_device *dev = vkr_device_from_handle(args->device);
   struct vn_device_proc_table *vk = &dev->proc_table;
   struct vkr_swapchain *obj = vkr_context_alloc_object(
      ctx, sizeof(*obj), VK_OBJECT_TYPE_SWAPCHAIN_KHR, args->pSwapchain);
   if (!obj) {
      args->ret = VK_ERROR_OUT_OF_HOST_MEMORY;
      return;
   }

   vn_replace_vkCreateSwapchainKHR_args_handle(args);
   args->ret = vk->CreateSwapchainKHR(
      args->device, args->pCreateInfo, NULL, &obj->base.handle.swapchain);
   if (args->ret != VK_SUCCESS) {
      free(obj);
      return;
   }

   list_inithead(&obj->images);
   vkr_device_add_object(ctx, dev, &obj->base);
}

static void
vkr_dispatch_vkDestroySwapchainKHR(
   struct vn_dispatch_context *dispatch,
   struct vn_command_vkDestroySwapchainKHR *args)
{
   struct vkr_context *ctx = dispatch->data;
   struct vkr_device *dev = vkr_device_from_handle(args->device);
   struct vkr_swapchain *obj = vkr_swapchain_from_handle(args->swapchain);
   struct vn_device_proc_table *vk = &dev->proc_table;

   list_for_each_entry_safe (struct vkr_image, image, &obj->images,
                             swapchain_link) {
      list_delinit(&image->swapchain_link);
      vkr_device_remove_object(ctx, dev, &image->base);
   }

   vn_replace_vkDestroySwapchainKHR_args_handle(args);
   vk->DestroySwapchainKHR(args->device, args->swapchain, NULL);
   vkr_device_remove_object(ctx, dev, &obj->base);
}

static void
vkr_dispatch_vkAcquireNextImage2KHR(
   UNUSED struct vn_dispatch_context *dispatch,
   struct vn_command_vkAcquireNextImage2KHR *args)
{
   struct vkr_device *dev = vkr_device_from_handle(args->device);
   struct vn_device_proc_table *vk = &dev->proc_table;

   vn_replace_vkAcquireNextImage2KHR_args_handle(args);
   args->ret = vk->AcquireNextImage2KHR(
      args->device, args->pAcquireInfo, args->pImageIndex);
}

static void
vkr_dispatch_vkQueuePresentKHR(
   UNUSED struct vn_dispatch_context *dispatch,
   struct vn_command_vkQueuePresentKHR *args)
{
   struct vkr_queue *queue = vkr_queue_from_handle(args->queue);
   struct vn_device_proc_table *vk = &queue->device->proc_table;

   vn_replace_vkQueuePresentKHR_args_handle(args);
   args->ret = vk->QueuePresentKHR(args->queue, args->pPresentInfo);
}

static void
vkr_dispatch_vkGetSwapchainImagesKHR(
   struct vn_dispatch_context *dispatch,
   struct vn_command_vkGetSwapchainImagesKHR *args)
{
   struct vkr_device *dev = vkr_device_from_handle(args->device);
   struct vkr_swapchain *swapchain = vkr_swapchain_from_handle(args->swapchain);
   struct vn_device_proc_table *vk = &dev->proc_table;
   VkImage *images = NULL;
   uint32_t image_count = args->pSwapchainImageCount
                             ? *args->pSwapchainImageCount
                             : 0;

   vn_replace_vkGetSwapchainImagesKHR_args_handle(args);

   if (args->pSwapchainImages && image_count) {
      images = calloc(image_count, sizeof(*images));
      if (!images) {
         args->ret = VK_ERROR_OUT_OF_HOST_MEMORY;
         return;
      }
   }

   args->ret = vk->GetSwapchainImagesKHR(
      args->device, args->swapchain, args->pSwapchainImageCount, images);
   if (args->ret == VK_SUCCESS || args->ret == VK_INCOMPLETE) {
      struct vkr_context *ctx = dispatch->data;
      for (uint32_t i = 0; i < image_count; i++) {
         struct vkr_image *obj = vkr_context_alloc_object(
            ctx, sizeof(*obj), VK_OBJECT_TYPE_IMAGE,
            &args->pSwapchainImages[i]);
         if (!obj) {
            args->ret = VK_ERROR_OUT_OF_HOST_MEMORY;
            break;
         }
         obj->base.handle.image = images[i];
         obj->swapchain_owned = true;
         list_addtail(&obj->swapchain_link, &swapchain->images);
         vkr_device_add_object(ctx, dev, &obj->base);
      }
   }

   free(images);
}

static void
vkr_dispatch_vkGetPhysicalDeviceSurfacePresentModesKHR(
   UNUSED struct vn_dispatch_context *dispatch,
   struct vn_command_vkGetPhysicalDeviceSurfacePresentModesKHR *args)
{
   struct vkr_physical_device *physical_dev =
      vkr_physical_device_from_handle(args->physicalDevice);
   struct vn_physical_device_proc_table *vk = &physical_dev->proc_table;

   vn_replace_vkGetPhysicalDeviceSurfacePresentModesKHR_args_handle(args);
   args->ret = vk->GetPhysicalDeviceSurfacePresentModesKHR(
      args->physicalDevice, args->surface, args->pPresentModeCount,
      args->pPresentModes);
}

void
vkr_context_init_webrogue_dispatch(struct vkr_context *ctx)
{
   struct vn_dispatch_context *dispatch = &ctx->dispatch;

   dispatch->dispatch_vkCreateSurfaceWEBROGUE = vkr_dispatch_vkCreateSurfaceWEBROGUE;
   dispatch->dispatch_vkGetPhysicalDeviceSurfaceSupportKHR =
      vkr_dispatch_vkGetPhysicalDeviceSurfaceSupportKHR;
   dispatch->dispatch_vkGetPhysicalDeviceSurfaceFormatsKHR =
      vkr_dispatch_vkGetPhysicalDeviceSurfaceFormatsKHR;
   dispatch->dispatch_vkGetPhysicalDeviceSurfaceFormats2KHR =
      vkr_dispatch_vkGetPhysicalDeviceSurfaceFormats2KHR;
   dispatch->dispatch_vkGetPhysicalDeviceSurfaceCapabilitiesKHR =
      vkr_dispatch_vkGetPhysicalDeviceSurfaceCapabilitiesKHR;
   dispatch->dispatch_vkGetPhysicalDeviceSurfaceCapabilities2KHR =
      vkr_dispatch_vkGetPhysicalDeviceSurfaceCapabilities2KHR;
   dispatch->dispatch_vkGetPhysicalDeviceSurfacePresentModesKHR =
      vkr_dispatch_vkGetPhysicalDeviceSurfacePresentModesKHR;
   dispatch->dispatch_vkCreateSwapchainKHR = vkr_dispatch_vkCreateSwapchainKHR;
   dispatch->dispatch_vkDestroySwapchainKHR = vkr_dispatch_vkDestroySwapchainKHR;
   dispatch->dispatch_vkAcquireNextImage2KHR =
      vkr_dispatch_vkAcquireNextImage2KHR;
   dispatch->dispatch_vkQueuePresentKHR = vkr_dispatch_vkQueuePresentKHR;
   dispatch->dispatch_vkGetSwapchainImagesKHR =
      vkr_dispatch_vkGetSwapchainImagesKHR;
}
