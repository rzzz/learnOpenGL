#ifndef __Director_H__
#define __Director_H__

// GLEW
//GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

#include <string>
#include <vector>

#include "node\OpenglNode.h"
#include "node\RectangleNode.h"
#include "node\FboNode.h"

namespace opengl_framewordk
{
#define lightPos glm::vec3(1.2f, 1.0f, 2.0f)
#define lightColor glm::vec3(1.0f, 1.0f, 1.0f)

	class CameraView;

	//导演类，初始化OpenGL窗口，响应系统事件，提供节点绘制工作，管理摄像机
	class Director
	{
	private:
		Director();
		~Director();

		static Director* instance;

	public:
		static Director* getInstance();
		static void destoryInstance();

	public:
		void setViewFactor(float vw, float vh);	//call this before initOpenGLWindow
		bool initOpenGLWindow();
		bool initOpenGLWindow(int width, int height, const char * title);

		void drawScene();
		bool drawScene(float dt);
		void realDrawScene();

		void addNode(OpenglNode* node);

		bool isInitReady(){ return isInit; }

		int getViewPortSizeWidth(){ return winWidth * viewWidthFactor; }
		int getViewPortSizeHeight(){ return winHeight * viewHeightFactor; }

		CameraView* getCamera(){ return camera; }

	private:
		void sortNodes();
		void drawNodes();

		void initViewPortRect();

	public:
		float thisTimeDt;

	private:
		CameraView* camera;
		int winWidth;
		int winHeight;
		std::string winTitle;

		float viewWidthFactor;		//viewport; todo : postion
		float viewHeightFactor;

		GLFWwindow* window;

		std::vector<OpenglNode*> nodes;		//渲染时排序

		bool isInit;
		RectangleNode* pViewRectNode;	//用于初始化一次viewport矩形框

		FboNode* fbo;

	};
}
#endif