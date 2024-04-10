


#include "ci_image.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>


ci_result_t ciInitImage(image_t* p_image, size_t width, size_t height, color_fmt_t fmt)
{
  // Allocate the data
  p_image->data = (void*)calloc(width * height, ciColorFmtSize(fmt));

  if (!p_image->data)
  {
    free(p_image);
    return CI_ERR_ALLOC;
  }

  // Set remaining values
  p_image->w = width;
  p_image->h = height;

  p_image->fmt = fmt;

  // Done
  return CI_SUCCESS;
}

void ciDestroyImage(image_t* p_image)
{
  if (p_image->data)
  {
    free(p_image->data);
    p_image->data = NULL;
  }
}


// Copies the first image to the second
ci_result_t ciImageCopy(image_t* src, image_t* dst)
{
  size_t image_size = ciImageSize(src);

  // Allocate the new data
  dst->data = (void*)malloc(image_size);

  if (!dst->data)
    return CI_ERR_ALLOC;

  // Copy image data
  memcpy(dst->data, src->data, image_size);

  // Copy remaining data
  dst->w = src->w;
  dst->h = src->h;

  dst->fmt = src->fmt;

  // Done
  return CI_SUCCESS;
}

ci_result_t ciImageScale(image_t* p_image, size_t new_width, size_t new_height)
{
  // Allocate the new data
  uint8_t* new_data = (uint8_t*)malloc(new_width * new_height * ciColorFmtSize(p_image->fmt));

  if (!new_data)
    return CI_ERR_ALLOC;

  // Copy pixels from the old data to the new data
  uint8_t* old_data = (uint8_t*)p_image->data;
  size_t pixel_size = ciColorFmtSize(p_image->fmt);
  double x_skip = (double)p_image->w / (double)new_width;
  double y_skip = (double)p_image->h / (double)new_height;
  double src_x = 0.0;
  double src_y = 0.0;

  for (size_t y = 0; y < new_height; y++)
  {
    for (size_t x = 0; x < new_width; x++)
    {
      memcpy(&new_data[pixel_size * ((y * new_width) + x)], &old_data[pixel_size * ciImageIndexFromXY(p_image, (size_t)src_x, (size_t)src_y)], pixel_size);

      src_x += x_skip;
    }

    src_x = 0.0;
    src_y += y_skip;
  }

  // Swap data and free the old data
  free(p_image->data);

  p_image->data = new_data;

  p_image->w = new_width;
  p_image->h = new_height;

  // Done
  return CI_SUCCESS;
}

size_t ciImageIndexFromXY(image_t* p_image, size_t x, size_t y)
{
  return (y * p_image->w) + x;
}

size_t ciImageNumPixels(image_t* p_image)
{
  return p_image->w * p_image->h;
}

size_t ciImageSize(image_t* p_image)
{
  return p_image->w * p_image->h * ciColorFmtSize(p_image->fmt);
}

void ciImageBlit(image_t* src, image_t* dst, size_t x, size_t y)
{
  size_t pixel_size = ciColorFmtSize(src->fmt);
  size_t chunk_size = src->w * pixel_size;
  size_t jump = dst->w * pixel_size;

  // Pointers that data is being copied from and to
  uint8_t* from = src->data;
  uint8_t* to = (uint8_t*)dst->data + (ciImageIndexFromXY(dst, x, y) * pixel_size);

  for (size_t i = 0; i < src->h; i++)
  {
    memcpy(to, from, chunk_size);

    // Increment the to and from pointers
    from += chunk_size;
    to += jump;
  }
}

void ciImageFill(image_t* p_image, void* p_color)
{
  uint8_t* ptr = (uint8_t*)p_image->data;
  size_t pixel_size = ciColorFmtSize(p_image->fmt);
  size_t num_pixels = p_image->w * p_image->h;

  for (size_t i = 0; i < num_pixels; i++)
  {
    memcpy(ptr, p_color, pixel_size);
    ptr += pixel_size;
  }
}

void ciImageForEach(image_t* p_image, for_each_pixel_fn_t func, void* params)
{
  uint8_t* ptr = (uint8_t*)p_image->data;
  size_t pixel_size = ciColorFmtSize(p_image->fmt);
  size_t num_pixels = p_image->w * p_image->h;

  for (size_t i = 0; i < num_pixels; i++)
  {
    func((void*)ptr, params);
    ptr += pixel_size;
  }
}

void ciImageForEachI(image_t* p_image, for_each_pixel_i_fn_t func, void* params)
{
  uint8_t* ptr = (uint8_t*)p_image->data;
  size_t pixel_size = ciColorFmtSize(p_image->fmt);
  size_t num_pixels = p_image->w * p_image->h;

  for (size_t i = 0; i < num_pixels; i++)
  {
    func((void*)ptr, i, params);
    ptr += pixel_size;
  }
}

void ciImageForEachXY(image_t* p_image, for_each_pixel_xy_fn_t func, void* params)
{
  uint8_t* ptr = (uint8_t*)p_image->data;
  size_t pixel_size = ciColorFmtSize(p_image->fmt);

  for (size_t y = 0; y < p_image->h; y++)
  {
    for (size_t x = 0; x < p_image->w; x++)
    {
      func((void*)ptr, x, y, params);
      ptr += pixel_size;
    }
  }
}

void ciImageSnip(image_t* src, image_t* dst, size_t x, size_t y)
{
  size_t pixel_size = ciColorFmtSize(src->fmt);
  size_t chunk_size = dst->w * pixel_size;
  size_t jump = src->w * pixel_size;

  // Pointers that data is being copied from and to
  uint8_t* from = (uint8_t*)src->data + (ciImageIndexFromXY(src, x, y) * pixel_size);
  uint8_t* to = dst->data;

  for (size_t i = 0; i < dst->h; i++)
  {
    memcpy(to, from, chunk_size);

    // Increment the to and from pointers
    from += jump;
    to += chunk_size;
  }
}
