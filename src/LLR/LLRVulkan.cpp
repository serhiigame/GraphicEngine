#include "LLRVulkan.h"

//#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
//#include <vulkan/vulkan.h>

#include "ShaderVulkan.h"

#include <algorithm>
#include <iostream>
#include <fstream>
#include <set>

namespace engine
{
	namespace graphic
	{
		/*const std::vector<const char*> validationLayers = {
			"VK_LAYER_LUNARG_standard_validation",
		};

		static VkResult CreateDebugReportCallbackEXT(VkInstance instance, const VkDebugReportCallbackCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugReportCallbackEXT* pCallback) {
			auto func = (PFN_vkCreateDebugReportCallbackEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugReportCallbackEXT");
			if (func != nullptr) {
				return func(instance, pCreateInfo, pAllocator, pCallback);
			}
			else {
				return VK_ERROR_EXTENSION_NOT_PRESENT;
			}
		}
		static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objType, uint64_t obj, size_t location, int32_t code, const char* layerPrefix, const char* msg, void* userData) {
			std::cerr << "validation layer: " << msg << std::endl;
			return VK_FALSE;
		}


		bool checkValidationLayerSupport() {
			uint32_t layerCount;
			vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

			std::vector<VkLayerProperties> availableLayers(layerCount);
			vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

			for (const char* layerName : validationLayers) {
				bool layerFound = false;

				for (const auto& layerProperties : availableLayers) {
					if (strcmp(layerName, layerProperties.layerName) == 0) {
						layerFound = true;
						break;
					}
				}

				if (!layerFound) {
					return false;
				}
			}

			return true;
		}

		class VulcanImpl
		{
		public:
			bool CreateInstance()
			{
				
				
				checkValidationLayerSupport();

				VkApplicationInfo appInfo = {};
				appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
				appInfo.pApplicationName = "Hello Triangle";
				appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
				appInfo.pEngineName = "No Engine";
				appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
				appInfo.apiVersion = VK_API_VERSION_1_0;

				VkInstanceCreateInfo createInfo = {};
				createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
				createInfo.pApplicationInfo = &appInfo;

				unsigned int glfwExtensionCount = 0;
				const char** glfwExtensions;

				glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);


				const char* test[] = {
					glfwExtensions[0],
					glfwExtensions[1],
					VK_EXT_DEBUG_REPORT_EXTENSION_NAME
				};

				createInfo.enabledExtensionCount = sizeof(test) / sizeof(test[0]);
				createInfo.ppEnabledExtensionNames = test;
				createInfo.enabledLayerCount = 0;

				createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
				createInfo.ppEnabledLayerNames = validationLayers.data();

				VkResult result = vkCreateInstance(&createInfo, nullptr, &m_instance);

				if (vkCreateInstance(&createInfo, nullptr, &m_instance) != VK_SUCCESS) {
					// Log Error
					return false;
				}

				uint32_t extensionCount = 0;
				vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

				std::vector<VkExtensionProperties> extensions(extensionCount);

				vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

				return true;
			}

			void DestroyInstance()
			{
				if (m_instance != VK_NULL_HANDLE)
				{
					vkDestroyInstance(m_instance, nullptr);
				}
			}

			bool SetupDebugCallback()
			{

				VkDebugReportCallbackCreateInfoEXT createInfo = {};
				createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
				createInfo.flags = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT;
				createInfo.pfnCallback = debugCallback;
				if (CreateDebugReportCallbackEXT(m_instance, &createInfo, nullptr, &m_debugCallback) != VK_SUCCESS) {
					false;
				}

				return true;
			}

			bool CreateSurface(IWindow * window)
			{
				return glfwCreateWindowSurface(m_instance, window->Window, nullptr, &m_surface) == VK_SUCCESS;
			}

			bool PickPhysicalDevice()
			{
				uint32_t deviceCount = 0;
				vkEnumeratePhysicalDevices(m_instance, &deviceCount, nullptr);

				if (deviceCount == 0) {
					return false;
				}

				std::vector<VkPhysicalDevice> devices(deviceCount);
				vkEnumeratePhysicalDevices(m_instance, &deviceCount, devices.data());

				//TODO: select the best one
				if(deviceCount > 0)
				{
					m_phisicalDevice = devices[0];
				}

				uint32_t queueFamilyCount = 0;
				vkGetPhysicalDeviceQueueFamilyProperties(m_phisicalDevice, &queueFamilyCount, nullptr);

				{
					std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
					vkGetPhysicalDeviceQueueFamilyProperties(m_phisicalDevice, &queueFamilyCount, queueFamilies.data());

					{
						auto finded = std::find_if(queueFamilies.begin(), queueFamilies.end(), [](const VkQueueFamilyProperties & p) {
							return p.queueFlags & VK_QUEUE_GRAPHICS_BIT;
						});


						if (finded == queueFamilies.end())
						{
							return false;
						}
						else
						{
							m_graphicFamily = static_cast<int>( finded - queueFamilies.begin());
						}
					}
					{
						for(int i = 0; i < queueFamilies.size(); ++i)
						{
							VkBool32 presentSupport = false;
							vkGetPhysicalDeviceSurfaceSupportKHR(m_phisicalDevice, i, m_surface, &presentSupport);

							if (queueFamilies[i].queueCount > 0 && presentSupport)
							{
								m_presentFamily = i;
								break;
							}
						}
						auto finded = std::find_if(queueFamilies.begin(), queueFamilies.end(), [](const VkQueueFamilyProperties & p) {
							return p.queueFlags & VK_QUEUE_GRAPHICS_BIT;
						});
					}
				}

				return true;
			}

			bool CreateLogicalDevice()
			{
				std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
				std::set<int> uniqueQueueFamilies = { m_graphicFamily, m_presentFamily };


				float queuePriority = 1.0f;
				for (int queueFamily : uniqueQueueFamilies) {
					VkDeviceQueueCreateInfo queueCreateInfo = {};
					queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
					queueCreateInfo.queueFamilyIndex = queueFamily;
					queueCreateInfo.queueCount = 1;
					queueCreateInfo.pQueuePriorities = &queuePriority;
					queueCreateInfos.push_back(queueCreateInfo);
				}

				std::vector<const char *> extentions
				{
					VK_KHR_SWAPCHAIN_EXTENSION_NAME
				};

				VkPhysicalDeviceFeatures deviceFeatures = {};
				VkDeviceCreateInfo createInfo = {};
				createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
				createInfo.pQueueCreateInfos = queueCreateInfos.data();
				createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
				createInfo.pEnabledFeatures = &deviceFeatures;

				createInfo.enabledExtensionCount = static_cast<uint32_t>(extentions.size());
				createInfo.ppEnabledExtensionNames = extentions.data();

				createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
				createInfo.ppEnabledLayerNames = validationLayers.data();

				if (vkCreateDevice(m_phisicalDevice, &createInfo, nullptr, &m_device) != VK_SUCCESS) {
					return false;
				}
				return true;
			}

			bool  CreateSwapChain()
			{
				uint32_t extensionCount;
				vkEnumerateDeviceExtensionProperties(m_phisicalDevice, nullptr, &extensionCount, nullptr);

				std::vector<VkExtensionProperties> availableExtensions(extensionCount);
				vkEnumerateDeviceExtensionProperties(m_phisicalDevice, nullptr, &extensionCount, availableExtensions.data());

				if (std::find_if(availableExtensions.begin(), availableExtensions.end(), [](const VkExtensionProperties & ext) 
				{ return !strcmp(VK_KHR_SWAPCHAIN_EXTENSION_NAME, ext.extensionName); }) == availableExtensions.end())
				{
					return false;
				}

				VkSurfaceCapabilitiesKHR capabilities;
				std::vector<VkSurfaceFormatKHR> formats;
				std::vector<VkPresentModeKHR> presentModes;

				uint32_t formatCount;
				vkGetPhysicalDeviceSurfaceFormatsKHR(m_phisicalDevice , m_surface, &formatCount, nullptr);

				if (formatCount != 0) {
					formats.resize(formatCount);
					vkGetPhysicalDeviceSurfaceFormatsKHR(m_phisicalDevice, m_surface, &formatCount, formats.data());
				}

				uint32_t presentModeCount;
				vkGetPhysicalDeviceSurfacePresentModesKHR(m_phisicalDevice, m_surface, &presentModeCount, nullptr);

				if (presentModeCount != 0) {
					presentModes.resize(presentModeCount);
					vkGetPhysicalDeviceSurfacePresentModesKHR(m_phisicalDevice, m_surface, &presentModeCount, presentModes.data());
				}

				vkGetPhysicalDeviceSurfaceCapabilitiesKHR(m_phisicalDevice, m_surface, &capabilities);	 

				VkSwapchainCreateInfoKHR createInfo = {};



				createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
				createInfo.surface = m_surface;

				createInfo.minImageCount = capabilities.maxImageCount;
				createInfo.imageFormat = formats.at(0).format;
				createInfo.imageColorSpace = formats.at(0).colorSpace;
				createInfo.imageExtent = capabilities.currentExtent;
				createInfo.imageArrayLayers = 1;
				createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;


				createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
				createInfo.queueFamilyIndexCount = 0;
				createInfo.pQueueFamilyIndices = nullptr;

				createInfo.preTransform = capabilities.currentTransform;
				createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;

				createInfo.presentMode = VK_PRESENT_MODE_FIFO_KHR;
				createInfo.clipped = VK_TRUE;


				if (vkCreateSwapchainKHR(m_device, &createInfo, nullptr, &m_swapChain) != VK_SUCCESS) {
					return false;
				}

				uint32_t imageCount;
				vkGetSwapchainImagesKHR(m_device, m_swapChain, &imageCount, nullptr);
				m_swapChainImages.resize(imageCount);
				vkGetSwapchainImagesKHR(m_device, m_swapChain, &imageCount, m_swapChainImages.data());
				m_swapChainImageFormat = formats.at(0).format;
				m_swapChainExtent = capabilities.currentExtent;

				return true;
			}

			bool CreateImageViews()
			{
				m_swapChainImageViews.resize(m_swapChainImages.size());

				for (size_t i = 0; i < m_swapChainImages.size(); i++) {
					VkImageViewCreateInfo createInfo = {};
					createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
					createInfo.image = m_swapChainImages[i];
					createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
					createInfo.format = m_swapChainImageFormat;
					createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
					createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
					createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
					createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
					createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
					createInfo.subresourceRange.baseMipLevel = 0;
					createInfo.subresourceRange.levelCount = 1;
					createInfo.subresourceRange.baseArrayLayer = 0;
					createInfo.subresourceRange.layerCount = 1;

					if (vkCreateImageView(m_device, &createInfo, nullptr, &m_swapChainImageViews[i]) != VK_SUCCESS) {
						return false;
					}
				}
				return true;
			}

			void CreateGraphicsPipeline()
			{
			}

			const VkPhysicalDevice & getPhisicalDevice() const { return m_phisicalDevice; }
			const VkDevice & getDevice() const { return m_device; }
			const VkExtent2D & getSwapChainExtent() const {return m_swapChainExtent;}
			const VkFormat & getSwapChainFormat() const {return m_swapChainImageFormat;}
		private:
			VkInstance m_instance = VK_NULL_HANDLE;
			VkPhysicalDevice m_phisicalDevice = VK_NULL_HANDLE;
			VkDevice m_device = VK_NULL_HANDLE;
			VkSurfaceKHR m_surface = VK_NULL_HANDLE;
			VkSwapchainKHR m_swapChain = VK_NULL_HANDLE;
			std::vector<VkImage> m_swapChainImages;
			VkFormat m_swapChainImageFormat;
			VkExtent2D m_swapChainExtent = {};
			std::vector<VkImageView> m_swapChainImageViews;
			int m_graphicFamily = -1;
			int m_presentFamily = -1;

			VkDebugReportCallbackEXT m_debugCallback;
		};
		*/
		LlrVulkan::LlrVulkan() /*: m_impl(new VulcanImpl) */{}

		LlrVulkan::~LlrVulkan()
		{
			/*if (m_impl)
			{
				delete m_impl;
				m_impl = nullptr;
			}*/
		}

		void LlrVulkan::Init(IWindow * window)
		{
			/*if (!window)
			{
				return;
			}
			m_window = window;

			m_impl->CreateInstance();
			m_impl->SetupDebugCallback();
			m_impl->CreateSurface(m_window);
			m_impl->PickPhysicalDevice();
			m_impl->CreateLogicalDevice();
			m_impl->CreateSwapChain();
			m_impl->CreateImageViews();*/
		}

		IShader * LlrVulkan::CreateShader(const std::string & vertexPath, const std::string & fragmentPass, const std::string & tesselationPass, const std::vector<std::string> define)
		{
			/*auto readFile = [](const std::string path)->std::vector<char>
			{
				std::ifstream file(path, std::ios::ate | std::ios::binary);

				if (!file.is_open()) {
					return std::vector<char>();
				}

				size_t fileSize = (size_t)file.tellg();
				std::vector<char> buffer(fileSize);

				file.seekg(0);
				file.read(buffer.data(), fileSize);

				file.close();

				return buffer;
			};
			
			auto createShader = [this](std::vector<char> & source) ->VkShaderModule
			{
				VkShaderModuleCreateInfo createInfo = {};
				createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
				createInfo.codeSize = source.size();
				createInfo.pCode = reinterpret_cast<const uint32_t*>(source.data());

				VkShaderModule shaderModule;
				if (vkCreateShaderModule(m_impl->getDevice(), &createInfo, nullptr, &shaderModule) != VK_SUCCESS) {
					return VK_NULL_HANDLE;
				}

				return shaderModule;
			};


			VkShaderModule vertexShader = createShader(readFile(vertexPath));
			VkShaderModule fragmentShader = createShader(readFile(fragmentPass));


			if (vertexShader == VK_NULL_HANDLE || vertexShader == VK_NULL_HANDLE)
			{
				vkDestroyShaderModule(m_impl->getDevice(), vertexShader, nullptr);
				vkDestroyShaderModule(m_impl->getDevice(), fragmentShader, nullptr);
			}
			
			
			ShaderVulkan * shader = new ShaderVulkan;
			shader->m_vertexShader = vertexShader;
			shader->m_fragmentShader = fragmentShader;

			return shader;*/

			return nullptr;
		}

		IBuffer * LlrVulkan::CreateBuffer(size_t size)
		{
			return nullptr;
		}

		IConstant * LlrVulkan::CreateConatant(const size_t size)
		{
			return nullptr;
		}

		ITexture2D * LlrVulkan::CreateTexture2d(const size_t width, const size_t heigth, const ETextureFormat format, const EDataType dataType)
		{
			return nullptr;
		}

		ITextureCubeMap * LlrVulkan::CreateTextureCubeMap(const size_t width, const size_t heigth, const ETextureFormat format, const EDataType dataType)
		{
			return nullptr;
		}


		IFramebuffer * LlrVulkan::CreateFramebuffer(const size_t width, const size_t heigth)
		{
			return nullptr;
		}

		IRenderbuffer * LlrVulkan::CreateRenderbuffer(const size_t width, const size_t heigth)
		{
			return nullptr;
		}

		/*EApiRenderPassStatus ApiVulkan::RenderPass(const IShader * shader, IFramebuffer * outputBuffer)
		{
			
			VkDynamicState dynamicStates[] = {
				VK_DYNAMIC_STATE_VIEWPORT,
				VK_DYNAMIC_STATE_LINE_WIDTH
			};

			VkPipelineVertexInputStateCreateInfo vertexInputInfo = {};
			vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
			vertexInputInfo.vertexBindingDescriptionCount = 0;
			vertexInputInfo.pVertexBindingDescriptions = nullptr; // Optional
			vertexInputInfo.vertexAttributeDescriptionCount = 0;
			vertexInputInfo.pVertexAttributeDescriptions = nullptr; // Optional
			
			VkPipelineInputAssemblyStateCreateInfo inputAssembly = {};
			inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
			inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
			inputAssembly.primitiveRestartEnable = VK_FALSE;
			
			const VkExtent2D & extent = m_impl->getSwapChainExtent();
			VkViewport viewport = {};
			viewport.x = 0.0f;
			viewport.y = 0.0f;
			viewport.width = static_cast<float>(extent.width);
			viewport.height = static_cast<float>(extent.height);
			viewport.minDepth = 0.0f;
			viewport.maxDepth = 1.0f;

			VkRect2D scissor = {};
			scissor.offset = { 0, 0 };
			scissor.extent = extent;

			VkPipelineViewportStateCreateInfo viewportState = {};
			viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
			viewportState.viewportCount = 1;
			viewportState.pViewports = &viewport;
			viewportState.scissorCount = 1;
			viewportState.pScissors = &scissor;

			VkPipelineRasterizationStateCreateInfo rasterizer = {};
			rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
			rasterizer.depthClampEnable = VK_FALSE;
			rasterizer.rasterizerDiscardEnable = VK_FALSE;
			rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
			rasterizer.lineWidth = 1.0f;
			rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
			rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
			rasterizer.depthBiasEnable = VK_FALSE;
			rasterizer.depthBiasConstantFactor = 0.0f; // Optional
			rasterizer.depthBiasClamp = 0.0f; // Optional
			rasterizer.depthBiasSlopeFactor = 0.0f; // Optional

			VkPipelineMultisampleStateCreateInfo multisampling = {};
			multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
			multisampling.sampleShadingEnable = VK_FALSE;
			multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
			multisampling.minSampleShading = 1.0f; // Optional
			multisampling.pSampleMask = nullptr; // Optional
			multisampling.alphaToCoverageEnable = VK_FALSE; // Optional
			multisampling.alphaToOneEnable = VK_FALSE; // Optional

			VkPipelineColorBlendAttachmentState colorBlendAttachment = {};
			colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
			colorBlendAttachment.blendEnable = VK_FALSE;
			colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
			colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
			colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD; // Optional
			colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
			colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
			colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD; // Optional

			VkPipelineColorBlendStateCreateInfo colorBlending = {};
			colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
			colorBlending.logicOpEnable = VK_FALSE;
			colorBlending.logicOp = VK_LOGIC_OP_COPY; // Optional
			colorBlending.attachmentCount = 1;
			colorBlending.pAttachments = &colorBlendAttachment;
			colorBlending.blendConstants[0] = 0.0f; // Optional
			colorBlending.blendConstants[1] = 0.0f; // Optional
			colorBlending.blendConstants[2] = 0.0f; // Optional
			colorBlending.blendConstants[3] = 0.0f; // Optional

			VkPipelineDynamicStateCreateInfo dynamicState = {};
			dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
			dynamicState.dynamicStateCount = 2;
			dynamicState.pDynamicStates = dynamicStates;

			VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
			pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
			pipelineLayoutInfo.setLayoutCount = 0; // Optional
			pipelineLayoutInfo.pSetLayouts = nullptr; // Optional
			pipelineLayoutInfo.pushConstantRangeCount = 0; // Optional
			pipelineLayoutInfo.pPushConstantRanges = 0; // Optional

			VkPipelineLayout pipelineLayout;
			if (vkCreatePipelineLayout(m_impl->getDevice(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS)
			{
				return EApiRenderPassStatus::FAIL;
			}


			VkAttachmentDescription colorAttachment = {};
			colorAttachment.format = m_impl->getSwapChainFormat();
			colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;

			colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
			colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;

			colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
			colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;

			colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
			colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

			VkAttachmentReference colorAttachmentRef = {};
			colorAttachmentRef.attachment = 0;
			colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

			VkSubpassDescription subpass = {};
			subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;

			subpass.colorAttachmentCount = 1;
			subpass.pColorAttachments = &colorAttachmentRef;

			VkRenderPass renderPass;
			//VkPipelineLayout pipelineLayout;

			VkRenderPassCreateInfo renderPassInfo = {};
			renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
			renderPassInfo.attachmentCount = 1;
			renderPassInfo.pAttachments = &colorAttachment;
			renderPassInfo.subpassCount = 1;
			renderPassInfo.pSubpasses = &subpass;

			if (vkCreateRenderPass(m_impl->getDevice(), &renderPassInfo, nullptr, &renderPass) != VK_SUCCESS) {
				return EApiRenderPassStatus::FAIL;
			}

			return EApiRenderPassStatus::SUCCESS;
		}*/
	}
}
