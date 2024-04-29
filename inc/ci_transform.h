
#ifndef CI_TRANSFORM_H_
#define CI_TRANSFORM_H_


#include "ci_image.h"
#include "ci_result.h"


// Sets dst to the src image with its data scaled to the given dimensions
ci_result_t ciTransformScale(CI_Image* dst, CI_Image* src, size_t width, size_t height);

// Blits the src image onto the dst image (color formats must match and the blitting rect. must be within the dst's bounds)
void ciTransformBlit(CI_Image* dst, CI_Image* src, size_t x, size_t y);

// Fills the entire image with the given color (the color must match the image's color format)
void ciTransformFill(CI_Image* image, void* p_color);

// Flips the image horizontally
void ciTransformFlipH(CI_Image* image);

// Flips the image vertically
void ciTransformFlipV(CI_Image* image);

// Copies a snip of the src image to the dst image corresponding to the dst's w and h (color format must match and the snipping rect. must be within the dst's bounds)
void ciTransformSnip(CI_Image* dst, CI_Image* src, size_t x, size_t y);


#endif // CI_TRANSFORM_H_
