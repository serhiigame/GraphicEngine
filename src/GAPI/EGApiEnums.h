#pragma once

#include <stdint.h>

namespace engine
{
	namespace graphic
	{
		enum class ERenderMode{
			DRAW_ALL
		};

		enum class ERenderStatus
		{
			SUCCESS
		};


		enum class EGeSceneObjectType
		{
			NONE = 0
			, MESH = 1
			, CAMERA
			, MATERIAL
			, TEXTURE_2D
		};


		class SceneObjectHandler
		{
		public:
			SceneObjectHandler(EGeSceneObjectType type) : m_objectType(type), m_id(-1) {}

			uint64_t GetId() const { return m_id; };

			void SetId(uint64_t id) { m_id = id; };

			bool IsValid() const { return m_id != -1; }

			EGeSceneObjectType GetType() { return m_objectType; };

			bool operator<(const SceneObjectHandler & rv) const {
				return this->GetId() < rv.GetId();
			}
		
			SceneObjectHandler & operator=(const SceneObjectHandler & rv)
			{
				this->m_id = rv.m_id;
				this->m_objectType = rv.m_objectType;

				return * this;
			}

		protected:
			EGeSceneObjectType m_objectType;
			uint64_t m_id;
		};


		class GeMesh : public SceneObjectHandler {
		public:
			GeMesh() : SceneObjectHandler(EGeSceneObjectType::MESH) {}
		};

		class GeCamera : public SceneObjectHandler {
		public:
			GeCamera() : SceneObjectHandler(EGeSceneObjectType::CAMERA) {}
		};

		class GeTexture2d : public SceneObjectHandler {
		public:
			GeTexture2d() : SceneObjectHandler(EGeSceneObjectType::TEXTURE_2D) {}
		};
	}
}