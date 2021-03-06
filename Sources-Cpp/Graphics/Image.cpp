// Copyright (C) 2013 Felix Ungman
//
// This file is part of the openwar platform (GPL v3 or later), see LICENSE.txt

#include "Image.h"
#include "Algorithms/GaussBlur.h"
#include "FrameBuffer.h"
#include <cstdlib>


#if defined(OPENWAR_PLATFORM_IOS) || defined(OPENWAR_PLATFORM_MAC)
#ifdef OPENWAR_PLATFORM_IOS
#import <UIKit/UIKit.h>
#else
#import <AppKit/AppKit.h>
#endif
#endif

#ifdef __ANDROID__
#include <android/log.h>
#define LOGERR(err) __android_log_print(ANDROID_LOG_INFO, "openwar", "Image: %s", err);
#else
#define LOGERR(err)
#endif


void Image::swap(Image& a, Image& b)
{
	using std::swap;

#ifdef OPENWAR_USE_SDL
	swap(a._surface, b._surface);
#endif
#if defined(OPENWAR_PLATFORM_IOS) || defined(OPENWAR_PLATFORM_MAC)
	swap(a._context, b._context);
	swap(a._image, b._image);
#endif
	swap(a._width, b._width);
	swap(a._height, b._height);
	swap(a._pixelDensity, b._pixelDensity);
	swap(a._pixels, b._pixels);
	swap(a._owner, b._owner);
}


Image::Image()
{
}


Image::Image(int width, int height) :
	_width(width),
	_height(height)
{
	_pixels = (unsigned char*)std::calloc((size_t)(_width * _height), 4);
	_owner = true;
}


Image::~Image()
{
#ifdef OPENWAR_USE_SDL

	if (_surface)
		SDL_FreeSurface(_surface);

#endif

#if defined(OPENWAR_PLATFORM_IOS) || defined(OPENWAR_PLATFORM_MAC)

	if (_context != NULL)
		CGContextRelease(_context);

	if (_image != NULL)
		CGImageRelease(_image);

#endif

	if (_owner)
		std::free(_pixels);
}


/*Image::Image(const Image& image) :
	_width(image._width),
	_height(image._height),
	_pixelDensity(image._pixelDensity)
{
	_pixels = (unsigned char*) calloc((size_t)(_width * _height), 4);
	std::memcpy(_pixels, image._pixels, (size_t)(_width * _height) * 4);
	_owner = true;
}*/


Image::Image(Image&& image)
{
	swap(*this, image);
}


Image& Image::operator=(Image&& image)
{
	swap(*this, image);
	return *this;
}


float Image::GetPixelDensity() const
{
	return _pixelDensity;
}


void Image::SetPixelDensity(float value)
{
	_pixelDensity = value;
}


Image& Image::ReadPixels(FrameBuffer* frameBuffer)
{
	GLint oldFrameBufferId;
	glGetIntegerv(GL_FRAMEBUFFER_BINDING, &oldFrameBufferId);

	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer->_id);

	glReadPixels(0, 0, _width, _height, GL_RGBA, GL_UNSIGNED_BYTE, _pixels);

	glBindFramebuffer(GL_FRAMEBUFFER, static_cast<GLuint>(oldFrameBufferId));

	return *this;
}


Image& Image::LoadFromData(const void* data, size_t size)
{
#if defined(OPENWAR_PLATFORM_IOS) || defined(OPENWAR_PLATFORM_MAC)

    /*
    Mac ???

	NSImage* img = [[NSImage alloc] initWithData:[NSData dataWithBytes:data length:size]];
	NSRect rect = NSMakeRect(0, 0, img.size.width, img.size.height);
	LoadFromCGImage([img CGImageForProposedRect:&rect context:nil hints:nil], 1);
	[img release];
     */

	CGDataProviderRef dataProvider = CGDataProviderCreateWithData(NULL, data, size, NULL);
	CGImageRef image = CGImageCreateWithPNGDataProvider(dataProvider, nil, true, kCGRenderingIntentDefault);

	LoadFromCGImage(image, 1);

	CGImageRelease(image);
	CGDataProviderRelease(dataProvider);


#elif defined(OPENWAR_USE_SDL)

	SDL_RWops* src = SDL_RWFromConstMem(data, static_cast<int>(size));
	SDL_Surface* surface = IMG_Load_RW(src, 0);
	if (surface)
	    surface = EnsureSurfaceFormat(surface);
	if (surface)
		LoadFromSurface(surface);

	SDL_FreeRW(src);
	_surface = surface;

#endif

	return *this;
}


Image& Image::LoadFromResource(const Resource& r)
{
#if defined(OPENWAR_PLATFORM_IOS) || defined(OPENWAR_PLATFORM_MAC)
#ifdef OPENWAR_PLATFORM_IOS

	NSString* name = [NSString stringWithFormat:@"%@%@", [NSString stringWithUTF8String:r.name()], [NSString stringWithUTF8String:r.type()]];
	UIImage* image = [UIImage imageNamed:name];
	if (image != nil)
	{
		LoadFromCGImage(image.CGImage, image.scale);
	}
	else
	{
		NSLog(@"image not found: %@", name);
	}

#else

	NSString* path = [NSString stringWithUTF8String:r.path()];
	NSImage* image = [[NSImage alloc] initWithContentsOfFile:path];
	if (image != nil)
	{
		LoadFromCGImage([image CGImageForProposedRect:nil context:nil hints:nil], 1);
	}
	else
	{
		NSString* name = [NSString stringWithFormat:@"%@%@", [NSString stringWithUTF8String:r.name()], [NSString stringWithUTF8String:r.type()]];
		NSLog(@"image not found: %@", name);
	}

#endif

#endif

#ifdef OPENWAR_USE_SDL


    _surface = IMG_Load(r.path());
	if (_surface)
    	_surface = EnsureSurfaceFormat(_surface);

    if (_surface)
    {
		_width = _surface->w;
		_height = _surface->h;
		_pixels = (unsigned char*)_surface->pixels ;
		_owner = false;
		PremultiplyAlpha();
	}
	else
	{
    	_width = 0;
		_height = 0;
		_pixels = nullptr;
		_owner = false;
	}

#endif
    
    return *this;

}


#ifdef OPENWAR_USE_SDL
void Image::LoadFromSurface(SDL_Surface* surface)
{
	_width = surface->w;
	_height = surface->h;
	_pixels = (unsigned char*)surface->pixels;
	_owner = false;
	_surface = nullptr;
#if defined(OPENWAR_PLATFORM_IOS) || defined(OPENWAR_PLATFORM_MAC)
	_context = nil;
	_image = nil;
#endif
}
#endif


#ifdef OPENWAR_USE_SDL
SDL_Surface* Image::GetSurface() const
{
	if (!_surface)
	{
		_surface = SDL_CreateRGBSurfaceFrom(_pixels, _width, _height, 32, _width * 4, 0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000);
	}

	return _surface;
}
#endif


#ifdef OPENWAR_USE_SDL
SDL_Surface* Image::EnsureSurfaceFormat(SDL_Surface* surface)
{
	if (surface->format->format == SDL_PIXELFORMAT_ABGR8888)
		return surface;

	SDL_Surface* converted = SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_ABGR8888, 0);
	SDL_FreeSurface(surface);
	return converted;
}
#endif


#if defined(OPENWAR_PLATFORM_IOS) || defined(OPENWAR_PLATFORM_MAC)
void Image::LoadFromCGImage(CGImageRef image, float pixelDensity)
{
	_width = CGImageGetWidth(image);
	_height = CGImageGetHeight(image);
	_pixelDensity = pixelDensity;
	_pixels = (unsigned char*)calloc((size_t)(_width * _height), 4);
	_owner = true;

	CGRect rect = CGRectMake(0.0f, 0.0f, (CGFloat)_width, (CGFloat)_height);

	CGContextDrawImage(GetCGContext(), rect, image);
}
#endif



#if defined(OPENWAR_PLATFORM_IOS) || defined(OPENWAR_PLATFORM_MAC)
CGContextRef Image::GetCGContext() const
{
	if (_context == NULL)
	{
		CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();

		size_t width = (size_t)_width;
		size_t height = (size_t)_height;
		size_t bitsPerComponent = 8;
		size_t bytesPerRow = width * 4;
		_context = CGBitmapContextCreate(_pixels, width, height, bitsPerComponent, bytesPerRow, colorSpace, kCGImageAlphaPremultipliedLast);

		CGColorSpaceRelease(colorSpace);
	}
	return _context;
}
#endif


#if defined(OPENWAR_PLATFORM_IOS) || defined(OPENWAR_PLATFORM_MAC)
CGImageRef Image::GetCGImage() const
{
	if (_image == NULL)
	{
		size_t width = (size_t)_width;
		size_t height = (size_t)_height;
		size_t bitsPerComponent = 8;
		size_t bitsPerPixel = bitsPerComponent * 4;
		size_t bytesPerRow = width * 4;

		CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
		CGDataProviderRef dataProvider = CGDataProviderCreateWithData(NULL, _pixels, bytesPerRow * height, NULL);

		_image = CGImageCreate(width, height, bitsPerComponent, bitsPerPixel, bytesPerRow, colorSpace, kCGImageAlphaPremultipliedLast, dataProvider, NULL, false, kCGRenderingIntentDefault);

		CGDataProviderRelease(dataProvider);
		CGColorSpaceRelease(colorSpace);
	}

	return _image;
}
#endif


int Image::GetWidth() const
{
	return _width;
}


int Image::GetHeight() const
{
	return _height;
}


const void* Image::GetPixels() const
{
	return _pixels;
}


glm::vec4 Image::GetPixel(int x, int y) const
{
	if (0 <= x && x < _width && 0 <= y && y < _height)
	{
		const unsigned char* p = _pixels + 4 * (x + _width * y);
		constexpr float k = 1.0f / 255.0f;
		return glm::vec4(k * p[0], k * p[1], k * p[2], k * p[3]);
	}
	return glm::vec4();
}


void Image::SetPixel(int x, int y, glm::vec4 c)
{
	if (0 <= x && x < _width && 0 <= y && y < _height)
	{
		bounds1f bounds(0, 255);
		unsigned char* p = _pixels + 4 * (x + _width * y);
		p[0] = (unsigned char)glm::round(bounds.clamp(c.r * 255));
		p[1] = (unsigned char)glm::round(bounds.clamp(c.g * 255));
		p[2] = (unsigned char)glm::round(bounds.clamp(c.b * 255));
		p[3] = (unsigned char)glm::round(bounds.clamp(c.a * 255));
	}
}


static int CalculateSaturation(const unsigned char* pixel)
{
	int r = pixel[0];
	int g = pixel[1];
	int b = pixel[2];
	int M = glm::max(glm::max(r, g), b);
	int m = glm::min(glm::min(r, g), b);
	int C = M - m;
	int V = M;
	return V == 0 ? 0 : 255 * C / V;
}


bool Image::IsGrayscale() const
{
	const unsigned char* end = _pixels + _height * _width * 4;
	for (const unsigned char* pixel = _pixels; pixel != end; pixel += 4)
		if (pixel[3] > 32 && CalculateSaturation(pixel) > 16)
			return false;

	return true;
}


void Image::PremultiplyAlpha()
{
	for (int y = 0; y < _height; ++y)
		for (int x = 0; x < _width; ++x)
			{
				glm::vec4 c = GetPixel(x, y);
				c.r *= c.a;
				c.g *= c.a;
				c.b *= c.a;
				SetPixel(x, y, c);
			}
}


void Image::NormalizeAlpha()
{
	float alpha = 0;

	for (int y = 0; y < _height; ++y)
		for (int x = 0; x < _width; ++x)
			alpha = glm::max(alpha, GetPixel(x, y).a);

	if (alpha > 0.01f)
	{
		alpha = glm::pow(alpha, 0.9f);
		for (int y = 0; y < _height; ++y)
			for (int x = 0; x < _width; ++x)
			{
				glm::vec4 c = GetPixel(x, y);
				c.a /= alpha;
				SetPixel(x, y, c);
			}
	}
}


void Image::ApplyBlurFilter(float radius)
{
	int w = GetWidth();
	int h = GetHeight();
	glm::vec4* scl = new glm::vec4[w * h];
	glm::vec4* tcl = new glm::vec4[w * h];

	int i = 0;
	for (int y = 0; y < h; ++y)
		for (int x = 0; x < w; ++x)
		{
			scl[i] = tcl[i] = GetPixel(x, y);
			++i;
		}

	GaussBlur(scl, tcl, w, h, _pixelDensity * radius);

	i = 0;
	for (int y = 0; y < h; ++y)
		for (int x = 0; x < w; ++x)
			SetPixel(x, y, tcl[i++]);

	delete [] scl;
	delete [] tcl;

}


void Image::ApplyPixelFilter(std::function<glm::vec4(glm::vec4)> filter)
{
	for (int y = 0; y < _height; ++y)
		for (int x = 0; x < _width; ++x)
			SetPixel(x, y, filter(GetPixel(x, y)));
}


void Image::ApplyCircleMask()
{
	glm::vec2 center = 0.5f * glm::vec2(_width - 1, _height - 1);
	float radius = 0.3f + 0.5f * glm::max(_width, _height);

	for (int y = 0; y < _height; ++y)
		for (int x = 0; x < _width; ++x)
		{
			float r = glm::length(glm::vec2(x, y) - center);
			float a = glm::clamp(radius - r, 0.0f, 1.0f);
			glm::vec4 c = GetPixel(x, y);
			SetPixel(x, y, c * a);
		}
}


void Image::Copy(const Image& image, int x, int y)
{
	Draw(image, x, y, image.GetWidth(), image.GetHeight());
}


void Image::Draw(const Image& image, int x, int y, int w, int h)
{
#if defined(OPENWAR_PLATFORM_IOS) || defined(OPENWAR_PLATFORM_MAC)

	CGContextRef context = GetCGContext();
	CGRect rect = CGRectMake(x, _height - y - h, w, h);
	CGContextClearRect(context, rect);
	CGContextDrawImage(context, rect, image.GetCGImage());

#elif defined(OPENWAR_USE_SDL)

	SDL_Surface* src = image.GetSurface();
	SDL_Surface* dst = GetSurface();

	SDL_Rect srcRect;
	srcRect.x = 0;
	srcRect.y = 0;
	srcRect.w = src->w;
	srcRect.h = src->h;

	SDL_Rect dstRect;
	dstRect.x = x;
	dstRect.y = y;
	dstRect.w = w;
	dstRect.h = h;

	SDL_BlendMode oldBlendMode;
	SDL_GetSurfaceBlendMode(src, &oldBlendMode);
	SDL_SetSurfaceBlendMode(src, SDL_BLENDMODE_NONE);

	if (srcRect.w == dstRect.w && srcRect.h == dstRect.h)
		SDL_BlitSurface(src, &srcRect, dst, &dstRect);
	else
		SDL_BlitScaled(src, &srcRect, dst, &dstRect);

	SDL_SetSurfaceBlendMode(src, oldBlendMode);

#endif
}


void Image::Fill(const glm::vec4& color, const bounds2f& bounds)
{
#if defined(OPENWAR_PLATFORM_IOS) || defined(OPENWAR_PLATFORM_MAC)

	CGContextRef context = GetCGContext();

	//NSGraphicsContext* gc = [NSGraphicsContext graphicsContextWithGraphicsPort:context flipped:YES];
	//[NSGraphicsContext saveGraphicsState];
	//[NSGraphicsContext setCurrentContext:gc];

	CGContextSetRGBFillColor(context, color.r, color.g, color.b, color.a);
	CGRect rect = CGRectMake(bounds.min.x, bounds.min.y, bounds.x().size(), bounds.y().size());
	CGContextClearRect(context, rect);
	CGContextFillRect(context, rect);

	//[NSGraphicsContext restoreGraphicsState];

#elif defined(OPENWAR_USE_SDL)

	SDL_Surface* dst = GetSurface();

	SDL_Rect rect;
	rect.x = static_cast<int>(bounds.min.x);
	rect.y = static_cast<int>(bounds.min.y);
	rect.w = static_cast<int>(bounds.x().size());
	rect.h = static_cast<int>(bounds.y().size());

	Uint8 r = (Uint8)glm::round(255 * color.r);
	Uint8 g = (Uint8)glm::round(255 * color.g);
	Uint8 b = (Uint8)glm::round(255 * color.b);
	Uint8 a = (Uint8)glm::round(255 * color.a);
	Uint32 c = SDL_MapRGBA(dst->format, r, g, b, a);

	SDL_FillRect(dst, &rect, c);

#endif
}


void Image::Resize(int width, int height)
{
	Image image(width, height);
	image.Draw(*this, 0, 0, width, height);
	*this = std::move(image);
}


#if !defined(OPENWAR_PLATFORM_IOS) && !defined(OPENWAR_PLATFORM_MAC)
#include "savepng.h"
#endif


std::vector<char> ConvertImageToPng(const Image& image)
{
#if defined(OPENWAR_PLATFORM_IOS)

	NSData* data = UIImagePNGRepresentation([UIImage imageWithCGImage:image.GetCGImage()]);
	const char* p = reinterpret_cast<const char*>(data.bytes);
	return std::vector<char>(p, p + data.length);

#elif defined(OPENWAR_PLATFORM_MAC)

	unsigned char* pixels = reinterpret_cast<unsigned char*>(const_cast<GLvoid*>(image.GetPixels()));
	NSBitmapImageRep* imageRep = [[NSBitmapImageRep alloc] initWithBitmapDataPlanes:&pixels
		pixelsWide:image.GetWidth()
		pixelsHigh:image.GetHeight()
		bitsPerSample:8
		samplesPerPixel:4
		hasAlpha:YES
		isPlanar:NO
		colorSpaceName:NSDeviceRGBColorSpace
		bytesPerRow:4 * image.GetWidth()
		bitsPerPixel:32];
	NSData* data = [imageRep representationUsingType:NSPNGFileType properties:[[NSDictionary alloc] init]];
	const char* p = reinterpret_cast<const char*>(data.bytes);
	return std::vector<char>(p, p + data.length);

#else

    std::vector<char> result(196 * 1024);
    SDL_RWops* rw = SDL_RWFromMem(result.data(), result.size());
    SDL_SavePNG_RW(image._surface, rw, 0);
    std::size_t size = SDL_RWtell(rw);
    SDL_RWclose(rw);
    result.erase(result.begin() + size, result.end());
    return result;

#endif
}


#if defined(OPENWAR_PLATFORM_IOS) || defined(OPENWAR_PLATFORM_MAC)
NSData* ConvertImageToTiff(const Image& image)
{
#ifdef OPENWAR_PLATFORM_IOS
	return nil;
#else
	unsigned char* pixels = reinterpret_cast<unsigned char*>(const_cast<GLvoid*>(image.GetPixels()));
	NSBitmapImageRep* imageRep = [[NSBitmapImageRep alloc] initWithBitmapDataPlanes:&pixels
		pixelsWide:image.GetWidth()
		pixelsHigh:image.GetHeight()
		bitsPerSample:8
		samplesPerPixel:4
		hasAlpha:YES
		isPlanar:NO
		colorSpaceName:NSDeviceRGBColorSpace
		bytesPerRow:4 * image.GetWidth()
		bitsPerPixel:32];
	NSData* result = [imageRep TIFFRepresentationUsingCompression:NSTIFFCompressionLZW factor:0.5];
	return result;
#endif
}
#endif


#if defined(OPENWAR_PLATFORM_IOS) || defined(OPENWAR_PLATFORM_MAC)
Image* ConvertTiffToImage(NSData* data)
{
#ifdef OPENWAR_PLATFORM_IOS
	Image* result = new Image();
	result->LoadFromCGImage([[UIImage imageWithData:data] CGImage], 1);
	return result;
#else
	NSImage* img = [[NSImage alloc] initWithData:data];
	NSSize size = img.size;
	NSRect rect = NSMakeRect(0, 0, size.width, size.height);
	Image* result = new Image();
	result->LoadFromCGImage([img CGImageForProposedRect:&rect context:nil hints:nil], 1);
	return result;
#endif
}
#endif
