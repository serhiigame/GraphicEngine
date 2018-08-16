#pragma once

#include <string>
#include <map>
namespace engine
{
	namespace utility
	{
		class ImageIOBase;

		class ImageIo final
		{
		public:
			__declspec(dllexport)
			static ImageIo & Get();
			
			__declspec(dllexport)
			const float * Load(const std::string & filePath, int & o_width, int & o_height);

			__declspec(dllexport)
			void Write(const std::string & filename, const int width, const int heigh, const float * data);


		private:
			ImageIo();
			~ImageIo();
			ImageIo(ImageIo &) = default;
			ImageIo & operator=(ImageIo &) = default;

			std::map<std::string, ImageIOBase *>  m_imgIoImpl;
		};
	}
}