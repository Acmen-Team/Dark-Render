#include <iostream>
#include <windows.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <DRender.h>

/*
* This is a simple example of using the DRender library to create a triangle
*/

using namespace DRender;

// Define the vertices for a triangle
float vertices[] = {
		 0.0f,  0.5f, 0.0f, // top
		-0.5f, -0.5f, 0.0f, // left
		 0.5f, -0.5f, 0.0f  // right
};

const char* vertexShaderSource = 
"#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"    gl_Position = vec4(aPos, 1.0);\n"
"}\n";

// Fragment shader source code
const char* fragmentShaderSource = 
"#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"    FragColor = vec4(1.0, 0.5, 0.2, 1.0);\n"
"}\n";

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

int main()
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(640, 480, "Dark-Render Example", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	if (status == 0)
	{
		std::cerr << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	//gladLoadGL();

	ResourceRegistry* resourceRegistry = new ResourceRegistry();

	resourceRegistry->Register(
		"ColorBuffer",
		[]() -> std::any {
			GLuint colorBuffer;
			glGenTextures(1, &colorBuffer);
			glBindTexture(GL_TEXTURE_2D, colorBuffer);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 640, 480, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
			glBindTexture(GL_TEXTURE_2D, 0);
			return colorBuffer;
		},
		[&resourceRegistry]() {
			GLuint colorBuffer = resourceRegistry->Get<GLuint>("ColorBuffer");
			glDeleteTextures(1, &colorBuffer);
		}
	);

	resourceRegistry->Register(
		"VertexArray",
		[]() -> std::any {
			GLuint VAO;
			glGenVertexArrays(1, &VAO);
			glBindVertexArray(VAO);
			return VAO;
		},
		[&resourceRegistry]() {
			GLuint VAO = resourceRegistry->Get<GLuint>("VertexArray");
			glBindVertexArray(0);
			glDeleteVertexArrays(1, &VAO);
		}
	);

	resourceRegistry->Register(
		"VertexBuffer",
		[]() -> std::any {
			GLuint VBO;
			glGenBuffers(1, &VBO);
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(0);
			return VBO;
		},
		[&resourceRegistry]() {
			GLuint VBO = resourceRegistry->Get<GLuint>("VertexBuffer");
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glDeleteBuffers(1, &VBO);
		}
	);

	resourceRegistry->Register(
		"VertexShader",
		[]() -> std::any {
			GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
			glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
			glCompileShader(vertexShader);
			return vertexShader;
		},
		[&resourceRegistry]() {
			GLuint vertexShader = resourceRegistry->Get<GLuint>("VertexShader");
			glDeleteShader(vertexShader);
		}
	);

	resourceRegistry->Register(
		"FragmentShader",
		[]() -> std::any {
			GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
			glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
			glCompileShader(fragmentShader);
			return fragmentShader;
		},
		[&resourceRegistry]() {
			GLuint fragmentShader = resourceRegistry->Get<GLuint>("FragmentShader");
			glDeleteShader(fragmentShader);
		}
	);

	resourceRegistry->Register(
		"ShaderProgram",
		[&resourceRegistry]() -> std::any {
			GLuint shaderProgram = glCreateProgram();
			glAttachShader(shaderProgram, resourceRegistry->Get<GLuint>("VertexShader"));
			glAttachShader(shaderProgram, resourceRegistry->Get<GLuint>("FragmentShader"));
			glLinkProgram(shaderProgram);
			return shaderProgram;
		},
		[&resourceRegistry]() {
			GLuint shaderProgram = resourceRegistry->Get<GLuint>("ShaderProgram");
			glDeleteProgram(shaderProgram);
		}
	);

	RenderGraph renderGraph;
	renderGraph.SetResourceRegistry(resourceRegistry);

	RenderPass* clearPass = new RenderPass("ClearPass");
	clearPass->SetExexute([&]() {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
	});

	RenderPass* drawPass = new RenderPass("DrawPass");
	drawPass->SetExexute([&]() {
		glBindTexture(GL_TEXTURE_2D, resourceRegistry->Get<GLuint>("ColorBuffer"));
		glUseProgram(resourceRegistry->Get<GLuint>("ShaderProgram"));
		glBindVertexArray(resourceRegistry->Get<GLuint>("VertexArray"));
		glDrawArrays(GL_TRIANGLES, 0, 3);
	});

	renderGraph.AddPass(clearPass);
	renderGraph.AddPass(drawPass);

	renderGraph.BuildGraph();

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		/* Use the DRender library to draw something */
		renderGraph.Execute();

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}