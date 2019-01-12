#pragma once
#include "DemoBase.h"

class DemoMaterial : public DemoBase
{
protected:
	virtual void SetScene() override;

	virtual void Update(double t) override;

private:

	engine::graphic::Mesh * mesh1 = nullptr;
	engine::graphic::Mesh * mesh2 = nullptr;
	engine::graphic::PointLight * light = nullptr;
};
