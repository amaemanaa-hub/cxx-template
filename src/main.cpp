#include <iostream>
#include <vector>
#include "rust_lib/src/lib.rs.h"
#include <glfw3.h>
#include <vulkan/vulkan.h>

int main() {
    int32_t value = get_int();
    std::cout << "get_int(" << value << ");" << std::endl;
    glfwInit();

    VkInstance instance;


    GLFWwindow* window = glfwCreateWindow(800, 600, "hi", nullptr, nullptr);
    while (true)
    {

    }

    return 0;
}