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


		enum class EGeObjectType
		{
			NONE = 0
			, MESH = 1
			, CAMERA
			, MATERIAL
			, MATERIAL_INSTANCE
			, TEXTURE_2D
			, SHADER
		};


		class GeObjectHandler
		{
		public:
			GeObjectHandler(EGeObjectType type) : m_objectType(type), m_id(-1) {}

			uint64_t GetId() const { return m_id; };

			void SetId(uint64_t id) { m_id = id; };

			bool IsValid() const { return m_id != -1; }

			GeObjectHandler GetType() { return m_objectType; };

			bool operator<(const GeObjectHandler & rv) const {
				return this->GetId() < rv.GetId();
			}
		
			GeObjectHandler & operator=(const GeObjectHandler & rv)
			{
				this->m_id = rv.m_id;
				this->m_objectType = rv.m_objectType;

				return * this;
			}

			bool operator==(const GeObjectHandler & rv) const
			{
				return this->m_id == rv.m_id;
			}

		protected:
			EGeObjectType m_objectType;
			uint64_t m_id;
		};


		class GeMesh : public GeObjectHandler {
		public:
			GeMesh() : GeObjectHandler(EGeObjectType::MESH) {}
		};

		class GeMaterial : public GeObjectHandler {
		public:
			GeMaterial() : GeObjectHandler(EGeObjectType::MATERIAL) {}
		};

		class GeCamera : public GeObjectHandler {
		public:
			GeCamera() : GeObjectHandler(EGeObjectType::CAMERA) {}
		};

		class GeTexture2d : public GeObjectHandler {
		public:
			GeTexture2d() : GeObjectHandler(EGeObjectType::TEXTURE_2D) {}
		};

		class GeShader : public GeObjectHandler
		{
		public:
			GeShader() : GeObjectHandler(EGeObjectType::SHADER) {}
		};
	}
}