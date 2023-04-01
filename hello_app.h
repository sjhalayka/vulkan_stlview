#pragma once


#include "vulkan_helper.h"

#include "uv_camera.h"




// https://vulkan-tutorial.com/Loading_models






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
	static VkExtent2D swapChainExtent;
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
	void initVulkan();
	void mainLoop();
	void cleanupSwapChain();
	void cleanup();
	void recreateSwapChain();
	void createInstance();
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
	VkImageView createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags);
	void createImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);
	void createVertexBuffer();
	void createIndexBuffer();
	void createUniformBuffers();
	void createDescriptorPool();
	void createDescriptorSets();
	void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
	void createSyncObjects();
	void createCommandBuffers();
	VkShaderModule createShaderModule(const std::vector<char>& code);
	void updateUniformBuffer(uint32_t currentImage);
	void drawFrame();
	void recordCommandBuffer(VkRenderPass& renderPass, VkCommandBuffer commandBuffer, uint32_t imageIndex);






	void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size) {
		VkCommandBuffer commandBuffer = beginSingleTimeCommands();

		VkBufferCopy copyRegion{};
		copyRegion.size = size;
		vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

		endSingleTimeCommands(commandBuffer);
	}


	void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height) {
		VkCommandBuffer commandBuffer = beginSingleTimeCommands();

		VkBufferImageCopy region{};
		region.bufferOffset = 0;
		region.bufferRowLength = 0;
		region.bufferImageHeight = 0;
		region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		region.imageSubresource.mipLevel = 0;
		region.imageSubresource.baseArrayLayer = 0;
		region.imageSubresource.layerCount = 1;
		region.imageOffset = { 0, 0, 0 };
		region.imageExtent = {
			width,
			height,
			1
		};

		vkCmdCopyBufferToImage(commandBuffer, buffer, image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);

		endSingleTimeCommands(commandBuffer);
	}



	void createTextureImage() {
		int texWidth, texHeight, texChannels;
		stbi_uc* pixels = stbi_load("tex.png", &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
		VkDeviceSize imageSize = texWidth * texHeight * 4;

		if (!pixels) {
			throw std::runtime_error("failed to load texture image!");
		}

		VkBuffer stagingBuffer;
		VkDeviceMemory stagingBufferMemory;
		createBuffer(imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

		void* data;
		vkMapMemory(device, stagingBufferMemory, 0, imageSize, 0, &data);
		memcpy(data, pixels, static_cast<size_t>(imageSize));
		vkUnmapMemory(device, stagingBufferMemory);

		stbi_image_free(pixels);

		createImage(texWidth, texHeight, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, textureImage, textureImageMemory);

		transitionImageLayout(textureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
		copyBufferToImage(stagingBuffer, textureImage, static_cast<uint32_t>(texWidth), static_cast<uint32_t>(texHeight));
		transitionImageLayout(textureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

		vkDestroyBuffer(device, stagingBuffer, nullptr);
		vkFreeMemory(device, stagingBufferMemory, nullptr);
	}

	void createTextureImageView() {
		textureImageView = createImageView(textureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT);
	}

	void createTextureSampler() {
		VkPhysicalDeviceProperties properties{};
		vkGetPhysicalDeviceProperties(physicalDevice, &properties);

		VkSamplerCreateInfo samplerInfo{};
		samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
		samplerInfo.magFilter = VK_FILTER_LINEAR;
		samplerInfo.minFilter = VK_FILTER_LINEAR;
		samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		samplerInfo.anisotropyEnable = VK_TRUE;
		samplerInfo.maxAnisotropy = properties.limits.maxSamplerAnisotropy;
		samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
		samplerInfo.unnormalizedCoordinates = VK_FALSE;
		samplerInfo.compareEnable = VK_FALSE;
		samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
		samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;

		if (vkCreateSampler(device, &samplerInfo, nullptr, &textureSampler) != VK_SUCCESS) {
			throw std::runtime_error("failed to create texture sampler!");
		}
	}

	void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout) {
		VkCommandBuffer commandBuffer = beginSingleTimeCommands();

		VkImageMemoryBarrier barrier{};
		barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
		barrier.oldLayout = oldLayout;
		barrier.newLayout = newLayout;
		barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.image = image;
		barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		barrier.subresourceRange.baseMipLevel = 0;
		barrier.subresourceRange.levelCount = 1;
		barrier.subresourceRange.baseArrayLayer = 0;
		barrier.subresourceRange.layerCount = 1;

		VkPipelineStageFlags sourceStage;
		VkPipelineStageFlags destinationStage;

		if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
			barrier.srcAccessMask = 0;
			barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

			sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
			destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
		}
		else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
			barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
			barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

			sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
			destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
		}
		else {
			throw std::invalid_argument("unsupported layout transition!");
		}

		vkCmdPipelineBarrier(
			commandBuffer,
			sourceStage, destinationStage,
			0,
			0, nullptr,
			0, nullptr,
			1, &barrier
		);

		endSingleTimeCommands(commandBuffer);
	}


	VkCommandBuffer beginSingleTimeCommands() {
		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandPool = commandPool;
		allocInfo.commandBufferCount = 1;

		VkCommandBuffer commandBuffer;
		vkAllocateCommandBuffers(device, &allocInfo, &commandBuffer);

		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

		vkBeginCommandBuffer(commandBuffer, &beginInfo);

		return commandBuffer;
	}


	void endSingleTimeCommands(VkCommandBuffer commandBuffer) {
		vkEndCommandBuffer(commandBuffer);

		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &commandBuffer;

		vkQueueSubmit(graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
		vkQueueWaitIdle(graphicsQueue);

		vkFreeCommandBuffers(device, commandPool, 1, &commandBuffer);
	}



	static void framebufferResizeCallback(GLFWwindow* window, int width, int height)
	{
		auto app = reinterpret_cast<HelloTriangleApplication*>(glfwGetWindowUserPointer(window));
		app->framebufferResized = true;
	}

	static uv_camera main_camera;

	static float u_spacer;
	static float v_spacer;
	static float w_spacer;

	static bool lmb_down;
	static bool mmb_down;
	static bool rmb_down;
	static int mouse_x;
	static int mouse_y;

	static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
	{
		int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);

		if (state == GLFW_PRESS)
			lmb_down = true;
		else
			lmb_down = false;

		state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT);

		if (state == GLFW_PRESS)
			rmb_down = true;
		else
			rmb_down = false;

	}

	static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
	{
		int prev_mouse_x = mouse_x;
		int prev_mouse_y = mouse_y;

		mouse_x = static_cast<int>(xpos);
		mouse_y = static_cast<int>(ypos);

		int mouse_delta_x = mouse_x - prev_mouse_x;
		int mouse_delta_y = prev_mouse_y - mouse_y;

		if (lmb_down == true)
		{
			main_camera.u -= static_cast<float>(mouse_delta_y) * u_spacer;
			main_camera.v += static_cast<float>(mouse_delta_x) * v_spacer;
		}
		else if (true == rmb_down && (0 != mouse_delta_y))
		{
			main_camera.w -= static_cast<float>(mouse_delta_y) * w_spacer;

			if (main_camera.w < 2.0f)
				main_camera.w = 2.0f;
			else if (main_camera.w > 20.0f)
				main_camera.w = 20.0f;
		}
	}



	std::vector<char> readFile(const std::string& filename);

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

};
