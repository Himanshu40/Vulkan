//#include <vulkan/vulkan.h>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <iostream>
#include <vector>
#include <set>
#include <stdexcept>
#include <cstdlib>
#include <cstring>

const uint32_t WIDTH = 800;
const uint32_t HEIGHT = 600;

const std::vector<const char*> validationLayers = {
	"VK_LAYER_KHRONOS_validation"
};

#ifdef NDEBUG
	const bool enableValidationLayers = false;
#else
	const bool enableValidationLayers = true;
#endif

class HelloTriangleApplication {
public:
	void run() {
		initWindow();
		initVulkan();
		mainLoop();
		cleanup();
	}
private:
	GLFWwindow* window;
	VkInstance instance;

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

	/// <summary>
	/// checks if all of the extensions returned by glfwGetRequiredInstanceExtensions are included in the supported vulkan extensions list
	/// </summary>
	/// <param name="glfwExtensions"></param>
	/// <param name="glfwExtensionCount"></param>
	/// <param name="vkExtensions"></param>
	void checkSupport(const char** glfwExtensions, const uint32_t &glfwExtensionCount, const std::set<std::string> &vkExtensions) {
		//std::cout << glfwExtensions << std::endl; // prints memory address of the first element in the glfwExtensions array.

		// prints each extension name that belongs to GLFW
		/*for (uint32_t i = 0; i < glfwExtensionCount; ++i) {
			std::cout << glfwExtensions[i] << std::endl;
		}*/

		for (uint32_t i = 0; i < glfwExtensionCount; ++i) {
			if (vkExtensions.find(glfwExtensions[i]) != vkExtensions.end()) {
				std::cout << glfwExtensions[i] << std::endl;
			}
		}
	}

	void createInstance() {
		if (enableValidationLayers && !checkValidationLayerSupport()) {
			throw std::runtime_error("valdiation layers requested, but not available");
		}

		uint32_t extensionCount = 0;
		vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
		//std::cout << extensionCount << std::endl;

		std::vector<VkExtensionProperties> extensions(extensionCount);
		vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());
	
		//std::cout << "Available extensions:\n";

		std::set<std::string> vkExtensions;
		for (const auto& extension : extensions) {
			vkExtensions.insert(extension.extensionName);
			//std::cout << '\t' << extension.extensionName << '\n';
		}

		VkApplicationInfo appInfo{};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName = "Hello Triangle";
		appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.pEngineName = "No Engine";
		appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.apiVersion = VK_API_VERSION_1_0;

		VkInstanceCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.pApplicationInfo = &appInfo;

		uint32_t glfwExtensionCount = 0;
		const char** glfwExtensions;

		glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
		checkSupport(glfwExtensions, glfwExtensionCount, vkExtensions);
		
		createInfo.enabledExtensionCount = glfwExtensionCount;
		createInfo.ppEnabledExtensionNames = glfwExtensions;

		if (enableValidationLayers) {
			createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
			createInfo.ppEnabledLayerNames = validationLayers.data();
		}
		else {
			createInfo.enabledLayerCount = 0;
		}

		VkResult result = vkCreateInstance(&createInfo, nullptr, &instance);
	
		if (result != VK_SUCCESS) {
			throw std::runtime_error("failed to create instance!");
		}
		else {
			std::cout << "SUCCESS";
		}
	}

	void initWindow() {
		glfwInit();

		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

		window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);
	}

	void initVulkan() {
		createInstance();
	}

	void mainLoop() {
		while (!glfwWindowShouldClose(window)) {
			glfwPollEvents();
		}
	}

	void cleanup() {
		vkDestroyInstance(instance, nullptr);

		glfwDestroyWindow(window);

		glfwTerminate();
	}
};

int main() {
	HelloTriangleApplication app;

	try {
		app.run();
	}
	catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}