

// https://github.com/itoral/vkdf/blob/master/demos/shadow/main.cpp
// https://blogs.igalia.com/itoral/2017/07/30/working-with-lights-and-shadows-part-ii-the-shadow-map/


// Won't compile on MSVC++ https://github.com/PacktPublishing/Vulkan-Cookbook/blob/master/Samples/Source%20Files/11%20Lighting/05-Adding_shadows_to_the_scene/main.cpp


// https://github.com/SaschaWillems/Vulkan/blob/master/examples/screenshot/screenshot.cpp



// https://github.com/itoral/vkdf


#pragma comment( lib, "vulkan-1.lib" )
#pragma comment( lib, "glfw3.lib" )


#include "hello_app.h"





int main()
{
	HelloTriangleApplication app;

	try
	{
		app.run();
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
