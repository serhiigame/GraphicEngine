#pragma once

#include "EGApiEnums.h"

#include <map>

namespace engine
{
	namespace graphic
	{
		class IResource;
		class Mesh;
		class MaterialObject;
		class Camera;
		class Texture2d;
		class Shader;

		class ResourceManager final
		{
			friend class GApiImpl;
		public:
			void Init();

			void Deinit();

			GeMesh CreateMesh();

			Mesh * GetMesh(const GeMesh & geMesh);

			GeMaterial CreateMaterial();

			MaterialObject * GetMaterial(const GeMaterial & geMesh);

			GeCamera CreateCamera();

			Camera * GetCamera(const GeCamera & geCamera);

			GeTexture2d CreateTexture2d();

			Texture2d * GetTexture2d(const GeTexture2d & geTexture2d);

			GeShader CreateShader();

			Shader * GetShader(const GeShader & geShader);

		private:

			size_t GenerateId() {
				return m_idCounter++;
			}

			std::map<GeMesh, Mesh * > m_meshes;

			std::map<GeMaterial, MaterialObject *> m_materialStorages;

			std::map<GeCamera, Camera * > m_cameras;

			std::map<GeTexture2d, Texture2d * > m_textures2d;

			std::map<GeShader, Shader * > m_shaders;

			size_t m_idCounter = 0;
		};
	}
}