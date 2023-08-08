#include "StdAfx.h"
//#include "Logger.h"
//#include "spdlog/spdlog.h"
//
////------------------------------------------------------------------------
//namespace
//{
//    std::shared_ptr<spdlog::logger> g_Logger;
//}
//
////------------------------------------------------------------------------
//void Logger::Init()
//{
//    std::vector<spdlog::sink_ptr> logSinks;
//    logSinks.emplace_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
//    logSinks.emplace_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>("ImageTool.log", true));
//
//    logSinks[0]->set_pattern("%^[%T] %n: %v%$");
//    logSinks[1]->set_pattern("[%T] [%l] %n: %v");
//
//    g_Logger = std::make_shared<spdlog::logger>("ImageTool", begin(logSinks), end(logSinks));
//    //g_Logger->set_level(spdlog::level::trace);
//    //g_Logger->flush_on(spdlog::level::trace);
//
//    DR_INFO("Logger initialized.");
//}
//
////------------------------------------------------------------------------
//std::shared_ptr<spdlog::logger> Logger::GetLogger()
//{
//    Logger::Init();
//
//    DR_INFO("It works");
//    DR_TRACE("Trace");
//    DR_WARN("WARNING");
//    DR_ERROR("ERROR");
//    DR_CRITICAL("CRITIAL");
//    return g_Logger;
//}
