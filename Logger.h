////------------------------------------------------------------------------
//#pragma once
//
////------------------------------------------------------------------------
//// spdlog
//#include <spdlog/sinks/stdout_color_sinks.h>
//#include <spdlog/sinks/basic_file_sink.h>
//
//#include <memory>
//
//namespace Logger
//{
//    void Init();
//
//    std::shared_ptr<spdlog::logger> GetLogger();
//
//}
//
//// Client log macros
//#define DR_TRACE(...)    ::Logger::GetLogger()->trace(__VA_ARGS__)
//#define DR_INFO(...)     ::Logger::GetLogger()->info(__VA_ARGS__)
//#define DR_WARN(...)     ::Logger::GetLogger()->warn(__VA_ARGS__)
//#define DR_ERROR(...)    ::Logger::GetLogger()->error(__VA_ARGS__)
//#define DR_CRITICAL(...) ::Logger::GetLogger()->critical(__VA_ARGS__)