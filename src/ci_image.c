


#include "ci_image.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>


// Converts the x and y to an index for a 1D array
#define CI_XY_TO_INDEX(x, y, w) (((y) * (w)) + (x))


typedef struct image_t__
{
  void* data;       // A pointer to the image's data
  size_t w;         // The image's width
  size_t h;         // The image's height
  color_fmt_t fmt;  // The image's color format
}
image_t__;


ci_result_t ciImageCreate(image_t* p_image, size_t width, size_t height, color_fmt_t fmt)
{
  // Allocate the image
  image_t__* image = (image_t__*)malloc(sizeof(image_t__));

  if (!image)
    return CI_ERR_ALLOC;

  // Allocate the data
  image->data = (void*)calloc(width * height, ciColorFmtSize(fmt));

  if (!image->data)
  {
    free(image);
    return CI_ERR_ALLOC;
  }

  // Set remaining values
  image->w = width;
  image->h = height;

  image->fmt = fmt;

  // Done
  *p_image = image;

  return CI_SUCCESS;
}

void ciImageDestroy(image_t image)
{
  if (image->data)
  {
    free(image->data);
    image->data = NULL;
  }
}


// Copies the first image to the second
ci_result_t ciImageCopy(image_t src, image_t* p_dst)
{
  // Create the image
  image_t image;

  ci_result_t r = ciImageCreate(&image, src->w, src->h, src->fmt);

  if (r != CI_SUCCESS)
    return r;

  // Copy the image data
  memcpy(image->data, src->data, ciImageGetSize(src));

  // Done
  *p_dst = image;

  return CI_SUCCESS;
}

ci_result_t ciImageScale(image_t image, size_t new_width, size_t new_height)
{
  // Allocate the new data
  uint8_t* new_data = (uint8_t*)malloc(new_width * new_height * ciColorFmtSize(image->fmt));

  if (!new_data)
    return CI_ERR_ALLOC;

  // Copy pixels from the old data to the new data
  uint8_t* old_data = (uint8_t*)image->data;

  size_t pixel_size = ciColorFmtSize(image->fmt);

  double x_skip = (double)image->w / (double)new_width;
  double y_skip = (double)image->h / (double)new_height;

  double src_x = 0.0;
  double src_y = 0.0;

  for (size_t y = 0; y < new_height; y++)
  {
    for (size_t x = 0; x < new_width; x++)
    {
      memcpy(&new_data[pixel_size * ((y * new_width) + x)], &old_data[pixel_size * CI_XY_TO_INDEX((size_t)src_x, (size_t)src_y, image->w)], pixel_size);

      src_x += x_skip;
    }

    src_x = 0.0;
    src_y += y_skip;
  }

  // Swap data and free the old data
  free(image->data);

  image->data = new_data;

  image->w = new_width;
  image->h = new_height;

  // Done
  return CI_SUCCESS;
}

size_t ciImageGetIndexXY(image_t image, size_t x, size_t y)
{
  return CI_XY_TO_INDEX(x, y, image->w);
}

size_t ciImageGetNumPixels(image_t image)
{
  return image->w * image->h;
}

size_t ciImageGetSize(image_t image)
{
  return image->w * image->h * ciColorFmtSize(image->fmt);
}

void ciImageBlit(image_t src, image_t dst, size_t x, size_t y)
{
  size_t pixel_size = ciColorFmtSize(src->fmt);
  size_t chunk_size = src->w * pixel_size;
  size_t jump = dst->w * pixel_size;

  // Pointers that data is being copied from and to
  uint8_t* from = src->data;
  uint8_t* to = (uint8_t*)dst->data + (CI_XY_TO_INDEX(x, y, dst->w) * pixel_size);

  for (size_t i = 0; i < src->h; i++)
  {
    memcpy(to, from, chunk_size);

    // Increment the to and from pointers
    from += chunk_size;
    to += jump;
  }
}

void ciImageFill(image_t image, void* p_color)
{
  uint8_t* ptr = (uint8_t*)image->data;

  size_t pixel_size = ciColorFmtSize(image->fmt);
  size_t num_pixels = image->w * image->h;

  for (size_t i = 0; i < num_pixels; i++)
  {
    memcpy(ptr, p_color, pixel_size);
    ptr += pixel_size;
  }
}

void ciImageFlipH(image_t image)
{
  uint8_t* data = (uint8_t*)image->data;

  size_t pixel_size = ciColorFmtSize(image->fmt);
  size_t half_w = image->w / 2;

  uint32_t temp; // Temporary value when swapping

  for (size_t y = 0; y < image->h; y++)
  {
    for (size_t x = 0; x < half_w; x++)
    {
      void* src = data + (CI_XY_TO_INDEX(x, y, image->w) * pixel_size);
      void* dst = data + (CI_XY_TO_INDEX(image->w - x, y, image->w) * pixel_size);

      memcpy(&temp, dst, pixel_size);

      memcpy(dst, src, pixel_size);
      memcpy(src, &temp, pixel_size);
    }
  }
}

void ciImageFlipV(image_t image)
{
  uint8_t* data = (uint8_t*)image->data;

  size_t pixel_size = ciColorFmtSize(image->fmt);
  size_t half_h = image->h / 2;

  uint32_t temp; // Temporary value when swapping

  for (size_t y = 0; y < half_h; y++)
  {
    for (size_t x = 0; x < image->w; x++)
    {
      void* src = data + (CI_XY_TO_INDEX(x, y, image->w) * pixel_size);
      void* dst = data + (CI_XY_TO_INDEX(x, image->h - y, image->w) * pixel_size);

      memcpy(&temp, dst, pixel_size);

      memcpy(dst, src, pixel_size);
      memcpy(src, &temp, pixel_size);
    }
  }
}

void ciImageForEach(image_t image, for_each_pixel_fn_t func, void* params)
{
  uint8_t* ptr = (uint8_t*)image->data;

  size_t pixel_size = ciColorFmtSize(image->fmt);
  size_t num_pixels = image->w * image->h;

  for (size_t i = 0; i < num_pixels; i++)
  {
    func((void*)ptr, params);
    ptr += pixel_size;
  }
}

void ciImageForEachI(image_t image, for_each_pixel_i_fn_t func, void* params)
{
  uint8_t* ptr = (uint8_t*)image->data;

  size_t pixel_size = ciColorFmtSize(image->fmt);
  size_t num_pixels = image->w * image->h;

  for (size_t i = 0; i < num_pixels; i++)
  {
    func((void*)ptr, i, params);
    ptr += pixel_size;
  }
}

void ciImageForEachXY(image_t image, for_each_pixel_xy_fn_t func, void* params)
{
  uint8_t* ptr = (uint8_t*)image->data;

  size_t pixel_size = ciColorFmtSize(image->fmt);

  for (size_t y = 0; y < image->h; y++)
  {
    for (size_t x = 0; x < image->w; x++)
    {
      func((void*)ptr, x, y, params);
      ptr += pixel_size;
    }
  }
}

void ciImageSnip(image_t src, image_t dst, size_t x, size_t y)
{
  size_t pixel_size = ciColorFmtSize(src->fmt);
  size_t chunk_size = dst->w * pixel_size;
  size_t jump = src->w * pixel_size;

  // Pointers that data is being copied from and to
  uint8_t* from = (uint8_t*)src->data + (CI_XY_TO_INDEX(x, y, src->w) * pixel_size);
  uint8_t* to = dst->data;

  for (size_t i = 0; i < dst->h; i++)
  {
    memcpy(to, from, chunk_size);

    // Increment the to and from pointers
    from += jump;
    to += chunk_size;
  }
}
