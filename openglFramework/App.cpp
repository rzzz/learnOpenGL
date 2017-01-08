#include "App.h"

#include "../Framework/Director.h"
#include "../Framework/node/TriangleNode.h"
#include "../Framework/node/ImageNode.h"
#include "../Framework/node/CubeBoxNode.h"
#include "../Framework/node/LightNode.h"
#include "../Framework/node/Model3dNode.h"
#include "../Framework/node/FboNode.h"
#include "../Framework/node/SkyBox.h"
using namespace opengl_framewordk;

App::App()
{
	//
}

App::~App()
{
	//
}

bool App::onApplicationCreate()
{
	auto director = Director::getInstance();
	director->setViewFactor(0.8, 0.8);
	director->initOpenGLWindow(1200, 800, "test");
	
	auto skybox = SkyBox::create();
	skybox->setZOrder(-1000);
	director->addNode(skybox);

	auto triangle = TriangleNode::create();
	triangle->setZOrder(2);
	director->addNode(triangle);

	auto cube = CubeBoxNode::create();
	cube->setZOrder(3);
	director->addNode(cube);

	auto img = ImageNode::create();
	img->setZOrder(7);
	director->addNode(img);

	//auto light = LightNode::create();
	//light->setZOrder(0);
	//director->addNode(light);

	//auto model3d = Model3dNode::create();
	//model3d->setZOrder(3);
	//director->addNode(model3d);

	return true;
}

bool App::onApplicationCreateFinish()
{
	auto director = Director::getInstance();

	//auto triangle = TriangleNode::create();
	//auto cube = CubeBoxNode::create();

	//auto fboNode = FboNode::create();
	//fboNode->beginDraw();
	//triangle->draw();
	//cube->draw();
	//fboNode->endDraw();

	//fboNode->setZOrder(1);
	//director->addNode(fboNode);

	return true;
}