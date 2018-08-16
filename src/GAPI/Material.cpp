#include "Material.h"

namespace engine
{
	namespace graphic
	{
		const std::map<std::string, MaterialProperty>& Material::GetProperties() const
		{
			return m_properties;
		}

		void Material::SetProperty(const std::string & name, const Texture & texture)
		{
			auto finded = m_properties.find(name);
#ifdef DEBUG
			if (finded == m_properties.end())
			{
				printf("Material parameter not found");
			}

			if (finded->second.Type != MaterialPropertyType::TEXTURE)
			{
				printf("Material parameter %s is not texture", finded->first.c_str());
			}
#endif

			finded->second.Texture = texture;
		}
		void Material::GetProperty(const std::string & name, Texture & out_texture) const
		{
			auto finded = m_properties.find(name);

#ifdef DEBUG
			if (finded == m_properties.end())
			{
				printf("Material parameter not found");
			}

			if (finded->second.Type != MaterialPropertyType::TEXTURE)
			{
				printf("Material parameter %s is not texture", finded->first.c_str());
			}
#endif
			out_texture = finded->second.Texture;
		}
	}
}