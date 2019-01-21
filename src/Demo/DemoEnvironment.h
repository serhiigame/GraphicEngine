#pragma once
#include "DemoBase.h"

class DemoEnvironment : public DemoBase
{
protected:
	virtual void SetScene() override;

	virtual void Update(double t) override;

private:
	engine::graphic::GeMesh mesh;
	engine::graphic::PointLight * light = nullptr;
};
