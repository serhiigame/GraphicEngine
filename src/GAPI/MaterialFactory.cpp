#include "MaterialFactory.h"

namespace engine
{
	namespace graphic
	{
		Material MaterialFactory::CreateMaterial(EMaterialType materialType)
		{
			Material material;

			switch (materialType)
			{
			case EMaterialType::PHONG:
			{
				MaterialProperty diffuseProperty;
				diffuseProperty.Type = MaterialPropertyType::TEXTURE;

				//diffuseProperty = default texture

				material.m_properties["diffuse"] = diffuseProperty;
			}
			default:
				printf("unknown material type \n");
			}


			

			return material;
		}
	}
}