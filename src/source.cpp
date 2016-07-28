/********************************************
*   CSRP Media Player                       *
*   Version 0.2                             *
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

#include "CSRP_SDL2_engine.H"



#ifdef __linux__
    #define     TEMP_PATH       "/tmp/CSRP"
    #define     mkdir(x)        system("mkdir " x)
    #define     ROOT_DIRECTORY  "/"
    #define     MUSIC_FILE      "/tmp/CSRP/1.mp3"
#else
    #define     TEMP_PATH       "C:/Windows/Temp/CSRP"
    #define     mkdir(x)        system("md " x)
    #define     ROOT_DIRECTORY  "G:/"
    #define     MUSIC_FILE      "C:/Windows/Temp/CSRP/1.mp3"
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

DIR *directory;
dirent *entries;
int i;
directory=opendir(ROOT_DIRECTORY);
string current_DIR(ROOT_DIRECTORY);
string text("HELLO!!!");
SDL2Window inWin;
inWin.init("CSRP Media Player-Enter Song",640,480,700,150);
int windowID = SDL_GetWindowID( inWin.returnWindowPointer() );
SDL_HideWindow(inWin.returnWindowPointer());
SDL_ShowWindow(inWin.returnWindowPointer());
SDL_RaiseWindow(inWin.returnWindowPointer());
inWin.focus();
SDL_Color textColor = { 0, 0, 0, 0xFF };
gRenderer=inWin.returnRenderer();
SDL2Texture gInputTextTexture(gRenderer, gFont);

SDL_StartTextInput();

START:
i=0;
string dirEntries[100];
while ((entries=readdir(directory))!=NULL)
{
    dirEntries[i]=entries->d_name;
    i++;
}

SDL_Event e;
bool quit=false;
bool requireRender=true;

while (quit==false)
{
    while( SDL_PollEvent( &e ) != 0 )
    {

        if (e.window.windowID == windowID)
        {
        switch (e.window.event)
        {
        case SDL_WINDOWEVENT_SHOWN:
        case SDL_WINDOWEVENT_HIDDEN:
        case SDL_WINDOWEVENT_MINIMIZED:
        case SDL_WINDOWEVENT_MAXIMIZED:
        case SDL_WINDOWEVENT_RESTORED:
            requireRender = true;
            break;
        }
        }

        if( e.type == SDL_QUIT )
            exit(0);

        if( e.type == SDL_TEXTINPUT )
        {
        text+=e.text.text;
        requireRender=true;
        }

        if( e.type == SDL_KEYDOWN )
        {
        switch( e.key.keysym.sym )
        {
        case SDLK_v:
            if (SDL_GetModState() & KMOD_CTRL)
                text += SDL_GetClipboardText();
            requireRender = true;
            break;

        case SDLK_RETURN:
            if ((text.find(".mp3")+1))
            {
                quit=true;
                break;
            }
            for (int z=0;z<=i;z++)
            {
                if (text==dirEntries[z])
                {
                    current_DIR+=dirEntries[z];
                    directory=opendir(current_DIR.c_str());
                    current_DIR+="/";
                    text="";
                    goto START;
                }
            }
            if (!(text.find(".mp3")+1))
            {
                SDL2Window alertBox;
                alertBox.init("CSRP Media Player---Alert",320,32,SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED);
                SDL_HideWindow(alertBox.returnWindowPointer());
                SDL_ShowWindow(alertBox.returnWindowPointer());
                SDL_RaiseWindow(alertBox.returnWindowPointer());
                SDL_SetRenderDrawColor( alertBox.returnRenderer(), 0xFF, 0xFF, 0xFF, 0xFF );
                SDL_RenderClear( alertBox.returnRenderer() );
                SDL2Texture textToRender(alertBox.returnRenderer(), gFont);

                textToRender.loadFromRenderedText( "NOT", textColor, 10 );
                textToRender.render( ( 320 - textToRender.getWidth() ) / 15, 0 );
                textToRender.loadFromRenderedText( "AN", textColor, 10 );
                textToRender.render( 2*( 320 - textToRender.getWidth() ) / 10, 0 );
                textToRender.loadFromRenderedText( "ACCEPTABLE", textColor, 10 );
                textToRender.render( 4.3*( 320 - textToRender.getWidth() ) / 10, 0 );
                textToRender.loadFromRenderedText( "FILE", textColor, 10 );
                textToRender.render( 7.3*( 320 - textToRender.getWidth() ) / 10, 0 );
                textToRender.loadFromRenderedText( "NAME", textColor, 10 );
                textToRender.render( 9.3*( 320 - textToRender.getWidth() ) / 10, 0 );

                SDL_RenderPresent( alertBox.returnRenderer() );
                SDL_Delay(2500);
                alertBox.free();
                break;
            }
            break;

        case SDLK_BACKSPACE:
            if (text.length() > 0)
                text.pop_back();
            requireRender=true;
            break;

        case SDLK_ESCAPE:
            exit(0);

        default:
            break;
        }
        }
    }


    if (requireRender)
    {
        SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
        SDL_RenderClear( gRenderer );

        int j=0,k=0;
        while (j<i)
        {
            string text=dirEntries[j];
            gInputTextTexture.loadFromRenderedText( text, textColor, 10 );

            j++;

            if (j%3==1)
                gInputTextTexture.render( ( 320 - gInputTextTexture.getWidth() ) / 2, k*25 );
            else if (j%3==2)
                gInputTextTexture.render( ( 640 - gInputTextTexture.getWidth() ) / 2, k*25 );
            else
            {
                gInputTextTexture.render( ( 960 - gInputTextTexture.getWidth() ) / 2, k*25 );
                k++;
            }
        }

        if (text.length()!=0)
        {
            gInputTextTexture.loadFromRenderedText( text, textColor, 10 );
            gInputTextTexture.render( ( 640 - gInputTextTexture.getWidth() ) / 2, 445 );
        }

        SDL_RenderPresent( gRenderer );
        requireRender=false;
    }
}

SDL_StopTextInput();

inWin.free();
closedir(directory);
*strPoint=current_DIR+text;
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

#ifdef __linux
char newc[120]="cp ";
#elif __Windows
char newc[120]="copy ";
#endif
strcat(newc,cP.c_str());
strcat(newc," " TEMP_PATH "/1.mp3");

system(newc);

try {play_Music(MUSIC_FILE);}
catch (const char * ERROR_DESCRIPTION)
{
    cout<<ERROR_DESCRIPTION<<endl;
    end_Of_Music_Handler();
}
}




void controls_Win_Graphics_Callback(SDL_Renderer* gRenderer)
{
/*
    @breif:
    Updates $controls_Win window with drawings
*/

SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0x00, 0xFF );
SDL_RenderClear( gRenderer );

//Play/Pause
if (Mix_PausedMusic()!=0)
{
    filledTrigonRGBA(gRenderer, 32, 16, 48, 32, 32, 48, 0xFF, 0xFF, 0xFF, 0xFF );
}
else
{
    SDL_Rect pauseBar1 = { 32, 16, 8, 32 };
    SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
    SDL_RenderFillRect( gRenderer, &pauseBar1 );

    SDL_Rect pauseBar2 = { 48, 16, 8, 32 };
    SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
    SDL_RenderFillRect( gRenderer, &pauseBar2 );
}

//Stop music
SDL_Rect stopButton = { 100, 16, 32, 32 };
SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
SDL_RenderFillRect( gRenderer, &stopButton );

//Volume circle
circleRGBA( gRenderer, 600, 32, 20, 0xFF, 0xFF, 0xFF, 0xFF );
filledCircleRGBA( gRenderer, 600, 32, ((float)VOLUME/128.0)*20.0, 0xFF, 0xFF, 0xFF, 0xFF );

SDL_RenderPresent( gRenderer );
}




void controls_Win_Event_Callback(SDL_Event &e)
{
int x, y;

switch( e.type )
{
    case SDL_MOUSEBUTTONDOWN:
        SDL_GetMouseState( &x, &y );

        if ( x < 60 and x > 20 )
        {
            if (Mix_PausedMusic()!=0)
                Mix_ResumeMusic();
            else
                Mix_PauseMusic();
        }
        else if ( x > 100 and x < 140 and Mix_PlayingMusic() == 1 )
        {
            Mix_HaltMusic();
            gMusic=NULL;
        }
        break;
    case SDL_MOUSEWHEEL:
        SDL_GetMouseState( &x, &y );

        if ( VOLUME < 128 and e.wheel.y > 0 and x > 580 and x < 620 )
            Mix_VolumeMusic(++VOLUME);
        if ( VOLUME > 0 and e.wheel.y < 0 and x > 580 and x < 620 )
            Mix_VolumeMusic(--VOLUME);
        break;
}
}




//Start here
int main(int argc, char* argv[])
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

if (!CSRP_SDL2::SDL2_Engine_init())
    abort();

gFont = TTF_OpenFont( "../res/Font.ttf", 14 );

if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 )
{
    cout<<"Audio couldn't be initialized properly..."<<endl;
}

main_Win.init("CSRP Media Player",640,480,50,50);
controls_Win.init("CSRP Media Player",640,64,50,600);

screenSurface = SDL_GetWindowSurface( main_Win.returnWindowPointer() );

controls_Win.registerCallback(controls_Win_Graphics_Callback, GRAPHICS_RENDERER);
controls_Win.registerCallback(controls_Win_Event_Callback, EVENT_HANDLER);
controls_Win.render();

//Set up initial display
startPageSurface = SDL_LoadBMP( "../res/Start_Page.bmp" );
if( startPageSurface == NULL )
    cout<<"ERROR : Can't find init display image..."<<endl;
else
{
    SDL_BlitSurface( startPageSurface, NULL, screenSurface, NULL );
    SDL_UpdateWindowSurface( main_Win.returnWindowPointer() );
}

SONG_SELECT:
std::string cP;
get_Song(&cP);

#ifdef __linux
char newc[120]="cp ";
#elif __Windows
char newc[120]="copy ";
#endif
strcat(newc,cP.c_str());
strcat(newc," " TEMP_PATH "/1.mp3");

system(newc);

try {play_Music(MUSIC_FILE);}
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
        startPageSurface = SDL_LoadBMP( "../res/Start_Page.bmp" );
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
            break;

        case SDLK_ESCAPE:
            exit(0);

        default:
            break;
        }
        }
        controls_Win.handleEvent(e);
        controls_Win.render();
    }
}

return 0;
}
