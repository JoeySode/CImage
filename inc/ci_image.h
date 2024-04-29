

#ifndef CI_IMAGE_H_
#define CI_IMAGE_H_


#include "ci_color.h"
#include "ci_result.h"

#include <stddef.h>
#include <stdint.h>


// A structure representing an image
typedef struct CI_Image
{
  void* _data;       // A pointer to the image's data
  size_t _w;         // The image's width
  size_t _h;         // The image's height
  color_fmt_t _fmt;  // The image's color format
}
CI_Image;


// Initializes the image with the given width height and color format
ci_result_t ciImageCreate(CI_Image* p_image, size_t width, size_t height, color_fmt_t fmt);

// Destroys the image
void ciImageDestroy(CI_Image* image);


// A pointer to the image's data
void* ciImageGetData(CI_Image* image);

// Copies the first image to the second, including the image data (dst should not have been created)
ci_result_t ciImageCopy(CI_Image* dst, CI_Image* src);

// The color format of the image
color_fmt_t ciImageGetFmt(CI_Image* image);

// The height of the image
size_t ciImageGetHeight(CI_Image* image);

// The width of the image
size_t ciImageGetWidth(CI_Image* image);

// Converts the given x and y coordinates to a singular index
size_t ciImageGetIndexXY(CI_Image* image, size_t x, size_t y);

// The number of pixels in the image
size_t ciImageGetNumPixels(CI_Image* image);

// The size of the image's data
size_t ciImageGetSize(CI_Image* image);


#endif // CI_IMAGE_H_
