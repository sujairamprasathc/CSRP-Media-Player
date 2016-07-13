/*
*	My own header for creating SDL2 windows
*	Base taken from lazyfoo.org
*/

#ifndef _CSRP_SDL2_WINDOWS_H_
#define _CSRP_SDL2_WINDOWS_H_	1

//Define type of callback functions
enum callback_t {
GRAPHICS_RENDERER=0, EVENT_HANDLER
};

class SDL2Window
{
	public:
		//Intializes internals
		SDL2Window();

		//Creates window
		virtual bool init(int, int, std::string, int, int);

		//Init OpenGL
		bool initGL();

		//Return window pointer
		SDL_Window *returnWindowPointer();

		//Return renderer
		SDL_Renderer* returnRenderer();

		//Handles window events
		void handleEvent( SDL_Event& e );

		//Focuses on window
		void focus();

		//Registers a callback to call when window needs update
		void registerCallback(void (*callback)(void), callback_t);

		//Registers a callback to call when window needs update
		void registerCallback(void (*callback)(SDL_Event &e), callback_t);

		//Shows windows contents
		void render();

		//Deallocates internals
		void free();

		//Window dimensions
		int getWidth();
		int getHeight();

		//Window focii
		bool hasMouseFocus();
		bool hasKeyboardFocus();
		bool isMinimized();
		bool isShown();

	protected:
		//Window data
		SDL_Window* mWindow;
		SDL_Renderer* mRenderer;
		SDL_GLContext mContext;
		int windowID;
		std::string windowTitle;

		//Callbacks
		void (*graphicsCallbackFunc)(void);
		void (*eventCallbackFunc)(SDL_Event &e);

		//Window dimensions
		int windowWidth;
		int windowHeight;

		//Window focus
		bool windowMouseFocus;
		bool windowKeyboardFocus;
		bool windowFullScreen;
		bool windowMinimized;
		bool windowShown;
};

/*class AlertBox : public SDL2Window
{
bool init()
{
SDL2Window::init(320,240,"CSRP Media Player---Alert",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED);

}
};*/

#include "SDL2_windows.cpp"

#endif
