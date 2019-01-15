#pragma once
#include <array>

#include <stdio.h>
#include <stdarg.h>

	constexpr const size_t k_logBufferSize = 2048U;

		enum ELogType : unsigned int {
			NONE	= 0,
			INFO	= (1u << 0),
			WARNING = (1u << 1),
			ERROR	= (1u << 2)
		};
		
		

		class LogStrategyConsole
		{
		protected:
			void LogMessage(const char * message)
			{
				printf("%s\n", message);
			}
		};


		template<class strategy>
		class Logger : protected strategy
		{

		public:
			static Logger & Get()
			{
				static Logger logger;
				return logger;
			}

			void Enable(ELogType types)
			{
				m_enabled |= types;
			}

			void Disable(ELogType types)
			{
				m_enabled &= ~types;
			}

			void Log(const ELogType logType, const char * format, ...) {

				if (m_enabled & logType)
				{
					
					const char * typeName = GetLogTypeName(logType);
					if (!typeName)
					{
						return;
					}

					size_t logBufferSize = k_logBufferSize;
					snprintf(m_buffer.data(), logBufferSize, "[ %s ]\t", GetLogTypeName(logType));


					const size_t shift = strlen(m_buffer.data());
					logBufferSize -= shift;

					va_list args;
					va_start(args, format);
					vsnprintf(m_buffer.data() + shift, logBufferSize, format, args);

					strategy::LogMessage(m_buffer.data());

					va_end(args);
				}
				
				
			}

		protected:
			Logger() {
			}

			const char * GetLogTypeName(const ELogType logType)
			{
				switch (logType)
				{
				case ELogType::INFO:
					return "INFO";
				default:
					break;
				}

				return nullptr;
			}

			unsigned int m_enabled = ELogType::NONE;
			std::array<char, k_logBufferSize> m_buffer;
		};


		typedef Logger<LogStrategyConsole> LoggerConsole;
