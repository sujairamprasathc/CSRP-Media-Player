/*
*	My own header for creating SDL2 windows
*	Base taken from lazyfoo.org
*/

#ifdef _CSRP_SDL2_WINDOWS_H_

SDL2Window::SDL2Window()
{
	//Initialize non-existant window
	mWindow = NULL;
	mRenderer = NULL;

	windowTitle="TITLE";

	windowMouseFocus = false;
	windowKeyboardFocus = false;
	windowFullScreen = false;
	windowShown = false;
	windowID = -1;
	
	windowWidth = 0;
	windowHeight = 0;
}

bool SDL2Window::init(int SCREEN_WIDTH, int SCREEN_HEIGHT, std::string WINDOW_TITLE, int x, int y)
{
	//Use OpenGL 2.1
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 2 );
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 1 );

	//Create window
	mWindow = SDL_CreateWindow( WINDOW_TITLE.c_str(), x, y, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE );
	if( mWindow != NULL )
	{
		windowMouseFocus = true;
		windowKeyboardFocus = true;
		windowWidth = SCREEN_WIDTH;
		windowHeight = SCREEN_HEIGHT;
		windowTitle = WINDOW_TITLE;

		//Create renderer for window
		mRenderer = SDL_CreateRenderer( mWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
		if( mRenderer == NULL )
		{
			printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
			SDL_DestroyWindow( mWindow );
			mWindow = NULL;
		}
		else
		{
			//Initialize renderer color
			SDL_SetRenderDrawColor( mRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

			//Grab window identifier
			windowID = SDL_GetWindowID( mWindow );

			mContext = SDL_GL_CreateContext( mWindow );
			if( mContext == NULL )
			{
				printf( "OpenGL context could not be created! SDL Error: %s\n", SDL_GetError() );
				return false;
			}
			if( SDL_GL_SetSwapInterval( 1 ) < 0 )
			{
				printf( "Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError() );
			}

			//Initialize OpenGL
			if( !initGL() )
			{
				printf( "Unable to initialize OpenGL!\n" );
			}

			//Flag as opened
			windowShown = true;
		}
	}
	else
	{
		printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
	}

	return mWindow != NULL && mRenderer != NULL;
}

bool SDL2Window::initGL()
{
	bool success = true;
	GLenum error = GL_NO_ERROR;

	//Initialize Projection Matrix
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	
	//Check for error
	error = glGetError();
	if( error != GL_NO_ERROR )
	{
		printf( "Error initializing OpenGL! %s\n", gluErrorString( error ) );
		success = false;
	}

	//Initialize Modelview Matrix
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();

	//Check for error
	error = glGetError();
	if( error != GL_NO_ERROR )
	{
		printf( "Error initializing OpenGL! %s\n", gluErrorString( error ) );
		success = false;
	}
	
	//Initialize clear color
	glClearColor( 1.0, 1.0, 1.0, 1.0 );
	
	//Check for error
	error = glGetError();
	if( error != GL_NO_ERROR )
	{
		printf( "Error initializing OpenGL! %s\n", gluErrorString( error ) );
		success = false;
	}
	
	return success;
}

SDL_Window* SDL2Window::returnWindowPointer()
{
return mWindow;
}

SDL_Renderer* SDL2Window::returnRenderer()
{
return mRenderer;
}

void SDL2Window::handleEvent( SDL_Event& e )
{
	//If an event was detected for this window
	if( e.type == SDL_WINDOWEVENT && e.window.windowID == windowID )
	{
		switch( e.window.event )
		{
			//Window appeared
			case SDL_WINDOWEVENT_SHOWN:
			windowShown = true;
			break;

			//Window disappeared
			case SDL_WINDOWEVENT_HIDDEN:
			windowShown = false;
			break;

			//Get new dimensions and repaint
			case SDL_WINDOWEVENT_SIZE_CHANGED:
			windowWidth = e.window.data1;
			windowHeight = e.window.data2;
			SDL_RenderPresent( mRenderer );
			break;

			//Repaint on expose
			case SDL_WINDOWEVENT_EXPOSED:
			SDL_RenderPresent( mRenderer );
			break;

			//Mouse enter
			case SDL_WINDOWEVENT_ENTER:
			windowMouseFocus = true;
			break;
			
			//Mouse exit
			case SDL_WINDOWEVENT_LEAVE:
			windowMouseFocus = false;
			break;

			//Keyboard focus gained
			case SDL_WINDOWEVENT_FOCUS_GAINED:
			windowKeyboardFocus = true;
			break;
			
			//Keyboard focus lost
			case SDL_WINDOWEVENT_FOCUS_LOST:
			windowKeyboardFocus = false;
			break;

			//Window minimized
			case SDL_WINDOWEVENT_MINIMIZED:
			windowMinimized = true;
			break;

			//Window maxized
			case SDL_WINDOWEVENT_MAXIMIZED:
			windowMinimized = false;
			break;
			
			//Window restored
			case SDL_WINDOWEVENT_RESTORED:
			windowMinimized = false;
			break;

			//Hide on close
			case SDL_WINDOWEVENT_CLOSE:
			SDL_HideWindow( mWindow );
			break;
		}
	}

	if (eventCallbackFunc!=NULL)
		eventCallbackFunc(e);
}

void SDL2Window::focus()
{
	//Restore window if needed
	if( !windowShown )
	{
		SDL_ShowWindow( mWindow );
	}

	//Move window forward
	SDL_RaiseWindow( mWindow );
}

void SDL2Window::registerCallback(void (*callback)(void), callback_t func_type)
{
if (func_type==GRAPHICS_RENDERER)
	graphicsCallbackFunc=callback;
}

void SDL2Window::registerCallback(void (*callback)(SDL_Event &e), callback_t func_type)
{
if (func_type==EVENT_HANDLER)
	eventCallbackFunc=callback;
}

void SDL2Window::render()
{
	if( !windowMinimized )
	{	
		graphicsCallbackFunc();
	}
}

void SDL2Window::free()
{
	if( mWindow != NULL )
	{
		SDL_DestroyWindow( mWindow );
	}

	windowMouseFocus = false;
	windowKeyboardFocus = false;
	windowWidth = 0;
	windowHeight = 0;
}

int SDL2Window::getWidth()
{
	return windowWidth;
}

int SDL2Window::getHeight()
{
	return windowHeight;
}

bool SDL2Window::hasMouseFocus()
{
	return windowMouseFocus;
}

bool SDL2Window::hasKeyboardFocus()
{
	return windowKeyboardFocus;
}

bool SDL2Window::isMinimized()
{
	return windowMinimized;
}

bool SDL2Window::isShown()
{
	return windowShown;
}

#endif
