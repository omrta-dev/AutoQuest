//
// Created by omar on 3/21/20.
//

#pragma once
#include <vulkan/vulkan.hpp>
#include <SDL2/SDL.h>
namespace aik
{
    class Vulkanize
    {
    public:
        bool setup(SDL_Window * window);
        bool renderScene();
    private:
        bool createInstance();
        const vk::Instance& getInstance() {return instance_.get();}
        bool hasLayer(const std::vector<vk::LayerProperties>& layerProperties, std::string layerName);
        bool pickPhysicalDevice(unsigned short deviceIndex = 0);
        bool createDevice(size_t graphicsQueueIndex);
        bool createSurface();
        uint32_t findGraphicsQueueFamilyIndex(std::vector<vk::QueueFamilyProperties> const& queueFamilyProperties);
        std::pair<uint32_t, uint32_t> findGraphicsAndPresentQueueFamilyIndex(vk::PhysicalDevice physicalDevice, const vk::SurfaceKHR& surface);
        bool createSwapChain(); // looks like this is what is considered a "buffer as part of the double buffer in openGL"
        bool createGraphicsPipeline();
        bool createSemaphores();
        bool createCommandBuffer();
        bool recordCommandBuffer();

        SDL_Window * window_;
        vk::UniqueInstance instance_;
        vk::PhysicalDevice physicalDevice_;
        vk::UniqueDevice  device_;
        vk::UniqueSurfaceKHR surface_;
        vk::Queue graphicsQueue_;
        vk::Queue presentQueue_;
        vk::UniqueSwapchainKHR swapChain_;
        std::vector<vk::Image> swapChainImages_;
        std::vector<vk::UniqueImageView> swapChainImageViews_;
        vk::Format swapChainImageFormat_;
        vk::Extent2D swapChainExtent_;
        vk::UniqueCommandPool commandPool_;
        std::vector<vk::UniqueCommandBuffer> commandBuffers;
        vk::UniqueSemaphore imageAvailableSempahore_;
        vk::UniqueSemaphore renderingFinishedSempahore_;
    };
}
