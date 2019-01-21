#include "Material.h"
#include "Shader.h"
#include "ShaderInputs.h"

namespace engine
{
	namespace graphic
	{
		bool operator<(const MaterialInstance & lhs, const MaterialInstance & rhs)
		{
			return lhs.GetId() < rhs.GetId();
		}

		ShaderInputDesc MaterialHelper::MakeShaderTextureInputDesc(const std::string & Name, const int Binding, const GeTexture2d & fallback)
		{
			ShaderInputDesc texInputDesc;
			texInputDesc.Binding = Binding;
			texInputDesc.Type = EMaterialInputType::TEXTURE;
			texInputDesc.Name = Name;

			// TODO: do not use new
			ShaderInputTexture2d * inputTexture = new ShaderInputTexture2d;
			inputTexture->SetTexture(fallback);

			texInputDesc.Fallback = inputTexture;

			return texInputDesc;
		}

		IShaderInput * MaterialObject::GetMaterialInputFallback(const std::string & name)
		{
			
			for (auto input : m_shaderDesc.Inputs)
			{
				if (input.Name == name)
				{
					return input.Fallback;
				}
			}

			return nullptr;
		}
	}
}