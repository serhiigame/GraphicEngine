#include "Scene.h"


namespace engine
{
	namespace graphic
	{
		void Scene::AddMesh(Mesh * mesh)
		{
			m_meshes.push_back(mesh);
		}

		void Scene::AddPointLight(PointLight * pointLight)
		{
			m_pointLights.push_back(pointLight);
		}

		void Scene::SetSkybox(TextureCubeMap * cubemap)
		{
			m_skybox = cubemap;
		}

		void Scene::SetCamera(Camera * camera)
		{
			m_camera = camera;
		}

		Camera * Scene::GetCamera()
		{
			return m_camera;
		}

		const Camera * Scene::GetCamera() const
		{
			return static_cast<const Scene *>(this)->GetCamera();
		}
	}
}