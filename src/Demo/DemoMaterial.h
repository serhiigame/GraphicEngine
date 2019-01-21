#pragma once
#include "DemoBase.h"

class DemoMaterial : public DemoBase
{
protected:
	virtual void SetScene() override;

	virtual void Update(double t) override;

private:

	engine::graphic::GeMesh mesh1;
	engine::graphic::GeMesh mesh2;
	engine::graphic::PointLight * light = nullptr;
};
