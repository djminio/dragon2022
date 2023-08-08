#pragma once

//class Renderer
//{
//public:
//    Renderer();
//    ~Renderer();
//  
//    static  void ShutDown();
//
//    static SDL_Renderer* GetRenderer();
//
//    static  SDL_Texture* GetSDLTexture();
//
//    static  char* GetSDLPixels();
//
//
//    static void SDL_EraseScreen();
//    static  void SDL_EndFont();
//    static  char* SDL_GetSurfacePointer();
//    static  void SDL_FlipScreen();
//    static void SDL_Hprint(const int x, const int y, char* destbuf, const char* szStr, SDL_Color textColor, int HangulAlign);
//    static void SDL_HprintBold(const int x, const int y, char* destbuf, const char* szStr, SDL_Color textColor, int HangulAlign);
//    static  void SDL_Draw();
//    static void SetSDLRenderer(SDL_Renderer* renderer) { renderer = g_renderer; };
//    static SDL_Texture* putFontIntoCache(char* szStr, bool bold, SDL_Color textColor);
//    static SDL_Rect AlignText(int HangulAlign, int x, int y, int texW, int texH);
//
//private:
//   static  char* g_pixels;
//   static SDL_Renderer* g_renderer;
//    static SDL_Texture* g_texture;
//};

/*

*/
#ifndef	__SDLRENDER_H__
#define	__SDLRENDER_H__


#ifdef _SDL
#include <SDL_pixels.h>
#include <SDL.h>
#include <SDL_ttf.h>
#endif


#ifdef __cplusplus
extern "C"
{
#endif
#ifdef _SDL
	//window handle

	extern BOOL	InitSDL();
	extern void SDL_EraseScreen();
	extern char* SDL_GetSurfacePointer();
	extern void SDL_FlipScreen();
	extern void SDL_Hprint(const int x, const int y, char* destbuf, const char* szStr, SDL_Color textColor, int HangulAlign);
	extern void SDL_HprintBold(const int x, const int y, char* destbuf, const char* szStr, SDL_Color textColor, int HangulAlign);
	extern void SDL_Draw();
	extern void SDLKill();
	extern void SDL_Lock();
	extern void SDL_UnLock();
	extern SDL_Window* GetSDLWindow();
	//extern HWND GetHMain();

	extern void SetHMain(HWND Window);

#endif // _SDL

	

#ifdef __cplusplus
}
#endif

#endif //__SDL2RENDER_H__