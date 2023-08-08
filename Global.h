#pragma once
//#include "windef.h"

#ifdef  _SDL
	TTF_Font* gFont;
	SDL_Texture* BlankTexture;
	//structure for front and color
	struct CACHEDFONT {
		int id;
		SDL_Texture* texture;
	}pCACHEDFONT;

	const int MAXFONTS = 512;

	CACHEDFONT CachedFonts[MAXFONTS];
	int CachedFontsCount;
	///////////////////////////////
	HWND				m_hwndMain;
	//Window Hnadlers
	SDL_Window* m_SDLhwndMain;
	SDL_Renderer* m_SDLRenderer;
	SDL_Texture* m_SDLTexture;
	SDL_Window* GetSDLWindow();
	/// <summary>
	/// /////////////////////////
	/// </summary>
	char* m_SDLPixels;
	//Func to return the handlers

	
	SDL_Window* GetSDLWindow() { return m_SDLhwndMain; }
	void SetSDLWindow(SDL_Window* SDLWindow) { m_SDLhwndMain = SDLWindow; }

	SDL_Renderer* GetSDLRenderer() { return m_SDLRenderer; }
	void SetSDLRenderer(SDL_Renderer* SDLRenderer) { m_SDLRenderer = SDLRenderer; }

	SDL_Texture* GetSDLTexture() { return m_SDLTexture; }
	static void SetSDLTexture(SDL_Texture* SDLTexture) { m_SDLTexture = SDLTexture; }

	char* GetSDLPixels();
	static void SetSDLPixels(char* pixels);

	char* GetSDLPixels() { return m_SDLPixels; }
	static void SetSDLPixels(char* pixels) { m_SDLPixels = pixels; };

	//extern void		SetHMain(const HWND Window);

#endif