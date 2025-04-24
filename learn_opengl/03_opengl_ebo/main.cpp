/*
2025-04-24

*/
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>



/*

OpenGL中 渲染管线每个阶段

1. 顶点着色器 (vertex shader)
2. 几何着色器 (goemetry shader)
3. 形状（图元）装配（shape assembley）
4. 参数与混合(tests and blending)
5. 片段着色器（fragment shader）
6. 光栅化(rasterization)

demo实现


1. 渲染管线和VBO（顶点数据）
2. 着色器编译链接和使用
3. 连接顶点属性和VAO （记录顶点数据在OpengGL中一种状态）
4. 索引缓冲对象EBO












*/



static const uint32_t SCR_WIDTH = 800;
static const uint32_t SCR_HEIGHT = 600;



//// 顶点数据 三角形坐标
//GLfloat vertices[] = {
//	-0.5f, -0.5f, 0.0f,
//	 0.5f, -0.5f, 0.0f,
//	 0.0f,  0.5f, 0.0f
//};


// 顶点数据 四角形
GLfloat vertices[] = {
	0.5f, 0.5f, 0.0f,   // 右上角
	0.5f, -0.5f, 0.0f,  // 右下角
	-0.5f, -0.5f, 0.0f, // 左下角
	-0.5f, 0.5f, 0.0f   // 左上角
};

GLuint indices[] = {
	// 注意索引从0开始! 
	// 此例的索引(0,1,2,3)就是顶点数组vertices的下标，
	// 这样可以由下标代表顶点组合成矩形

	0, 1, 3, // 第一个三角形
	1, 2, 3  // 第二个三角形
};

// 顶点着色器的shader
static const GLchar *vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

// 判断着色器 shader
static const GLchar * fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\0";


/*
窗口改变回调函数
*/
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	  glViewport(0, 0, width, height);
}

/*
输入的设备的处理
*/
void process_input(GLFWwindow* window)
{
	// 检查键盘 scape键是否按下
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}

int main(int argc, char *argv[])
{
	glfwInit();
	//设置 主要版本 
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	// 设置次要版本
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//  下载OpenGL是 核心模式   [Core]
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

	// 给GLAD传入了用来加载系统相关的OpenGL函数指针地址的函数
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	// 设置视口  -》 函数前两个参数控制窗口左下角的位置。第三个和第四个参数控制渲染窗口的宽度和高度（像素）。
	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

	// 注册窗口变化时回调函数 
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);




	// 申请顶点缓冲区对象
	unsigned int VBO;
	glGenBuffers(1, &VBO);

	// 顶点缓存对象绑定到顶点缓存GL_ARRAY_BUFFER类型中去
//	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// 顶点数据复制到GPU缓冲的内存
//	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);




	// 创建VAO  是保存顶点数据在OpenGL中一个状态 
	GLuint VAO;
	glGenVertexArrays(1, &VAO);

	// 创建EBO 元素缓冲对象
	GLuint EBO;
	glGenBuffers(1, &EBO);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


	//////////////////////////////////////////////顶点着色器创建、附加和编译//////////////////////////////////////////////////////////
	// 创建顶点着色器对象
	GLuint vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER/*着色器的类型*/);

	// 把着色器的shader附加到顶点着色器对象上去然后编译
	glShaderSource(vertexShader, 1/*。第二参数指定了传递的源码字符串数量*/, &vertexShaderSource, NULL);
	//编译着色器 
	glCompileShader(vertexShader);

	// 检查顶点着色器是否编译成功
	GLint success = 0;
	GLchar infoLog[512] = {0};
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

	//判断顶点着色器是否有错误 错误打印
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR: Shader vertex Compilation failed !!! => " << infoLog << std::endl;
	}
	//////////////////////////////////////////////片段着色器创建、附加和编译//////////////////////////////////////////////////////////

	GLuint fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	// 检查片段着色器是否编译成功
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

	//判断顶点着色器是否有错误 错误打印
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR: Shader Fragment  Compilation failed !!! => " << infoLog << std::endl;
	}

	///////////////////////////////////////////////着色器程序/////////////////////////////////////////////////////////
	GLuint shaderProgram;
	//创建着色器程序对象
	shaderProgram = glCreateProgram();
	// 我们需要把之前编译的着色器附加到程序对象上，然后用glLinkProgram链接它们
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

	// 链接着色器程序后删除顶点着色器和片段着色器
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);



	// 1. 绑定顶点数组对象
	glBindVertexArray(VAO);
	// 2. 把我们的顶点数组复制到一个顶点缓冲中，供OpenGL使用
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// 3. 复制我们的索引数组到一个索引缓冲中，供OpenGL使用
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	// 4. 设定顶点属性指针
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);


	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	// // 解绑 顶点VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	// 解绑 VAO
	glBindVertexArray(0);
	//线框模式(Wireframe Mode)
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	// 

	while (!glfwWindowShouldClose(window))
	{
		// 输入设备的处理
		process_input(window);


		//////////////////////////////////////////////////////////////////////////////////////////////////
		// 渲染指令 处理
		// 设置清空名片所用的拥塞
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		// 
		glClear(GL_COLOR_BUFFER_BIT);
		/////////////////////////////////////////////////////////////////////////////////////////////////
		glUseProgram(shaderProgram);
		// 使用VAO记录VBO在OpenGL中一个状态
		glBindVertexArray(VAO);
		//glDrawArrays(GL_TRIANGLES, 0, 3);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		// 交换颜色缓冲（它是一个储存着GLFW窗口每一个像素颜色值的大缓冲），它在这一迭代中被用来绘制，并且将会作为输出显示在屏幕上
		glfwSwapBuffers(window);
		// 检查有没有触发什么事件（比如键盘输入、鼠标移动等）、更新窗口状态，并调用对应的回调函数（可以通过回调方法手动设置）
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shaderProgram);
	// 释放render 资源
	glfwTerminate();
	return EXIT_SUCCESS;
}