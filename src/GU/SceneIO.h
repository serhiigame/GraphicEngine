#pragma once

#include <string>
#include <map>

#include "../GAPI/RawMeshData.h"

namespace engine
{
	namespace utility
	{
		class SceneIo final
		{
		public:
			__declspec(dllexport)
			static SceneIo & Get();
			
			__declspec(dllexport)
				bool LoadMesh(const std::string & filename, std::vector<graphic::RawMeshData> & out_meshDatas);


		private:
			SceneIo();
			~SceneIo();
			SceneIo(SceneIo &) = default;
			SceneIo & operator=(SceneIo &) = default;

			//std::map<std::string, ImageIOBase *>  m_imgIoImpl;
		};
	}
}