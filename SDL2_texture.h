/*
*	My own header for creating SDL2 textures
*	Base taken from lazyfoo.org
*/

#ifndef	_CSRP_SDL2_TEXTURE_H_
#define	_CSRP_SDL2_TEXTURE_H_	1

//Texture wrapper class
class SDL2Texture
{
	public:
		//Initializes variables
		SDL2Texture(SDL_Renderer*, TTF_Font*);

		//Deallocates memory
		~SDL2Texture();

		//Loads image at specified path
		bool loadFromFile( std::string path );
		
		#ifdef _SDL_TTF_H
		//Creates image from font string
		bool loadFromRenderedText( std::string textureText, SDL_Color textColor );
		#endif

		//Deallocates texture
		void free();

		//Set color modulation
		void setColor( Uint8 red, Uint8 green, Uint8 blue );

		//Set blending
		void setBlendMode( SDL_BlendMode blending );

		//Set alpha modulation
		void setAlpha( Uint8 alpha );
		
		//Renders texture at given point
		void render( int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE );

		//Gets image dimensions
		int getWidth();
		int getHeight();

	private:
		//Prevent access to default constructer
		SDL2Texture();

		//The actual hardware texture
		SDL_Texture* mTexture;

		//Renderer
		SDL_Renderer *gRenderer;

		#ifdef _SDL_TTF_H
		//Font to use
		TTF_Font *gFont;
		#endif

		//Image dimensions
		int mWidth;
		int mHeight;
};

SDL2Texture::SDL2Texture()
{
	//Initialize
	mTexture = NULL;
	gRenderer = NULL;
	gFont = NULL;
	mWidth = 0;
	mHeight = 0;
}

SDL2Texture::SDL2Texture(SDL_Renderer *gRender, TTF_Font *gFon)
{
	//Initialize
	mTexture = NULL;
	gRenderer = gRender;
	gFont = gFon;
	mWidth = 0;
	mHeight = 0;
}

SDL2Texture::~SDL2Texture()
{
	//Deallocate
	free();
}

bool SDL2Texture::loadFromFile( std::string path )
{
	//Get rid of preexisting texture
	free();

	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
	if( loadedSurface == NULL )
	{
		printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
	}
	else
	{
		//Color key image
		SDL_SetColorKey( loadedSurface, SDL_TRUE, SDL_MapRGB( loadedSurface->format, 0, 0xFF, 0xFF ) );

		//Create texture from surface pixels
        	newTexture = SDL_CreateTextureFromSurface( gRenderer, loadedSurface );
		if( newTexture == NULL )
		{
			printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
		}
		else
		{
			//Get image dimensions
			mWidth = loadedSurface->w;
			mHeight = loadedSurface->h;
		}

		//Get rid of old loaded surface
		SDL_FreeSurface( loadedSurface );
	}

	//Return success
	mTexture = newTexture;
	return mTexture != NULL;
}

#ifdef _SDL_TTF_H
bool SDL2Texture::loadFromRenderedText( std::string textureText, SDL_Color textColor )
{
	//Get rid of preexisting texture
	free();

	//Render text surface
	SDL_Surface* textSurface = TTF_RenderText_Solid( gFont, textureText.c_str(), textColor );
	if( textSurface != NULL )
	{
		//Create texture from surface pixels
        mTexture = SDL_CreateTextureFromSurface( gRenderer, textSurface );
		if( mTexture == NULL )
		{
			printf( "Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError() );
		}
		else
		{
			//Get image dimensions
			mWidth = textSurface->w;
			mHeight = textSurface->h;
		}

		//Get rid of old surface
		SDL_FreeSurface( textSurface );
	}
	else
	{
		printf( "Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError() );
	}

	
	//Return success
	return mTexture != NULL;
}
#endif

void SDL2Texture::free()
{
	//Free texture if it exists
	if( mTexture != NULL )
	{
		SDL_DestroyTexture( mTexture );
		mTexture = NULL;
		mWidth = 0;
		mHeight = 0;
	}
}

void SDL2Texture::setColor( Uint8 red, Uint8 green, Uint8 blue )
{
	//Modulate texture rgb
	SDL_SetTextureColorMod( mTexture, red, green, blue );
}

void SDL2Texture::setBlendMode( SDL_BlendMode blending )
{
	//Set blending function
	SDL_SetTextureBlendMode( mTexture, blending );
}
		
void SDL2Texture::setAlpha( Uint8 alpha )
{
	//Modulate texture alpha
	SDL_SetTextureAlphaMod( mTexture, alpha );
}

void SDL2Texture::render( int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip )
{
	//Set rendering space and render to screen
	SDL_Rect renderQuad = { x, y, mWidth, mHeight };

	//Set clip rendering dimensions
	if( clip != NULL )
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}

	//Render to screen
	SDL_RenderCopyEx( gRenderer, mTexture, clip, &renderQuad, angle, center, flip );
}

int SDL2Texture::getWidth()
{
	return mWidth;
}

int SDL2Texture::getHeight()
{
	return mHeight;
}

#endif
