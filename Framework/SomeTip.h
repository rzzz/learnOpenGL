/*

3D模型文件：
模型为一个物体坐标系内的物体集合，每个物体包含自己的若干Mesh，每个Mesh有若干顶点（坐标，纹理坐标）组成，考虑到顶点较多，所以同时会包含index数据，另外还可能还有贴图纹理和法线纹理，
渲染物体时一般使用glDrawElements来进行index渲染。


深度测试：
深度测试是在像素着色器运行后（也在模板测试之后，我们会在下节讨论），在屏幕空间中完成的。
屏幕空间坐标与OpenGL的glViewport函数定义的视口（viewport）直接相关，可以在像素着色器中通过GLSL的内建变量gl_FragCoord来获取。
gl_FragCoord的x和y元素代表fragment的屏幕空间坐标(0, 0是左下角)。gl_FragCoord也包含一个z元素，这个元素是fragment的实际深度值。z值就是用来和深度缓冲内容对比的那个值。
模型坐标到世界坐标，完成基本的坐标系转换，此时只有在世界坐标系中一个三坐标（-1，1）的立方体中的物体才是可见的，
此时顶点还有一个w（=1）坐标，每个顶点（xyz）除以w，完成像素除法得到的值是屏幕空间的深度，深度从0到1规则化，可见立方体内，z为1的点深度最大为1，z为-1的点深度最低为0，
于是得到了，默认less比较深度的情况，离“我”越近的物体反而越靠后；
从世界坐标到真正我们希望的坐标，还有一个相机坐标和投影矩阵，投影矩阵会把摄像机空间的顶点转换到一个新的坐标系内，它刚好是三坐标（-1，1）的立方体内，
这个过程为了保证w还为1，xyz被重新计算，再通过透视除法后，发现近平面到远平面，深度不断在增大（近远平面垂直摄像机的-z轴上），这样就符合“我”的人眼观察。
APIs:
glEnable(GL_DEPTH_TEST);	//开启深度测试
glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		//清空屏幕空间每个像素点的深度缓冲
glDepthMask(GL_FALSE);		//false是关闭深度写入功能，也就是说，在使用只读的深度缓冲（默认下在像素着色器的透视除法会得到深度，同时会更新每个像素的深度）
glDepthFunc(GL_LESS);		//less比较，深度越小，则通过测试，得以被显示出来
z-fighting是深度缓冲中常见的问题，通常当物体越远的时候越严重（因为深度缓冲在更远的地方z值的精度越低）。

模板测试：
当像素着色器处理完fragment以后，模板测试（stencil test）就开始执行了，和深度测试一样，它有能力丢弃fragment，仍然保留下来的fragment进入深度测试阶段，深度测试可能丢弃更多的fragment，所以它先于深度测试。
模板缓冲中一个模板值（stencil value）通常含有8位大小，因此每个像素（fragment）总共有256个不同的模板值；而深度测试的值则是浮点型（0，1）之间。
与深度测试不同，模板测试，是需要先通过调用渲染命令来更新模板缓冲，在有了模板缓冲之后，再次渲染物体，这时候利用已经存储的模板缓冲来选择每个像素是否保留，可应用在物体描边上
APIs：
glEnable(GL_STENCIL_TEST);	//开启模板测试
glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);		//清空缓冲
glStencilMask(0xFF);		//和glDepthMask一样，它（这里是0xFF）与模板值进行与（and）运算决定缓冲是否可写，最终得到一个8位的模板值，一般用0x00或0xFF
glStencilFunc(GLenum func, GLint ref, GLuint mask)	//模板方程：首先在模板测试对比ref和已储存的模板值前，对它们进行与（and）操作；func决定已储存的模板值与ref值如何比较（比较成功则通过模板测试，像素得以保留下来）
//如：glStencilFunc(GL_EQUAL, 1, 0xFF)，它代表glStencilMask(0xFF)后，如果当前像素的模板值和1相同，则此像素点保留
那么深度值是透视除法得到的结果，模板值是如何计算的呢？
glStencilOp(GLenum sfail, GLenum dpfail, GLenum dppass)	//sfail如果模板测试失败将采取的动作;dpfail如果模板测试通过，但是深度测试失败时采取的动作;dppass如果深度测试和模板测试都通过，将采取的动作

混合：
像素着色器运行和所有的测试就通过了以后，混合方程才能自由执行fragment的颜色输出，当前它在颜色缓冲中（前面的fragment的颜色在当前fragment之前储存）。
源和目标颜色会自动被OpenGL设置，但是源和目标因子可以让我们自由设置。完全透明的alpha为0，完全不透明的alpha为1。

*/