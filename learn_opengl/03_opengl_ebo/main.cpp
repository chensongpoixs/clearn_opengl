/*
2025-04-24

*/
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>



/*

OpenGL�� ��Ⱦ����ÿ���׶�

1. ������ɫ�� (vertex shader)
2. ������ɫ�� (goemetry shader)
3. ��״��ͼԪ��װ�䣨shape assembley��
4. ��������(tests and blending)
5. Ƭ����ɫ����fragment shader��
6. ��դ��(rasterization)

demoʵ��


1. ��Ⱦ���ߺ�VBO���������ݣ�
2. ��ɫ���������Ӻ�ʹ��
3. ���Ӷ������Ժ�VAO ����¼����������OpengGL��һ��״̬��
4. �����������EBO












*/



static const uint32_t SCR_WIDTH = 800;
static const uint32_t SCR_HEIGHT = 600;



//// �������� ����������
//GLfloat vertices[] = {
//	-0.5f, -0.5f, 0.0f,
//	 0.5f, -0.5f, 0.0f,
//	 0.0f,  0.5f, 0.0f
//};


// �������� �Ľ���
GLfloat vertices[] = {
	0.5f, 0.5f, 0.0f,   // ���Ͻ�
	0.5f, -0.5f, 0.0f,  // ���½�
	-0.5f, -0.5f, 0.0f, // ���½�
	-0.5f, 0.5f, 0.0f   // ���Ͻ�
};

GLuint indices[] = {
	// ע��������0��ʼ! 
	// ����������(0,1,2,3)���Ƕ�������vertices���±꣬
	// �����������±��������ϳɾ���

	0, 1, 3, // ��һ��������
	1, 2, 3  // �ڶ���������
};

// ������ɫ����shader
static const GLchar *vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

// �ж���ɫ�� shader
static const GLchar * fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\0";


/*
���ڸı�ص�����
*/
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	  glViewport(0, 0, width, height);
}

/*
������豸�Ĵ���
*/
void process_input(GLFWwindow* window)
{
	// ������ scape���Ƿ���
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}

int main(int argc, char *argv[])
{
	glfwInit();
	//���� ��Ҫ�汾 
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	// ���ô�Ҫ�汾
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//  ����OpenGL�� ����ģʽ   [Core]
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// glfw window creation
	// --------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// ��GLAD��������������ϵͳ��ص�OpenGL����ָ���ַ�ĺ���
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	// �����ӿ�  -�� ����ǰ�����������ƴ������½ǵ�λ�á��������͵��ĸ�����������Ⱦ���ڵĿ�Ⱥ͸߶ȣ����أ���
	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

	// ע�ᴰ�ڱ仯ʱ�ص����� 
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);




	// ���붥�㻺��������
	unsigned int VBO;
	glGenBuffers(1, &VBO);

	// ���㻺�����󶨵����㻺��GL_ARRAY_BUFFER������ȥ
//	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// �������ݸ��Ƶ�GPU������ڴ�
//	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);




	// ����VAO  �Ǳ��涥��������OpenGL��һ��״̬ 
	GLuint VAO;
	glGenVertexArrays(1, &VAO);

	// ����EBO Ԫ�ػ������
	GLuint EBO;
	glGenBuffers(1, &EBO);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


	//////////////////////////////////////////////������ɫ�����������Ӻͱ���//////////////////////////////////////////////////////////
	// ����������ɫ������
	GLuint vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER/*��ɫ��������*/);

	// ����ɫ����shader���ӵ�������ɫ��������ȥȻ�����
	glShaderSource(vertexShader, 1/*���ڶ�����ָ���˴��ݵ�Դ���ַ�������*/, &vertexShaderSource, NULL);
	//������ɫ�� 
	glCompileShader(vertexShader);

	// ��鶥����ɫ���Ƿ����ɹ�
	GLint success = 0;
	GLchar infoLog[512] = {0};
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

	//�ж϶�����ɫ���Ƿ��д��� �����ӡ
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR: Shader vertex Compilation failed !!! => " << infoLog << std::endl;
	}
	//////////////////////////////////////////////Ƭ����ɫ�����������Ӻͱ���//////////////////////////////////////////////////////////

	GLuint fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	// ���Ƭ����ɫ���Ƿ����ɹ�
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

	//�ж϶�����ɫ���Ƿ��д��� �����ӡ
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR: Shader Fragment  Compilation failed !!! => " << infoLog << std::endl;
	}

	///////////////////////////////////////////////��ɫ������/////////////////////////////////////////////////////////
	GLuint shaderProgram;
	//������ɫ���������
	shaderProgram = glCreateProgram();
	// ������Ҫ��֮ǰ�������ɫ�����ӵ���������ϣ�Ȼ����glLinkProgram��������
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) 
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED  LINKING failed !!! => " << infoLog << std::endl;
	}
	//glUseProgram(shaderProgram);

	// ������ɫ�������ɾ��������ɫ����Ƭ����ɫ��
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);



	// 1. �󶨶����������
	glBindVertexArray(VAO);
	// 2. �����ǵĶ������鸴�Ƶ�һ�����㻺���У���OpenGLʹ��
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// 3. �������ǵ��������鵽һ�����������У���OpenGLʹ��
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	// 4. �趨��������ָ��
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);


	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	// // ��� ����VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	// ��� VAO
	glBindVertexArray(0);
	//�߿�ģʽ(Wireframe Mode)
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	// 

	while (!glfwWindowShouldClose(window))
	{
		// �����豸�Ĵ���
		process_input(window);


		//////////////////////////////////////////////////////////////////////////////////////////////////
		// ��Ⱦָ�� ����
		// ���������Ƭ���õ�ӵ��
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		// 
		glClear(GL_COLOR_BUFFER_BIT);
		/////////////////////////////////////////////////////////////////////////////////////////////////
		glUseProgram(shaderProgram);
		// ʹ��VAO��¼VBO��OpenGL��һ��״̬
		glBindVertexArray(VAO);
		//glDrawArrays(GL_TRIANGLES, 0, 3);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		// ������ɫ���壨����һ��������GLFW����ÿһ��������ɫֵ�Ĵ󻺳壩��������һ�����б��������ƣ����ҽ�����Ϊ�����ʾ����Ļ��
		glfwSwapBuffers(window);
		// �����û�д���ʲô�¼�������������롢����ƶ��ȣ������´���״̬�������ö�Ӧ�Ļص�����������ͨ���ص������ֶ����ã�
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shaderProgram);
	// �ͷ�render ��Դ
	glfwTerminate();
	return EXIT_SUCCESS;
}