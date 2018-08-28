#include "Demo.h"

void main(void)
{
	//DemoBase  * demo = new PointLightAnim();
	DemoBase  * demo = new DemoEnvironment();

	demo->Init();
	demo->Run();
	demo->Deinit();

	delete demo;	
}