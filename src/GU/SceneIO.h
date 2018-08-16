#pragma once

#include <string>
#include <map>

#include "../GAPI/RawMeshData.h"

namespace engine
{
	namespace utility
	{
		//class ImageIOBase;

		class SceneIo final
		{
		public:
			__declspec(dllexport)
			static SceneIo & Get();
			
			__declspec(dllexport)
			const float * LoadScene(const std::string & filePath);


		private:
			SceneIo();
			~SceneIo();
			SceneIo(SceneIo &) = default;
			SceneIo & operator=(SceneIo &) = default;

			//std::map<std::string, ImageIOBase *>  m_imgIoImpl;
		};
	}
}