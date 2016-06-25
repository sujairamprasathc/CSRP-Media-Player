/********************************************
*   CSRP Media Player                       *
*   Version 0.1                             *
*   Written by Sujai Ram Prasath C          *
*                                           *
*   Download the up-to-date full source     *
*   code from github                        *
*                                           *
*   Mention my name if you use this as a    *
*   base/reference for your app/game        *
********************************************/


#include <iostream>
#include <fstream>
#include <dirent.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_opengl.h>

#include <GL/glu.h>

#include "SDL2_windows.h"
#include "SDL2_texture.h"



#ifdef __linux__
    #define     TEMP_PATH   "/tmp/CSRP"
    #define     mkdir(x)    system("mkdir " x)
#endif

using namespace std;



SDL2Window main_Win;
SDL2Window controls_Win;
SDL_Renderer *gRenderer = NULL;
SDL_Surface *screenSurface=NULL;
SDL_Surface *startPageSurface=NULL;
TTF_Font *gFont = NULL;

Mix_Music *gMusic = NULL;

SDL_GLContext gContext;


int VOLUME;


bool initialize()
{
/*
    @brief:
    Initialize SDL2 and return success status
*/
if( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_AUDIO ) < 0 )
{
    cout<<"Application couldn't be initialized properly... Try reopening the program..."<<endl;
    return false;
}
else
{
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 2 );
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 1 );

    if( main_Win.init(640,480,"CSRP Media Player",10,10) == false )
    {
        cout<<"Error creating window... Reopen the program"<<endl;
        return false;
    }
    else
    {
        screenSurface = SDL_GetWindowSurface( main_Win.returnWindowPointer() );
        gContext = SDL_GL_CreateContext( main_Win.returnWindowPointer() );
        if( gContext == NULL )
        {
            printf( "OpenGL context could not be created! SDL Error: %s\n", SDL_GetError() );
            return false;
        }
        else
        {
            if( SDL_GL_SetSwapInterval( 1 ) < 0 )
                printf( "Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError() );

            if( TTF_Init() == -1 )
            {
                printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
                return false;
            }
            else
            {
                gFont = TTF_OpenFont( "Font.ttf", 14 );
                if( gFont == NULL )
                {
                    printf( "Failed to load font! SDL_ttf Error: %s\n", TTF_GetError() );
                    return false;
                }
            }
        }
    }

    if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 )
    {
        cout<<"Audio couldn't be initialized properly..."<<endl;
        return false;
    }
}

return true;
}






void play_Music(const char * _File_Name_)
{
/*
    @brief:
    Play music file requested by passing the _File_Name_

    @description:
    This function checks if the specified file exists or
    not, and throws an error if file can't be found.  If
    the file exists, then it plays the music file, if no
    file is already being played.  It throws an error if
    a file is already being played.
*/
gMusic = Mix_LoadMUS( _File_Name_ );
if( gMusic == NULL )
    throw "Couldn't load the specified file...";
else  if (Mix_PlayingMusic()==1)
    throw "Already playing some music...";
else  if (Mix_PlayingMusic()==0)
    Mix_PlayMusic(gMusic,1);
}






void get_Song(std::string *strPoint)
{
/*
    @brief:
    Get the name of the song to be played from the user
    and place it in $strPoint

    @description:
*/

SDL2Window inWin;
inWin.init(640,64,"CSRP Media Player-Enter Song",670,320);
inWin.focus();
SDL_Color textColor = { 0, 0, 0, 0xFF };
gRenderer=inWin.returnRenderer();
SDL2Texture gInputTextTexture(gRenderer, gFont);

SDL_StartTextInput();

std::string FileName("/");

gInputTextTexture.loadFromRenderedText( FileName.c_str(), textColor );
SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
SDL_RenderClear( gRenderer );
gInputTextTexture.render( ( 640 - gInputTextTexture.getWidth() ) / 2, 10 );
SDL_RenderPresent( gRenderer );
inWin.focus();

SDL_Event e;
bool quit=false;
bool requireRender=true;

while (quit==false)
{
    while( SDL_PollEvent( &e ) != 0 )
    {
        if( e.type == SDL_QUIT )
            exit(0);

        if( e.type == SDL_TEXTINPUT )
        {
        FileName+=e.text.text;
        requireRender=true;
        }

        if( e.type == SDL_KEYDOWN )
        {
        switch( e.key.keysym.sym )
        {
        case SDLK_v:
            if (SDL_GetModState() & KMOD_CTRL)
                FileName += SDL_GetClipboardText();
            requireRender = true;
            break;

        case SDLK_RETURN:
            quit=true;
            break;

        case SDLK_BACKSPACE:
            if (FileName.length() > 0)
                FileName.pop_back();
            requireRender=true;
            break;

        case SDLK_ESCAPE:
            exit(0);

        default:
            break;
        }
        }
    }


    if (requireRender && FileName.length()!=0)
    {
        gInputTextTexture.loadFromRenderedText( FileName.c_str(), textColor );

        SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
        SDL_RenderClear( gRenderer );

        gInputTextTexture.render( ( 640 - gInputTextTexture.getWidth() ) / 2, 10 );

        SDL_RenderPresent( gRenderer );

        requireRender=false;
    }
    else if (requireRender && FileName.length()==0)
    {
        gInputTextTexture.loadFromRenderedText( " ", textColor );

        SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
        SDL_RenderClear( gRenderer );

        gInputTextTexture.render( ( 640 - gInputTextTexture.getWidth() ) / 2, 10 );

        SDL_RenderPresent( gRenderer );

        requireRender=false;
    }
}

SDL_StopTextInput();

inWin.free();
*strPoint=FileName;
}






void end_Of_Music_Handler()
{
/*
    @brief:
    Do whaterver you want when music finishes
*/

system("rm /tmp/CSRP/1.mp3");

std::string cP;
get_Song(&cP);

char newc[120]="cp ";
strcat(newc,cP.c_str());
strcat(newc," /tmp/CSRP/1.mp3");

system(newc);

try {play_Music("/tmp/CSRP/1.mp3");}
catch (const char * ERROR_DESCRIPTION)
{
    cout<<ERROR_DESCRIPTION<<endl;
    end_Of_Music_Handler();
}
}




void controls_Win_Callback()
{
/*
    @breif:
    Updates $controls_Win window with OpenGL drawings
*/

glClear(GL_COLOR_BUFFER_BIT);

glColor3f(1.0,1.0,1.0);

glBegin(GL_TRIANGLES);
    glVertex2f(-0.5,0.5);
    glVertex2f(-0.5,-0.5);
    glVertex2f(-0.3,0.0);
glEnd();

SDL_GL_SwapWindow( controls_Win.returnWindowPointer() );
}


//Start here
int main()
{
DIR *tmpD;
ifstream tmp_1("/tmp/CSRP/1.mp3");
tmpD=opendir("/tmp/CSRP");
if (tmpD==NULL)
    mkdir(TEMP_PATH);
else if (tmp_1!=NULL)
    system("rm /tmp/CSRP/1.mp3");
tmp_1.close();
closedir(tmpD);

if (!initialize())
    abort();


controls_Win.init(640,64,"CSRP Media Player",670,10);

controls_Win.registerCallback(controls_Win_Callback, GRAPHICS_RENDERER);
controls_Win.render();

//Set up initial display
startPageSurface = SDL_LoadBMP( "Start_Page.bmp" );
if( startPageSurface == NULL )
    cout<<"ERROR : Can't find init display image..."<<endl;
else
{
    SDL_BlitSurface( startPageSurface, NULL, screenSurface, NULL );
    SDL_UpdateWindowSurface( main_Win.returnWindowPointer() );
    SDL_Delay( 2000 );
}

SONG_SELECT:
std::string cP;
get_Song(&cP);

char newc[120]="cp ";
strcat(newc,cP.c_str());
strcat(newc," /tmp/CSRP/1.mp3");

system(newc);

try {play_Music("/tmp/CSRP/1.mp3");}
catch (const char * ERROR_DESCRIPTION)
{
    cout<<ERROR_DESCRIPTION<<endl;
    goto SONG_SELECT;
}

Mix_HookMusicFinished(end_Of_Music_Handler);
VOLUME=Mix_VolumeMusic(-1);

SDL_Event e;

while(true)
{
    if( startPageSurface == NULL )
    {
        startPageSurface = SDL_LoadBMP( "Start_Page.bmp" );
    }
    else
    {
        SDL_BlitSurface( startPageSurface, NULL, screenSurface, NULL );
        SDL_UpdateWindowSurface( main_Win.returnWindowPointer() );
    }

    while( SDL_PollEvent( &e ) != 0 )
    {
        if( e.type == SDL_QUIT )
            exit(0);

        controls_Win.handleEvent(e);

        if( e.type == SDL_KEYDOWN )
        {
        switch( e.key.keysym.sym )
        {
        case SDLK_SPACE:
            if( Mix_PlayingMusic() == 0 )
                Mix_PlayMusic( gMusic, -1 );
            else
            {
                if( Mix_PausedMusic() == 1 )
                    Mix_ResumeMusic();
                else
                    Mix_PauseMusic();
            }
            break;

        case SDLK_EQUALS:
        case SDLK_KP_PLUS:
            if (VOLUME<128)
                Mix_VolumeMusic(++VOLUME);
            break;

        case SDLK_MINUS:
        case SDLK_KP_MINUS:
            if (VOLUME>0)
                Mix_VolumeMusic(--VOLUME);
            break;

        case SDLK_RETURN:
            if (Mix_PlayingMusic()==1)
            {
                Mix_HaltMusic();
                gMusic=NULL;
            }
            else
                play_Music("/tmp/CSRP/1.mp3");
            break;

        case SDLK_ESCAPE:
            exit(0);

        default:
            break;
        }
        }
    }
}

return 0;
}
