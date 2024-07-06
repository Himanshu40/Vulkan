#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <iostream>
#include <vector>
#include <set>
#include <string>
#include <optional>

#ifdef NDEBUG
	const bool enableValidationLayers = false;
#else
	const bool enableValidationLayers = true;
#endif

const uint32_t WIDTH = 800;
const uint32_t HEIGHT = 600;

const std::vector<const char*> validationLayers = {
	"VK_LAYER_KHRONOS_validation"
};

struct QueueFamilyIndices {
	std::optional<uint32_t> graphicsFamily;

	bool isComplete() {
		return graphicsFamily.has_value();
	}
};

class Engine {
private:
	GLFWwindow* window;
	VkInstance instance;
	VkDevice device;
	VkQueue graphicsQueue;
	VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
	VkDebugUtilsMessengerEXT debugMessenger;

	// Vulkan helper functions
	bool checkValidationLayerSupport();
	std::vector<const char*> getRequiredExtensions();
	void checkGLFWVulkanExtensionCompatibility(
		const char** glfwExtensions, 
		const uint32_t& glfwExtensionCount, 
		const std::set<std::string>& vkExtensions
	);
	bool isDeviceSuitable(VkPhysicalDevice device);
	QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);

	// Vulkan Initialization functions
	void initVulkan();
	void createInstance();
	void setupDebugMessenger();
	void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
	void pickPhysicalDevice();
	void createLogicalDevice();

	// GLFW Initialization functions
	void initWindow(); // Window initialization
	void mainLoop();

	// Destroy Vulkan and GLFW resources
	void cleanup();
public:
	void run();
};
