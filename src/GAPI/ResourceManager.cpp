#include "ResourceManager.h"

#include "Mesh.h"
#include "Camera.h"

#include "Texture2d.h"

namespace engine
{
	namespace graphic
	{
		template <class Handler, class Obj>
		inline Handler createGeObject(std::map<Handler, Obj * > & mho, uint64_t id)
		{
			Handler handler;
			handler.SetId(id);
			mho.emplace(handler, new Obj);
			return handler;
		}


		void ResourceManager::Init()
		{
		
		}

		void ResourceManager::Deinit()
		{
		
		}

		GeMesh ResourceManager::CreateMesh() {
			return createGeObject(m_meshes, GenerateId());
		}

		Mesh * ResourceManager::GetMesh(const GeMesh & geMesh)
		{
			return m_meshes.at(geMesh);
		}
		GeCamera ResourceManager::CreateCamera()
		{
			return createGeObject(m_cameras, GenerateId());
		}
		Camera * ResourceManager::GetCamera(const GeCamera & geMesh)
		{
			return m_cameras.at(geMesh);
		}
		GeTexture2d ResourceManager::CreateTexture2d()
		{
			return createGeObject(m_textures2d, GenerateId());
		}
		Texture2d * ResourceManager::GetTexture2d(const GeTexture2d & geTexture2d)
		{
			return m_textures2d.at(geTexture2d);
		}
	}
}