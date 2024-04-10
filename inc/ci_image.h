

#ifndef CI_IMAGE_H_

#define CI_IMAGE_H_


#include "ci_color.h"
#include "ci_result.h"

#include <stddef.h>
#include <stdint.h>


// A function that can be called for each pixel in an image
typedef void(*for_each_pixel_fn_t)(void* p_pixel, void* params);

// A function that can be called for each pixel in an image, using its index in the image's data
typedef void(*for_each_pixel_i_fn_t)(void* p_pixel, size_t i, void* params);

// A function that can be called for each pixel in an image, using its x and y value in the image's data
typedef void(*for_each_pixel_xy_fn_t)(void* p_pixel, size_t x, size_t y, void* params);

// An image that is represented by its width and height and a pointer to its pixel data (when indexing data, (0, 0) is the bottom left corner)
typedef struct image_t
{
  void* data;       // A pointer to the image's data (pointer value should only be modified by CImage functions, indexed values can be modified)
  size_t w;         // The image's width (should only be modified by CImage functions)
  size_t h;         // The image's height (should only be modified by CImage functions)
  color_fmt_t fmt;  // The image's color format (should only be modified by CImage functions)
}
image_t;


// Initializes the image with the given width height and color format
ci_result_t ciInitImage(image_t* p_image, size_t width, size_t height, color_fmt_t fmt);

// Destroys the image
void ciDestroyImage(image_t* p_image);


// Copies the first image to the second, including the image data (dst should not already have any data)
ci_result_t ciImageCopy(image_t* src, image_t* dst);

// Scales the image to the new width and height, stretching or shrinking the color data
ci_result_t ciImageScale(image_t* p_image, size_t new_width, size_t new_height);

// Converts the given x and y coordinates to a singular index
size_t ciImageIndexFromXY(image_t* p_image, size_t x, size_t y);

// The number of pixels in the image
size_t ciImageNumPixels(image_t* p_image);

// The size of the image's data
size_t ciImageSize(image_t* p_image);

// Blits the src image onto the dst image (color formats must match and the blitting rect. must be within the dst's bounds)
void ciImageBlit(image_t* src, image_t* dst, size_t x, size_t y);

// Fills the entire image with the given color (the color must match the image's color format)
void ciImageFill(image_t* p_image, void* p_color);

// Calls the given function for each pixel in the image
void ciImageForEach(image_t* p_image, for_each_pixel_fn_t func, void* params);

// Calls the given function for each pixel in the image, giving its index in the image's data (an index of 0 is bottom left)
void ciImageForEachI(image_t* p_image, for_each_pixel_i_fn_t func, void* params);

// Calls the given function for each pixel in the image, giving its x and y values in the image's data ((0, 0) is bottom left)
void ciImageForEachXY(image_t* p_image, for_each_pixel_xy_fn_t func, void* params);

// Copies a snip of the src image to the dst image corresponding to the dst's w and h (color format must match and the snipping rect. must be within the dst's bounds)
void ciImageSnip(image_t* src, image_t* dst, size_t x, size_t y);


#endif // CI_IMAGE_H_
