#include"StdAfx.h"
//#include "Global.h"
//namespace {
//
//    int g_WindowWidth = 0;
//    int g_WindowHeight = 0;
//
//    SDL_Window* g_Window = nullptr;
//}
//
//bool Window::Init(std::string title, uint16_t width, uint16_t height, uint32_t flags)
//{
//    g_Window = SDL_CreateWindow(title.data(), SCREEN_WIDTH/2, SCREEN_HEIGHT/2, width, height, flags);
//
//    if (g_Window == nullptr)
//
//    {
//        printf("[Error:]%s", SDL_GetError());
//        return false;
//    }
//    g_WindowWidth = width;
//    g_WindowHeight = height;
//
//    //resize resoultuion
//   // SDL_SetWindowMinimumSize(g_Window,1280,720);
//    // Window::SetSDLWindow(g_Window);
//    //set up log
//    return true;
//}
//
//void Window::ShutDown()
//{
//    if(!g_Window==NULL)
//    SDL_DestroyWindow(g_Window);
//    //set up log
//}
//
//
//SDL_Window* Window::GetWindow()
//{
//   
//   
//    //Returns current windows
//    return g_Window;
//}
//
//uint16_t Window::GetWidth()
//{
//    //returns windows width
//    return g_WindowWidth;
//}
//
//uint16_t Window::GetHeight()
//{
//    //returns windows Height
//    return g_WindowHeight;
//}
//
//
//
//
//
//
