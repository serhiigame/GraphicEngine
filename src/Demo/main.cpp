#include "Demo.h"

void main(void)
{
	//DemoBase  * demo = new DemoPointLight();
	//DemoBase  * demo = new DemoEnvironment();
	DemoBase  * demo = new DemoMaterial();

	demo->Init();
	demo->Run();
	demo->Deinit();

	delete demo;	
}