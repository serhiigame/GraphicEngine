#pragma once

#include "IResource.h"

#include "Texture2d.h"

#include <map>

namespace engine
{
	namespace graphic
	{
		enum class MaterialPropertyType
		{
			NONE = -1
			, TEXTURE = 1
			//, INT
			//, FLOAT
			//, COLOR
		};

		struct MaterialProperty
		{
			MaterialPropertyType Type;
			Texture2d Texture;
			//int Int;
			//float Float;
		};
	

		class Material : public IResource
		{
			friend class MaterialFactory;
		public:
			const std::map<std::string, MaterialProperty> & GetProperties() const;

			void SetProperty(const std::string & name, const Texture2d & texture);

			void GetProperty(const std::string & name, Texture2d & out_texture) const;

		protected:
			std::map<std::string, MaterialProperty> m_properties;
		};
	}
}