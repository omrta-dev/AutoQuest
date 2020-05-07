//
// Created by omar on 3/21/20.
//

#include "Vulkanize.hpp"
#include <SDL2/SDL_vulkan.h>
#include <iostream>

#include "Shaders.hpp"

bool aik::Vulkanize::setup(SDL_Window * window)
{
    window_ = window;
    if (!createInstance()) return false;
    if (!pickPhysicalDevice()) return false;
    if (!createSurface()) return false;
    if (!createSwapChain()) return false;
    if (!createSemaphores()) return false;
    if (!createRenderPass()) return false;
    if (!createGraphicsPipeline()) return false;
    if (!createFrameBuffers()) return false;
    if (!createCommandBuffer()) return false;
    if (!recordCommandBuffer()) return false;

    std::cout << "All Good!" << std::endl;
    return true;
}

bool aik::Vulkanize::createInstance()
{
    vk::ApplicationInfo applicationInfo("AutoQuest", 1, "All I Know", 1, VK_API_VERSION_1_1);

    vk::InstanceCreateInfo instanceCreateInfo({}, &applicationInfo);

    // get supported extensions
    uint32_t extensionCount;
    SDL_Vulkan_GetInstanceExtensions(window_, &extensionCount, nullptr);
    std::vector<const char *> extensionNames(extensionCount);
    SDL_Vulkan_GetInstanceExtensions(window_, &extensionCount, extensionNames.data());

    // enable those extensions
    instanceCreateInfo.setEnabledExtensionCount(extensionCount);
    instanceCreateInfo.setPpEnabledExtensionNames(extensionNames.data());

    #ifndef NDEBUG
        std::cout << "DEBUG MODE: True" << std::endl;
        std::vector<const char *> layerPropertyNames;
        std::vector<vk::LayerProperties> layerProperties = vk::enumerateInstanceLayerProperties();
        if(hasLayer(layerProperties, "VK_LAYER_LUNARG_api_dump"))
            layerPropertyNames.push_back("VK_LAYER_LUNARG_api_dump");
        if(hasLayer(layerProperties, "VK_LAYER_KHRONOS_validation"))
            layerPropertyNames.push_back("VK_LAYER_KHRONOS_validation");
        if(hasLayer(layerProperties, "VK_LAYER_LUNARG_standard_validation"))
            layerPropertyNames.push_back("VK_LAYER_LUNARG_standard_validation");
        instanceCreateInfo.setEnabledLayerCount(layerPropertyNames.size());
        instanceCreateInfo.setPpEnabledLayerNames(layerPropertyNames.data());
    #endif

    instance_ = vk::createInstanceUnique(instanceCreateInfo);

    if(instance_)
    {
        return true;
    }

    return false;
}

bool aik::Vulkanize::hasLayer(const std::vector<vk::LayerProperties> &layerProperties, std::string layerName)
{
    for(const auto& layerProperty : layerProperties)
    {
        if(layerProperty.layerName.data() == layerName)
            return true;
    }
    return false;
}

bool aik::Vulkanize::pickPhysicalDevice(unsigned short deviceIndex)
{
    physicalDevice_ = instance_->enumeratePhysicalDevices()[deviceIndex];

    std::vector<vk::QueueFamilyProperties> queueFamilyProperties = physicalDevice_.getQueueFamilyProperties();
    // get the first index into queueFamiliyProperties which supports graphics
    size_t graphicsQueueFamilyIndex = std::distance(queueFamilyProperties.begin(),
                                                    std::find_if(queueFamilyProperties.begin(),
                                                                 queueFamilyProperties.end(),
                                                                 [](vk::QueueFamilyProperties const& qfp) { return qfp.queueFlags & vk::QueueFlagBits::eGraphics; }));

    return createDevice(graphicsQueueFamilyIndex);
}

bool aik::Vulkanize::createDevice(size_t graphicsQueueIndex)
{
    // create a UniqueDevice
    float queuePriority = 0.0f;
    std::vector<const char*> requestedExtensions = {"VK_KHR_swapchain"};
    vk::DeviceQueueCreateInfo deviceQueueCreateInfo(vk::DeviceQueueCreateFlags(), static_cast<uint32_t>(graphicsQueueIndex), 1, &queuePriority);
    device_ = physicalDevice_.createDeviceUnique(vk::DeviceCreateInfo(vk::DeviceCreateFlags(), 1, &deviceQueueCreateInfo, {}, {}, requestedExtensions.size(), requestedExtensions.data()));
    return device_.get();
}

bool aik::Vulkanize::createSurface()
{
    VkSurfaceKHR surface;
    if (!SDL_Vulkan_CreateSurface(window_, instance_.get(), &surface))
    {
        std::cout << "Failed to create surface" << std::endl;
        std::cout << "SDL2 Error: " << SDL_GetError() << "\n";
        std::exit(1);
    }
    surface_ = vk::UniqueSurfaceKHR(surface, instance_.get());
    return true;
}

uint32_t aik::Vulkanize::findGraphicsQueueFamilyIndex(std::vector<vk::QueueFamilyProperties> const &queueFamilyProperties)
{
    // get the first index into queueFamiliyProperties which supports graphics
    size_t graphicsQueueFamilyIndex = std::distance(queueFamilyProperties.begin(), std::find_if(queueFamilyProperties.begin(), queueFamilyProperties.end(),
                                                                                                [](vk::QueueFamilyProperties const& qfp) { return qfp.queueFlags & vk::QueueFlagBits::eGraphics; }));
    return static_cast<uint32_t>(graphicsQueueFamilyIndex);
}

std::pair<uint32_t, uint32_t> aik::Vulkanize::findGraphicsAndPresentQueueFamilyIndex(vk::PhysicalDevice physicalDevice, const vk::SurfaceKHR& surface)
{
    std::vector<vk::QueueFamilyProperties> queueFamilyProperties = physicalDevice.getQueueFamilyProperties();

    uint32_t graphicsQueueFamilyIndex = findGraphicsQueueFamilyIndex(queueFamilyProperties);
    if (physicalDevice.getSurfaceSupportKHR(graphicsQueueFamilyIndex, surface))
    {
        return std::make_pair(graphicsQueueFamilyIndex,
                              graphicsQueueFamilyIndex);    // the first graphicsQueueFamilyIndex does also support presents
    }

    // the graphicsQueueFamilyIndex doesn't support present -> look for an other family index that supports both graphics and present
    for (size_t i = 0; i < queueFamilyProperties.size(); i++)
    {
        if ((queueFamilyProperties[i].queueFlags & vk::QueueFlagBits::eGraphics) &&
            physicalDevice.getSurfaceSupportKHR(static_cast<uint32_t>(i), surface))
        {
            return std::make_pair(static_cast<uint32_t>(i), static_cast<uint32_t>(i));
        }
    }
    return std::make_pair(-1, -1);
}

bool aik::Vulkanize::createSwapChain()
{
    // get the supported VkFormats
    std::vector<vk::SurfaceFormatKHR> formats = physicalDevice_.getSurfaceFormatsKHR(surface_.get());
    swapChainImageFormat_ = (formats[0].format == vk::Format::eUndefined) ? vk::Format::eB8G8R8A8Unorm : formats[0].format;

    vk::SurfaceCapabilitiesKHR surfaceCapabilities = physicalDevice_.getSurfaceCapabilitiesKHR(surface_.get());
    if (surfaceCapabilities.currentExtent.width == std::numeric_limits<uint32_t>::max())
    {
        // If the surface size is undefined, the size is set to the size of the window
        int width, height;
        SDL_GetWindowSize(window_, &width, &height);
        swapChainExtent_.width = width;
        swapChainExtent_.height = height;
    }
    else
    {
        // If the surface size is defined, the swap chain size must match
        swapChainExtent_ = surfaceCapabilities.currentExtent;
    }

    // The FIFO present mode is guaranteed by the spec to be supported
    vk::PresentModeKHR swapchainPresentMode = vk::PresentModeKHR::eFifo;

    // query for triple buffer support
    std::vector<vk::PresentModeKHR> presentModes = physicalDevice_.getSurfacePresentModesKHR(surface_.get());
    for(const auto& presentMode : presentModes)
    {
        if (presentMode == vk::PresentModeKHR::eMailbox)
        {
            swapchainPresentMode = presentMode;
            break;
        }
    }

    vk::SurfaceTransformFlagBitsKHR preTransform = (surfaceCapabilities.supportedTransforms & vk::SurfaceTransformFlagBitsKHR::eIdentity) ? vk::SurfaceTransformFlagBitsKHR::eIdentity : surfaceCapabilities.currentTransform;

    vk::CompositeAlphaFlagBitsKHR compositeAlpha =
            (surfaceCapabilities.supportedCompositeAlpha & vk::CompositeAlphaFlagBitsKHR::ePreMultiplied) ? vk::CompositeAlphaFlagBitsKHR::ePreMultiplied :
            (surfaceCapabilities.supportedCompositeAlpha & vk::CompositeAlphaFlagBitsKHR::ePostMultiplied) ? vk::CompositeAlphaFlagBitsKHR::ePostMultiplied :
            (surfaceCapabilities.supportedCompositeAlpha & vk::CompositeAlphaFlagBitsKHR::eInherit) ? vk::CompositeAlphaFlagBitsKHR::eInherit : vk::CompositeAlphaFlagBitsKHR::eOpaque;

    vk::SwapchainCreateInfoKHR swapChainCreateInfo(vk::SwapchainCreateFlagsKHR(), surface_.get(), surfaceCapabilities.minImageCount, swapChainImageFormat_, vk::ColorSpaceKHR::eSrgbNonlinear,
                                                   swapChainExtent_, 1, vk::ImageUsageFlagBits::eColorAttachment, vk::SharingMode::eExclusive, 0, nullptr, preTransform, compositeAlpha, swapchainPresentMode, true, nullptr);

    auto queueFamilyIndices = findGraphicsAndPresentQueueFamilyIndex(physicalDevice_, surface_.get());

    // create the two graphics and present queue
    graphicsQueue_ = device_->getQueue(queueFamilyIndices.first, 0);
    presentQueue_ = device_->getQueue(queueFamilyIndices.second, 0);

    if (queueFamilyIndices.first != queueFamilyIndices.second)
    {
        // If the graphics and present queues are from different queue families, we either have to explicitly transfer ownership of images between
        // the queues, or we have to create the swapchain with imageSharingMode as VK_SHARING_MODE_CONCURRENT
        swapChainCreateInfo.imageSharingMode = vk::SharingMode::eConcurrent;
        swapChainCreateInfo.queueFamilyIndexCount = 2;
        swapChainCreateInfo.pQueueFamilyIndices = std::vector<uint32_t>{queueFamilyIndices.first, queueFamilyIndices.second}.data();
    }

    swapChain_ = device_->createSwapchainKHRUnique(swapChainCreateInfo);

    swapChainImages_ = device_->getSwapchainImagesKHR(swapChain_.get());

    swapChainImageViews_.reserve(swapChainImages_.size());
    vk::ComponentMapping componentMapping(vk::ComponentSwizzle::eR, vk::ComponentSwizzle::eG, vk::ComponentSwizzle::eB, vk::ComponentSwizzle::eA);
    vk::ImageSubresourceRange subResourceRange(vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1);
    for (auto image : swapChainImages_)
    {
        vk::ImageViewCreateInfo imageViewCreateInfo(vk::ImageViewCreateFlags(), image, vk::ImageViewType::e2D, swapChainImageFormat_, componentMapping, subResourceRange);
        swapChainImageViews_.push_back(device_->createImageViewUnique(imageViewCreateInfo));
    }

    // if we have swapChainImageViews that means that swapchain creation was successful
    return !swapChainImageViews_.empty();
}

bool aik::Vulkanize::createRenderPass()
{
    vk::AttachmentDescription colorAttachment = {{}, swapChainImageFormat_, vk::SampleCountFlagBits::e1, vk::AttachmentLoadOp::eClear, vk::AttachmentStoreOp::eStore,
                                                 vk::AttachmentLoadOp::eDontCare, vk::AttachmentStoreOp::eDontCare, vk::ImageLayout::eUndefined, vk::ImageLayout::ePresentSrcKHR};
    vk::AttachmentReference colorAttachmentRef = {0, vk::ImageLayout::eColorAttachmentOptimal};
    vk::SubpassDescription subpass = {{}, {}, {}, {}, 1, &colorAttachmentRef};
    vk::SubpassDependency dependency = {VK_SUBPASS_EXTERNAL, 0, vk::PipelineStageFlagBits::eColorAttachmentOutput, vk::PipelineStageFlagBits::eColorAttachmentOutput, {}, vk::AccessFlagBits::eColorAttachmentWrite};
    vk::RenderPassCreateInfo renderPassCreateInfo = {{}, 1, &colorAttachment, 1, &subpass, 1, &dependency};
    renderPass_ = device_->createRenderPassUnique(renderPassCreateInfo);
    if(!renderPass_)
    {
        std::cout << "failed to create render pass!" << std::endl;
        return false;
    }
    return true;
}

bool aik::Vulkanize::createGraphicsPipeline()
{
    auto vertexShaderCode = aik::Shaders::loadShader("assets/shaders/vert.vert.spv");
    auto fragmentShaderCode = aik::Shaders::loadShader("assets/shaders/frag.frag.spv");
    auto vertexShaderModule = aik::Shaders::createShaderModule(device_.get(), vertexShaderCode);
    auto fragmentShaderModule = aik::Shaders::createShaderModule(device_.get(), fragmentShaderCode);
    vk::PipelineShaderStageCreateInfo vertexShader({}, vk::ShaderStageFlagBits::eVertex, vertexShaderModule.get(), "main");
    vk::PipelineShaderStageCreateInfo fragmentShader({}, vk::ShaderStageFlagBits::eFragment, fragmentShaderModule.get(), "main");
    vk::PipelineShaderStageCreateInfo shaderStages[] = {vertexShader, fragmentShader};

    vk::PipelineVertexInputStateCreateInfo vertexInputStateCreateInfo = {{}, 0, nullptr, 0, nullptr};
    vk::PipelineInputAssemblyStateCreateInfo assemblyStateCreateInfo = {{}, vk::PrimitiveTopology::eTriangleList, false};
    vk::Viewport viewport = {0.0f, 0.0f, static_cast<float>(swapChainExtent_.width), static_cast<float>(swapChainExtent_.height), 0.0f, 1.0f};
    vk::Rect2D scissor = {{0, 0}, swapChainExtent_};
    vk::PipelineViewportStateCreateInfo viewportStateCreateInfo = {{}, 1, &viewport, 1, &scissor};
    vk::PipelineRasterizationStateCreateInfo rasterizationStateCreateInfo = {{}, false, false, vk::PolygonMode::eFill, vk::CullModeFlagBits::eBack, vk::FrontFace::eClockwise};
    vk::PipelineMultisampleStateCreateInfo multisampleStateCreateInfo = {{}, vk::SampleCountFlagBits::e1};
    vk::PipelineColorBlendAttachmentState colorBlendAttachmentState = {{}, vk::BlendFactor::eSrcAlpha, vk::BlendFactor::eOneMinusSrc1Alpha, vk::BlendOp::eAdd,
                                                                       vk::BlendFactor::eOne, vk::BlendFactor::eZero, vk::BlendOp::eAdd,
                                                                       vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG | vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA};
    vk::PipelineColorBlendStateCreateInfo colorBlendStateCreateInfo = {{}, false, vk::LogicOp::eCopy, 1, &colorBlendAttachmentState, {0.0f, 0.0f, 0.0f, 0.0f} };
    std::vector<vk::DynamicState> dynamicStates = {vk::DynamicState::eViewport, vk::DynamicState::eLineWidth};
    vk::PipelineDynamicStateCreateInfo dynamicStateCreateInfo = {{}, static_cast<uint32_t>(dynamicStates.size()), dynamicStates.data()};
    vk::PipelineLayoutCreateInfo layoutCreateInfo = {{}, 0, nullptr, 0, nullptr};
    pipelineLayout_ = device_->createPipelineLayoutUnique(layoutCreateInfo);

    // create the actual pipeline with all the information from the renderpass and pipeline stages above
    vk::GraphicsPipelineCreateInfo graphicsPipelineCreateInfo = {{}, 2, shaderStages, &vertexInputStateCreateInfo, &assemblyStateCreateInfo, {},
                                                                 &viewportStateCreateInfo, &rasterizationStateCreateInfo, &multisampleStateCreateInfo, {}, &colorBlendStateCreateInfo, {},
                                                                 pipelineLayout_.get(), renderPass_.get(), {}, {}, {}};
    pipeline_ = device_->createGraphicsPipelineUnique(nullptr, graphicsPipelineCreateInfo);
    return true;
}

bool aik::Vulkanize::createFrameBuffers()
{
    swapChainFramebuffers_.resize(swapChainImageViews_.size());
    for(size_t i = 0; i < swapChainImageViews_.size(); i++)
    {
        vk::ImageView attachments[] = {swapChainImageViews_[i].get()};
        vk::FramebufferCreateInfo framebufferCreateInfo = {{}, renderPass_.get(), 1, attachments, swapChainExtent_.width, swapChainExtent_.height, 1};
        swapChainFramebuffers_[i] = device_->createFramebufferUnique(framebufferCreateInfo);
    }
    return true;
}

bool aik::Vulkanize::createSemaphores()
{
    imageAvailableSempahore_ = device_->createSemaphoreUnique(vk::SemaphoreCreateInfo(), nullptr);
    renderingFinishedSempahore_ = device_->createSemaphoreUnique(vk::SemaphoreCreateInfo(), nullptr);
    return true;
}

bool aik::Vulkanize::createCommandBuffer()
{
    commandBuffers_.resize(swapChainImages_.size());
    uint32_t graphicsQueueIndex = findGraphicsQueueFamilyIndex(physicalDevice_.getQueueFamilyProperties());
    commandPool_ = device_->createCommandPoolUnique(vk::CommandPoolCreateInfo(vk::CommandPoolCreateFlags(), graphicsQueueIndex));
    commandBuffers_ = device_->allocateCommandBuffersUnique(vk::CommandBufferAllocateInfo(commandPool_.get(), vk::CommandBufferLevel::ePrimary, static_cast<uint32_t>(swapChainImages_.size())));
    return true;
}

bool aik::Vulkanize::recordCommandBuffer()
{
    vk::ClearColorValue clearColorValue = {std::array<float, 4>{.2f, .4f, .6f, 1.0f}};
    vk::ClearValue clearValue = {clearColorValue};
    vk::CommandBufferBeginInfo commandBufferBeginInfo;
    for(uint i = 0; i < swapChainImages_.size(); i++)
    {
        vk::RenderPassBeginInfo renderPassBeginInfo = {renderPass_.get(), swapChainFramebuffers_[i].get(), {{0, 0}, swapChainExtent_}, 1, &clearValue};

        commandBuffers_[i]->begin(&commandBufferBeginInfo);
        commandBuffers_[i]->beginRenderPass(renderPassBeginInfo, vk::SubpassContents::eInline);
        commandBuffers_[i]->bindPipeline(vk::PipelineBindPoint::eGraphics, pipeline_.get());
        commandBuffers_[i]->draw(3, 1, 0, 0);
        commandBuffers_[i]->endRenderPass();
        commandBuffers_[i]->end();
    }

    return true;
}

bool aik::Vulkanize::renderScene()
{
    auto imageAcquireResults = device_->acquireNextImageKHR(swapChain_.get(), UINT64_MAX, imageAvailableSempahore_.get(), nullptr);
    vk::SubmitInfo submitInfo;
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = &imageAvailableSempahore_.get();
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = &renderingFinishedSempahore_.get();
    vk::PipelineStageFlags pipelineStageFlags(vk::PipelineStageFlagBits::eTransfer);
    submitInfo.setPWaitDstStageMask(&pipelineStageFlags);

    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffers_[imageAcquireResults.value].get();

    presentQueue_.submit(1, &submitInfo, nullptr);

    vk::PresentInfoKHR presentInfoKhr;
    presentInfoKhr.swapchainCount = 1;
    presentInfoKhr.pSwapchains = &swapChain_.get();
    presentInfoKhr.pImageIndices = &imageAcquireResults.value;
    presentInfoKhr.waitSemaphoreCount = 1;
    presentInfoKhr.pWaitSemaphores = &renderingFinishedSempahore_.get();

    presentQueue_.presentKHR(presentInfoKhr);

    return true;
}

void aik::Vulkanize::idleDevice()
{
    device_->waitIdle();
}
