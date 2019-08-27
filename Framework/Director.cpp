#include "Director.h"
#include <iostream>
#include <algorithm>
#include "Shader\ShaderProgram.h"
#include "FreeImage\TextureManager.h"
#include "CameraView.h"

using namespace std;

namespace opengl_framework
{
	static bool keys[1024];
	GLfloat lastX = 400, lastY = 400;
	bool firstMouse = true;
	GLfloat lastFrame = 0.0f;

	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
	{
		std::cout << key << std::endl;
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		{
			glfwSetWindowShouldClose(window, GL_TRUE);
		}

		if (key >= 0 && key < 1024)
		{
			if (action == GLFW_PRESS)
				keys[key] = true;
			else if (action == GLFW_RELEASE)
				keys[key] = false;
		}
	}

	static void mouse_callback(GLFWwindow* window, double xpos, double ypos)
	{
		if (firstMouse)
		{
			lastX = xpos;
			lastY = ypos;
			firstMouse = false;
		}

		GLfloat xoffset = xpos - lastX;
		GLfloat yoffset = lastY - ypos;  // Reversed since y-coordinates go from bottom to left

		lastX = xpos;
		lastY = ypos;

		if (Director::getInstance()->getCamera())
		{
			Director::getInstance()->getCamera()->ProcessMouseMovement(xoffset, yoffset);
		}
	}

	static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
	{
		if (Director::getInstance()->getCamera())
		{
			Director::getInstance()->getCamera()->ProcessMouseScroll(yoffset);
		}
	}

	// Moves/alters the camera positions based on user input
	static void Do_Movement()
	{
		CameraView* camera = Director::getInstance()->getCamera();		
		if (camera)
		{
			float deltaTime = Director::getInstance()->thisTimeDt;
			// Camera controls
			if (keys[GLFW_KEY_W])
				camera->ProcessKeyboard(CameraView::FORWARD, deltaTime);
			if (keys[GLFW_KEY_S])
				camera->ProcessKeyboard(CameraView::BACKWARD, deltaTime);
			if (keys[GLFW_KEY_A])
				camera->ProcessKeyboard(CameraView::LEFT, deltaTime);
			if (keys[GLFW_KEY_D])
				camera->ProcessKeyboard(CameraView::RIGHT, deltaTime);
		}
	}

	Director* Director::instance = nullptr;
	Director::Director()
		:winWidth(800), winHeight(800), winTitle(), viewWidthFactor(1), viewHeightFactor(1), window(nullptr)
		, nodes(), isInit(false), pViewRectNode(nullptr), camera(nullptr), thisTimeDt(0.01), fbo(nullptr)
	{
		//
	}

	Director::~Director()
	{
		delete TextureManager::Inst();

		TextureCache::destoryInstance();

		if (pViewRectNode)
		{
			delete pViewRectNode;
		}

		if (camera)
		{
			delete camera;
		}

		for (vector<OpenglNode*>::iterator itr = nodes.begin(); itr != nodes.end(); itr++)
		{
			delete *itr;
		}
		nodes.clear();

		if (fbo)
		{
			delete fbo;
		}

		ShaderProgramCache::destroyInstacne();
	}

	Director* Director::getInstance()
	{
		if (instance == nullptr)
		{
			instance = new Director();
		}

		return instance;
	}

	void Director::destoryInstance()
	{
		if (instance)
		{
			delete instance;
			instance = nullptr;
		}
	}

	void Director::setViewFactor(float vw, float vh)
	{
		viewWidthFactor = vw > 1.0f ? 1.0f : (vw <= 0.1f ? 0.1f : vw);
		viewHeightFactor = vh > 1.0f ? 1.0f : (vh <= 0.1f ? 0.1f : vh);
	}

	bool Director::initOpenGLWindow()
	{
		return initOpenGLWindow(800, 800, "opengl");
	}

	bool Director::initOpenGLWindow(int width, int height, const char * title)
	{
		winWidth = width;
		winHeight = height;
		winTitle = title;

		//init opengl
		cout << "Starting GLFW context, OpenGL 3.3" << std::endl;
		// Init GLFW
		glfwInit();
		// Set all the required options for GLFW
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);	//we use VAO,so GLFW_OPENGL_CORE_PROFILE
		glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

		// Create a GLFWwindow object that we can use for GLFW's functions
		window = glfwCreateWindow(winWidth + 2, winHeight + 2, winTitle.c_str(), nullptr, nullptr);		//winTitle的cstr //string的c_str()返回的指针是由string管理的，因此它的生命期是string对象的生命期
		if (window == nullptr)
		{
			std::cout << "Failed to create GLFW window" << std::endl;
			glfwTerminate();
			return false;
		}
		glfwMakeContextCurrent(window);

		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		// Set the required callback functions
		glfwSetKeyCallback(window, key_callback);
		glfwSetCursorPosCallback(window, mouse_callback);
		glfwSetScrollCallback(window, scroll_callback);

		// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
		glewExperimental = GL_TRUE;	//we use VAO,so set this
		// Initialize GLEW to setup the OpenGL Function pointers
		if (glewInit() != GLEW_OK)
		{
			std::cout << "Failed to initialize GLEW" << std::endl;
			return false;
		}

		// Define the viewport dimensions
		//int fm_width, fm_height;
		//glfwGetFramebufferSize(window, &fm_width, &fm_height);
		glViewport(1, 1, viewWidthFactor * winWidth, viewHeightFactor * winHeight);

		glEnable(GL_DEPTH_TEST);

		camera = new CameraView(glm::vec3(0.0f, 0.0f, 3.0f));

		initViewPortRect();

		isInit = true;

		return true;
	}

	void Director::initViewPortRect()
	{
		if (pViewRectNode == nullptr)
		{
			pViewRectNode = RectangleNode::create();
		}
	}

	void Director::addNode(OpenglNode* node)
	{
		nodes.push_back(node);
	}

	void Director::sortNodes()
	{
		sort(nodes.begin(), nodes.end(), [](OpenglNode* v1, OpenglNode* v2)->bool{ return v1->getZOrder() < v2->getZOrder(); });
	}

	void Director::drawNodes()
	{
		for (vector<OpenglNode*>::iterator itr = nodes.begin(); itr != nodes.end(); itr++)
		{
			(*itr)->draw();
		}
	}

	void Director::drawScene()
	{
		while (!glfwWindowShouldClose(window))
		{
			// Set frame time
			GLfloat currentFrame = glfwGetTime();		//运行的秒数
			thisTimeDt = currentFrame - lastFrame;
			lastFrame = currentFrame;

			realDrawScene();
		}

		glfwTerminate();
	}

	bool Director::drawScene(float dt)
	{
		thisTimeDt = dt;

		if (window == nullptr)
		{
			return false;
		}

		// Game loop
		if (glfwWindowShouldClose(window))
		{
			// Terminate GLFW, clearing any resources allocated by GLFW.
			glfwTerminate();
			return false;
		}

		realDrawScene();

		return true;
	}

	void Director::realDrawScene()
	{
		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();
		Do_Movement();

		if (fbo == nullptr)
		{
			fbo = FboNode::create();
		}
		fbo->beginDraw();
		sortNodes();
		drawNodes();
		fbo->endDraw();

		// Render
		// Clear the colorbuffer
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		pViewRectNode->draw();

		sortNodes();
		drawNodes();

		fbo->draw();

		// Swap the screen buffers
		glfwSwapBuffers(window);
	}
}