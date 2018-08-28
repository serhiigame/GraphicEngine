#pragma once
#include "DemoBase.h"

#include "DemoUtil.h"

class DemoPointLight : public DemoBase
{
protected:
	virtual void SetScene() override;

	virtual void Update(double t) override;

private:
	engine::graphic::Mesh * mesh = nullptr;
	engine::graphic::PointLight * light = nullptr;
};
