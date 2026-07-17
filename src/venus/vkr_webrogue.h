/*
 * Copyright 2020 Google LLC
 * SPDX-License-Identifier: MIT
 */

#ifndef VKR_WEBROGUE_H
#define VKR_WEBROGUE_H

#include "vkr_common.h"

/* Pops the webrogue pending-shmem slot (set by webrogue_virgl_setup_shmem) if
 * any, returning NULL when empty. */
void *webrogue_virgl_pop_shmem(size_t *out_size);

struct vkr_surface {
   struct vkr_object base;
};
VKR_DEFINE_OBJECT_CAST(surface, VK_OBJECT_TYPE_SURFACE_KHR, VkSurfaceKHR)

struct vkr_swapchain {
   struct vkr_object base;
   struct list_head images;
};
VKR_DEFINE_OBJECT_CAST(swapchain, VK_OBJECT_TYPE_SWAPCHAIN_KHR, VkSwapchainKHR)

void
vkr_context_init_webrogue_dispatch(struct vkr_context *ctx);

#endif /* VKR_WEBROGUE_H */
