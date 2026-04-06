#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "shaderClass.h"
#include "VBO.h"
#include "VAO.h"
#include "EBO.h"

GLfloat vertices[] = {
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.5f, 0.5f, 0.0f,
		-0.5f, 0.5f, 0.0f,
};

GLuint indices[]{
	0, 1, 2,
	2, 3, 0,
};

int main() {
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 800, "Lab 2: Moving segment", NULL, NULL);
	glfwMakeContextCurrent(window);

	gladLoadGL();

	glViewport(0, 0, 800, 800);

	Shader shaderProgram("segment.vert", "segment.frag");

	VAO segmentVAO;
	segmentVAO.Bind();

	VBO segmentVBO(vertices, sizeof(vertices));
	EBO segmentEBO(indices, sizeof(indices));

	segmentVAO.LinkVBO(segmentVBO, 0);

	segmentVAO.Unbind();
	segmentVBO.Unbind();
	segmentEBO.Unbind();

	while (!glfwWindowShouldClose(window)) {
		glClearColor(42.0f / 255, 42.0f / 255, 53.0f / 255, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		shaderProgram.Activate();
		segmentVAO.Bind();

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();

	segmentVAO.Delete();
	segmentVBO.Delete();
	segmentEBO.Delete();
	shaderProgram.Delete();

	return 0;
}