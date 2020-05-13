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
        void idleDevice();
    private:
        bool createInstance();
        const vk::Instance& getInstance() {return instance_.get();}
        static bool hasLayer(const std::vector<vk::LayerProperties>& layerProperties, const std::string& layerName);
        bool pickPhysicalDevice(unsigned short deviceIndex = 0);
        bool createDevice(size_t graphicsQueueIndex);
        bool createSurface();
        static uint32_t findGraphicsQueueFamilyIndex(std::vector<vk::QueueFamilyProperties> const& queueFamilyProperties);
        static std::pair<uint32_t, uint32_t> findGraphicsAndPresentQueueFamilyIndex(vk::PhysicalDevice physicalDevice, const vk::SurfaceKHR& surface);
        bool createSwapChain(); // looks like this is what is considered a "buffer as part of the double buffer in openGL"
        bool createRenderPass();
        bool createGraphicsPipeline();
        bool createFrameBuffers();
        bool createSyncObjects();
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
        std::vector<vk::UniqueCommandBuffer> commandBuffers_;
        std::vector<vk::UniqueSemaphore> imageAvailableSempahores_;
        std::vector<vk::UniqueSemaphore> renderingFinishedSempahores_;
        std::vector<vk::UniqueFence> inFlightFences_;
        vk::UniqueRenderPass renderPass_;
        vk::UniquePipelineLayout pipelineLayout_;
        vk::UniquePipeline pipeline_;
        std::vector<vk::UniqueFramebuffer> swapChainFramebuffers_;
        const int maxFramesInFlight_ = 2;
        size_t currentFrame_ = 0;
    };
}
