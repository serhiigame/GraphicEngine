#pragma once

#include "api.h"

#include "IResource.h"

#include <vector>

namespace engine
{
	namespace graphic
	{
		class Mesh;
		class Camera;
		class PointLight;
		class TextureCubeMap;

		class Scene : public IResource
		{
		public:
			void SetCamera(Camera * camera);

			const Camera * GetCamera() const;
			Camera * GetCamera();

			void AddMesh(Mesh * mesh);

			const std::vector< Mesh *> & GetMeshes() { return m_meshes; }

			void AddPointLight(PointLight * mesh);

			void SetSkybox(TextureCubeMap * cubemap);

			TextureCubeMap * GetSkybox() const { return m_skybox; }

			const std::vector< PointLight *> & GetPointLight() { return m_pointLights; }

		protected:
			std::vector< Mesh *> m_meshes;
			std::vector< PointLight *> m_pointLights;
			TextureCubeMap * m_skybox = nullptr;
			Camera * m_camera = nullptr;
		};
	}
}