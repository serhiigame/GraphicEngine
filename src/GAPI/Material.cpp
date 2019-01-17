#include "Material.h"
#include "Shader.h"

bool engine::graphic::operator<(const MaterialInstance & lhs, const MaterialInstance & rhs)
{
	return lhs.GetId() < rhs.GetId();
}
