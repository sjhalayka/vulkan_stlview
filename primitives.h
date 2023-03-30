#pragma once


#include "vulkan_functionality.h"


struct Vertex
{
	glm::vec3 pos;
	glm::vec3 normal;

	static VkVertexInputBindingDescription getBindingDescription()
	{
		VkVertexInputBindingDescription bindingDescription{};
		bindingDescription.binding = 0;
		bindingDescription.stride = sizeof(Vertex);
		bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

		return bindingDescription;
	}

	static std::array<VkVertexInputAttributeDescription, 2> getAttributeDescriptions()
	{
		std::array<VkVertexInputAttributeDescription, 2> attributeDescriptions{};

		attributeDescriptions[0].binding = 0;
		attributeDescriptions[0].location = 0;
		attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescriptions[0].offset = offsetof(Vertex, pos);

		attributeDescriptions[1].binding = 0;
		attributeDescriptions[1].location = 1;
		attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescriptions[1].offset = offsetof(Vertex, normal);

		return attributeDescriptions;
	}

	bool operator==(const Vertex& other) const
	{
		return pos == other.pos && normal == other.normal;
	}
};



class comp  //custom comparator
{
public:

	bool operator()(const glm::vec3& left, const glm::vec3& right) const
	{
		if (f(left, right))
			return true;
		else if (f(right, left))
			return false;

		return false;
	}

	bool f(const glm::vec3& left, const glm::vec3& right) const
	{
		if (right.x > left.x)
			return true;
		else if (right.x < left.x)
			return false;

		if (right.y > left.y)
			return true;
		else if (right.y < left.y)
			return false;

		if (right.z > left.z)
			return true;
		else if (right.z < left.z)
			return false;

		return false;
	}

	bool operator()(const Vertex& left, const Vertex& right) const
	{
		if (f(left.pos, right.pos))
			return true;
		else if (f(right.pos, left.pos))
			return false;

		return false;
	}
};




class indexed_vertex_3
{
public:

	inline indexed_vertex_3(void) : x(0.0f), y(0.0f), z(0.0f), index(0) { /*default constructor*/ }
	inline indexed_vertex_3(const float src_x, const float src_y, const float src_z, const size_t src_index) : x(src_x), y(src_y), z(src_z), index(src_index) { /* custom constructor */ }
	inline indexed_vertex_3(const float src_x, const float src_y, const float src_z) : x(src_x), y(src_y), z(src_z), index(0) { /* custom constructor */ }

	inline const void normalize(void)
	{
		float len = length();

		if (0.0f != len)
		{
			x /= len;
			y /= len;
			z /= len;
		}
	}

	inline const float self_dot(void)
	{
		return x * x + y * y + z * z;
	}

	inline const float length(void)
	{
		return std::sqrtf(self_dot());
	}

	inline const indexed_vertex_3& cross(const indexed_vertex_3& right) const
	{
		static indexed_vertex_3 temp;

		temp.x = y * right.z - z * right.y;
		temp.y = z * right.x - x * right.z;
		temp.z = x * right.y - y * right.x;

		return temp;
	}

	inline const indexed_vertex_3& operator-(const indexed_vertex_3& right) const
	{
		static indexed_vertex_3 temp;

		temp.x = this->x - right.x;
		temp.y = this->y - right.y;
		temp.z = this->z - right.z;

		return temp;
	}

	inline const indexed_vertex_3& operator+(const indexed_vertex_3& right) const
	{
		static indexed_vertex_3 temp;

		temp.x = this->x + right.x;
		temp.y = this->y + right.y;
		temp.z = this->z + right.z;

		return temp;
	}

	inline bool operator<(const indexed_vertex_3& right) const
	{
		if (right.x > x)
			return true;
		else if (right.x < x)
			return false;

		if (right.y > y)
			return true;
		else if (right.y < y)
			return false;

		if (right.z > z)
			return true;
		else if (right.z < z)
			return false;

		return false;
	}

	inline void zero(void)
	{
		x = y = z = 0;
	}

	inline void rotate_x(const float& radians)
	{
		float t_y = y;

		y = t_y * cos(radians) + z * sin(radians);
		z = t_y * -sin(radians) + z * cos(radians);
	}

	inline void rotate_y(const float& radians)
	{
		float t_x = x;

		x = t_x * cos(radians) + z * -sin(radians);
		z = t_x * sin(radians) + z * cos(radians);
	}

	inline void rotate_z(const float& radians)
	{
		float t_x = x;

		x = t_x * cos(radians) + y * sin(radians);
		y = t_x * -sin(radians) + y * cos(radians);
	}

	float x, y, z, w;
	size_t index;
};

class triangle
{
public:
	indexed_vertex_3 vertex[3];
};


struct UniformBufferObject {
	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 proj;
	glm::vec3 light_pos;
};


class HelloTriangleApplication
{
public:
	void run()
	{
		initWindow();
		initVulkan();
		mainLoop();
		cleanup();
	}

private:

	GLFWwindow* window;

	VkInstance instance;
	VkDebugUtilsMessengerEXT debugMessenger;
	VkSurfaceKHR surface;

	VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
	VkDevice device;

	VkQueue graphicsQueue;
	VkQueue presentQueue;

	VkSwapchainKHR swapChain;
	std::vector<VkImage> swapChainImages;
	VkFormat swapChainImageFormat;
	VkExtent2D swapChainExtent;
	std::vector<VkImageView> swapChainImageViews;
	std::vector<VkFramebuffer> swapChainFramebuffers;

	VkRenderPass renderPass;
	VkDescriptorSetLayout descriptorSetLayout;
	VkPipelineLayout pipelineLayout;
	VkPipeline graphicsPipeline;

	VkCommandPool commandPool;

	VkImage depthImage;
	VkDeviceMemory depthImageMemory;
	VkImageView depthImageView;

	VkImage textureImage;
	VkDeviceMemory textureImageMemory;
	VkImageView textureImageView;
	VkSampler textureSampler;

	std::vector<Vertex> final_vertices;
	std::vector<uint32_t> final_indices;
	VkBuffer vertexBuffer;
	VkDeviceMemory vertexBufferMemory;
	VkBuffer indexBuffer;
	VkDeviceMemory indexBufferMemory;

	std::vector<VkBuffer> uniformBuffers;
	std::vector<VkDeviceMemory> uniformBuffersMemory;
	std::vector<void*> uniformBuffersMapped;

	VkDescriptorPool descriptorPool;
	std::vector<VkDescriptorSet> descriptorSets;

	std::vector<VkCommandBuffer> commandBuffers;

	std::vector<VkSemaphore> imageAvailableSemaphores;
	std::vector<VkSemaphore> renderFinishedSemaphores;
	std::vector<VkFence> inFlightFences;
	uint32_t currentFrame = 0;

	bool framebufferResized = false;

	void initWindow();

	static void framebufferResizeCallback(GLFWwindow* window, int width, int height)
	{
		auto app = reinterpret_cast<HelloTriangleApplication*>(glfwGetWindowUserPointer(window));
		app->framebufferResized = true;
	}

	void initVulkan();

	void mainLoop();

	void cleanupSwapChain();

	void cleanup();

	void recreateSwapChain();

	void createInstance();

	void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
	void setupDebugMessenger();

	void createSurface();
	void pickPhysicalDevice();

	void createLogicalDevice();

	void createSwapChain();

	void createImageViews();

	void createRenderPass();

	void createDescriptorSetLayout();

	void createGraphicsPipeline();

	void createFramebuffers();

	void createCommandPool();

	void createDepthResources();

	VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);

	VkFormat findDepthFormat();

	bool hasStencilComponent(VkFormat format);

	void createTextureImage();
	void createTextureImageView();

	void createTextureSampler();

	VkImageView createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags);

	void createImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);

	void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);

	void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);



	class indexed_vertex_3
	{
	public:

		inline indexed_vertex_3(void) : x(0.0f), y(0.0f), z(0.0f), index(0) { /*default constructor*/ }
		inline indexed_vertex_3(const float src_x, const float src_y, const float src_z, const size_t src_index) : x(src_x), y(src_y), z(src_z), index(src_index) { /* custom constructor */ }
		inline indexed_vertex_3(const float src_x, const float src_y, const float src_z) : x(src_x), y(src_y), z(src_z), index(0) { /* custom constructor */ }

		inline const void normalize(void)
		{
			float len = length();

			if (0.0f != len)
			{
				x /= len;
				y /= len;
				z /= len;
			}
		}

		inline const float self_dot(void)
		{
			return x * x + y * y + z * z;
		}

		inline const float length(void)
		{
			return std::sqrtf(self_dot());
		}

		inline const indexed_vertex_3& cross(const indexed_vertex_3& right) const
		{
			static indexed_vertex_3 temp;

			temp.x = y * right.z - z * right.y;
			temp.y = z * right.x - x * right.z;
			temp.z = x * right.y - y * right.x;

			return temp;
		}

		inline const indexed_vertex_3& operator-(const indexed_vertex_3& right) const
		{
			static indexed_vertex_3 temp;

			temp.x = this->x - right.x;
			temp.y = this->y - right.y;
			temp.z = this->z - right.z;

			return temp;
		}

		inline const indexed_vertex_3& operator+(const indexed_vertex_3& right) const
		{
			static indexed_vertex_3 temp;

			temp.x = this->x + right.x;
			temp.y = this->y + right.y;
			temp.z = this->z + right.z;

			return temp;
		}

		inline bool operator<(const indexed_vertex_3& right) const
		{
			if (right.x > x)
				return true;
			else if (right.x < x)
				return false;

			if (right.y > y)
				return true;
			else if (right.y < y)
				return false;

			if (right.z > z)
				return true;
			else if (right.z < z)
				return false;

			return false;
		}

		inline void zero(void)
		{
			x = y = z = 0;
		}

		inline void rotate_x(const float& radians)
		{
			float t_y = y;

			y = t_y * cos(radians) + z * sin(radians);
			z = t_y * -sin(radians) + z * cos(radians);
		}

		inline void rotate_y(const float& radians)
		{
			float t_x = x;

			x = t_x * cos(radians) + z * -sin(radians);
			z = t_x * sin(radians) + z * cos(radians);
		}

		inline void rotate_z(const float& radians)
		{
			float t_x = x;

			x = t_x * cos(radians) + y * sin(radians);
			y = t_x * -sin(radians) + y * cos(radians);
		}

		float x, y, z, w;
		size_t index;
	};

	class triangle
	{
	public:
		indexed_vertex_3 vertex[3];
	};

	void get_vertex_normals_from_triangles(vector<triangle>& triangles, map<glm::vec3, glm::vec3, comp>& vertex_normals)
	{
		vector<glm::vec3> face_normals;
		vector<glm::vec3> vertices;

		vertex_normals.clear();

		if (0 == triangles.size())
			return;

		cout << "Triangles: " << triangles.size() << endl;

		cout << "Welding vertices" << endl;

		// Insert unique vertices into set.
		set<indexed_vertex_3> vertex_set;

		for (vector<triangle>::const_iterator i = triangles.begin(); i != triangles.end(); i++)
		{
			vertex_set.insert(i->vertex[0]);
			vertex_set.insert(i->vertex[1]);
			vertex_set.insert(i->vertex[2]);
		}

		cout << "Vertices: " << vertex_set.size() << endl;

		cout << "Generating vertex indices" << endl;

		vector<indexed_vertex_3> vv;

		// Add indices to the vertices.
		for (set<indexed_vertex_3>::const_iterator i = vertex_set.begin(); i != vertex_set.end(); i++)
		{
			size_t index = vv.size();
			vv.push_back(*i);
			vv[index].index = index;
		}

		for (size_t i = 0; i < vv.size(); i++)
		{
			glm::vec3 vv_element(vv[i].x, vv[i].y, vv[i].z);
			vertices.push_back(vv_element);
		}

		vertex_set.clear();

		// Re-insert modifies vertices into set.
		for (vector<indexed_vertex_3>::const_iterator i = vv.begin(); i != vv.end(); i++)
			vertex_set.insert(*i);

		cout << "Assigning vertex indices to triangles" << endl;

		// Find the three vertices for each triangle, by index.
		set<indexed_vertex_3>::iterator find_iter;

		for (vector<triangle>::iterator i = triangles.begin(); i != triangles.end(); i++)
		{
			find_iter = vertex_set.find(i->vertex[0]);
			i->vertex[0].index = find_iter->index;

			find_iter = vertex_set.find(i->vertex[1]);
			i->vertex[1].index = find_iter->index;

			find_iter = vertex_set.find(i->vertex[2]);
			i->vertex[2].index = find_iter->index;
		}

		vertex_set.clear();

		cout << "Calculating normals" << endl;
		face_normals.resize(triangles.size());
		//vertex_normals.resize(vertices.size());

		for (size_t i = 0; i < triangles.size(); i++)
		{
			glm::vec3 v0;
			v0.x = triangles[i].vertex[1].x - triangles[i].vertex[0].x;
			v0.y = triangles[i].vertex[1].y - triangles[i].vertex[0].y;
			v0.z = triangles[i].vertex[1].z - triangles[i].vertex[0].z;

			glm::vec3 v1;
			v1.x = triangles[i].vertex[2].x - triangles[i].vertex[0].x;
			v1.y = triangles[i].vertex[2].y - triangles[i].vertex[0].y;
			v1.z = triangles[i].vertex[2].z - triangles[i].vertex[0].z;

			face_normals[i] = cross(v0, v1);
			face_normals[i] = normalize(face_normals[i]);

			glm::vec3 tv0, tv1, tv2;

			tv0.x = triangles[i].vertex[0].x;
			tv0.y = triangles[i].vertex[0].y;
			tv0.z = triangles[i].vertex[0].z;
			tv1.x = triangles[i].vertex[1].x;
			tv1.y = triangles[i].vertex[1].y;
			tv1.z = triangles[i].vertex[1].z;
			tv2.x = triangles[i].vertex[2].x;
			tv2.y = triangles[i].vertex[2].y;
			tv2.z = triangles[i].vertex[2].z;

			vertex_normals[tv0] += face_normals[i];
			vertex_normals[tv1] += face_normals[i];
			vertex_normals[tv2] += face_normals[i];
		}

		for (map<glm::vec3, glm::vec3>::iterator i = vertex_normals.begin(); i != vertex_normals.end(); i++)
			//		for (size_t i = 0; i < vertex_normals.size(); i++)
			i->second = normalize(i->second);
	}


	void loadModel_STL(void)
	{
		vector<triangle> t;


		ifstream in("fractal.stl", ios_base::binary);

		if (in.fail())
			return;

		const size_t header_size = 80;
		vector<char> buffer(header_size, 0);
		unsigned int num_triangles = 0; // Must be 4-byte unsigned int.

		// Read header.
		in.read(reinterpret_cast<char*>(&(buffer[0])), header_size);

		if (header_size != in.gcount())
			return;

		if (tolower(buffer[0]) == 's' &&
			tolower(buffer[1]) == 'o' &&
			tolower(buffer[2]) == 'l' &&
			tolower(buffer[3]) == 'i' &&
			tolower(buffer[4]) == 'd')
		{
			return;
		}


		// Read number of triangles.
		in.read(reinterpret_cast<char*>(&num_triangles), sizeof(unsigned int));

		if (sizeof(unsigned int) != in.gcount())
			return;

		t.resize(num_triangles);

		// Enough bytes for twelve 4-byte floats plus one 2-byte integer, per triangle.
		const size_t data_size = (12 * sizeof(float) + sizeof(short unsigned int)) * num_triangles;
		buffer.resize(data_size, 0);

		in.read(reinterpret_cast<char*>(&buffer[0]), data_size);

		if (data_size != in.gcount())
			return;

		// Use a pointer to assist with the copying.
		// Should probably use std::copy() instead, but memcpy() does the trick, so whatever...
		char* cp = &buffer[0];

		for (vector<triangle>::iterator ci = t.begin(); ci != t.end(); ci++)
		{
			// Skip face normal.
			cp += 3 * sizeof(float);

			// Get vertices.
			memcpy(&ci->vertex[0].x, cp, sizeof(float)); cp += sizeof(float);
			memcpy(&ci->vertex[0].y, cp, sizeof(float)); cp += sizeof(float);
			memcpy(&ci->vertex[0].z, cp, sizeof(float)); cp += sizeof(float);
			memcpy(&ci->vertex[1].x, cp, sizeof(float)); cp += sizeof(float);
			memcpy(&ci->vertex[1].y, cp, sizeof(float)); cp += sizeof(float);
			memcpy(&ci->vertex[1].z, cp, sizeof(float)); cp += sizeof(float);
			memcpy(&ci->vertex[2].x, cp, sizeof(float)); cp += sizeof(float);
			memcpy(&ci->vertex[2].y, cp, sizeof(float)); cp += sizeof(float);
			memcpy(&ci->vertex[2].z, cp, sizeof(float)); cp += sizeof(float);

			// Skip attribute.
			cp += sizeof(short unsigned int);
		}

		in.close();

		map<glm::vec3, glm::vec3, comp> vertex_normals;
		get_vertex_normals_from_triangles(t, vertex_normals);
		std::map<Vertex, uint32_t, comp> uniqueVertices{};

		for (size_t i = 0; i < t.size(); i++)
		{
			for (size_t j = 0; j < 3; j++)
			{
				Vertex vertex{};

				vertex.pos = {
					t[i].vertex[j].x,
					t[i].vertex[j].y,
					t[i].vertex[j].z
				};

				vertex.normal = vertex_normals[vertex.pos];

				if (uniqueVertices.count(vertex) == 0)
				{
					uniqueVertices[vertex] = static_cast<uint32_t>(final_vertices.size());
					final_vertices.push_back(vertex);
				}

				final_indices.push_back(uniqueVertices[vertex]);
			}
		}

		return;
	}

	void createVertexBuffer();
	void createIndexBuffer();
	void createUniformBuffers();
	void createDescriptorPool();
	void createDescriptorSets();
	void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
	VkCommandBuffer beginSingleTimeCommands();
	void endSingleTimeCommands(VkCommandBuffer commandBuffer);
	void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
	uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
	void createCommandBuffers();
	void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);
	void createSyncObjects();
	void updateUniformBuffer(uint32_t currentImage);
	void drawFrame();
	VkShaderModule createShaderModule(const std::vector<char>& code);
	VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
	VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
	VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
	SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);
	bool isDeviceSuitable(VkPhysicalDevice device);
	bool checkDeviceExtensionSupport(VkPhysicalDevice device);
	QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
	std::vector<const char*> getRequiredExtensions();
	bool checkValidationLayerSupport();
	static std::vector<char> readFile(const std::string& filename);
	static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData); 
};