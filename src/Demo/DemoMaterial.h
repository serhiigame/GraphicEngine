#pragma once
#include "DemoBase.h"
#include "DemoUtil.h"

class DemoMaterial : public DemoBase
{
protected:
	virtual void SetScene() override;

	virtual void Update(double t) override;

private:

	void RegisterMaterials();

	engine::graphic::Mesh * mesh = nullptr;
	engine::graphic::PointLight * light = nullptr;
};
