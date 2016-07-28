/****************************************************
*   CSRP_SDL_engine.H                               *
*   version 0.1                                     *
*                                                   *
*   Written by Sujai Ram Prasath C                  *
*                                                   *
*   Written to simplify using SDL2                  *
*   This is the master file, including which        *
*   will inlcude all SDL codes I've written.        *
****************************************************/


#ifndef _CSRP_SDL2_ENGINE_H_
#define _CSRP_SDL2_ENGINE_H_    1


#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL2_gfxPrimitives.h>

#include <GL/glu.h>

#include "CSRP_SDL2_windows.h"
#include "CSRP_SDL2_texture.h"



namespace CSRP_SDL2     //Prevent name conflicts
{


//Initialize SDL2
bool SDL2_Engine_init();


//DEFNITIONS
//Move to some other file
bool SDL2_Engine_init()
{
if( SDL_Init( SDL_INIT_EVERYTHING ) < 0 )   //Starting initiation
    return false;
if( TTF_Init() != 0 )   //Initialize SDL2_ttf
    return false;
if( Mix_Init( MIX_INIT_FLAC | MIX_INIT_MOD | MIX_INIT_MODPLUG | MIX_INIT_MP3 | MIX_INIT_OGG |MIX_INIT_FLUIDSYNTH ) < 6 )    //Initialize all music types
    return false;
if ( IMG_Init( IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_TIF | IMG_INIT_WEBP ) < 4 )
    return false;

atexit(IMG_Quit);
atexit(Mix_Quit);
atexit(TTF_Quit);
atexit(SDL_Quit);

return true;
}


}   //namespace


#endif /*_CSRP_SDL2_ENGINE_H_*/
