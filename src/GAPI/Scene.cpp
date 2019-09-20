#include "Scene.h"


namespace engine
{
	namespace graphic
	{
		void Scene::AddMesh(GeMesh mesh)
		{
			m_meshes.insert(mesh);
		}

		void Scene::AddPointLight(PointLight * pointLight)
		{
			m_pointLights.push_back(pointLight);
		}

		void Scene::SetSkybox(GeTextureCubeMap cubemap)
		{
			m_skybox = cubemap;
		}

		void Scene::SetCamera(GeCamera camera)
		{
			m_camera = camera;
		}

		const GeCamera Scene::GetCamera() const
		{
			return m_camera;
		}
	}
}