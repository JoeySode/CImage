
#include "ci_transform.h"

#include <stdlib.h>
#include <string.h>


// Converts the x and y to an index for a 1D array
#define CI_XY_TO_INDEX(x, y, w) (((y) * (w)) + (x))


ci_result_t ciTransformScale(CI_Image* dst, CI_Image* src, size_t width, size_t height)
{
  // Allocate the new data
  uint8_t* dst_data = (uint8_t*)malloc(width * height * ciColorFmtSize(src->_fmt));

  if (!dst_data)
    return CI_ERR_ALLOC;

  // Copy pixels from the old data to the new data
  uint8_t* src_data = (uint8_t*)src->_data;

  size_t pixel_size = ciColorFmtSize(src->_fmt);

  double x_skip = (double)src->_w / (double)width;
  double y_skip = (double)src->_h / (double)height;

  double src_x = 0.0;
  double src_y = 0.0;

  for (size_t y = 0; y < height; y++)
  {
    for (size_t x = 0; x < width; x++)
    {
      memcpy(&dst_data[pixel_size * ((y * width) + x)], &src_data[pixel_size * CI_XY_TO_INDEX((size_t)src_x, (size_t)src_y, src->_w)], pixel_size);

      src_x += x_skip;
    }

    src_x = 0.0;
    src_y += y_skip;
  }

  // Swap dst data, freeing if needed
  if (dst->_data)
  {
    free(dst->_data);
  }

  dst->_data = (void*)dst_data;

  dst->_w = width;
  dst->_h = height;

  dst->_fmt = src->_fmt;

  // Done
  return CI_SUCCESS;
}

void ciTransformBlit(CI_Image* dst, CI_Image* src, size_t x, size_t y)
{
  size_t pixel_size = ciColorFmtSize(src->_fmt);
  size_t chunk_size = src->_w * pixel_size;
  size_t jump = dst->_w * pixel_size;

  // Pointers that data is being copied from and to
  uint8_t* from = src->_data;
  uint8_t* to = (uint8_t*)dst->_data + (CI_XY_TO_INDEX(x, y, dst->_w) * pixel_size);

  for (size_t i = 0; i < src->_h; i++)
  {
    memcpy(to, from, chunk_size);

    // Increment the to and from pointers
    from += chunk_size;
    to += jump;
  }
}

void ciTransformFill(CI_Image* image, void* p_color)
{
  uint8_t* ptr = (uint8_t*)image->_data;

  size_t pixel_size = ciColorFmtSize(image->_fmt);
  size_t num_pixels = image->_w * image->_h;

  for (size_t i = 0; i < num_pixels; i++)
  {
    memcpy(ptr, p_color, pixel_size);
    ptr += pixel_size;
  }
}

void ciTransformFlipH(CI_Image* image)
{
  uint8_t* data = (uint8_t*)image->_data;

  size_t pixel_size = ciColorFmtSize(image->_fmt);
  size_t half_w = image->_w / 2;

  uint32_t temp; // Temporary value when swapping

  for (size_t y = 0; y < image->_h; y++)
  {
    for (size_t x = 0; x < half_w; x++)
    {
      void* src = data + (CI_XY_TO_INDEX(x, y, image->_w) * pixel_size);
      void* dst = data + (CI_XY_TO_INDEX(image->_w - x, y, image->_w) * pixel_size);

      memcpy(&temp, dst, pixel_size);

      memcpy(dst, src, pixel_size);
      memcpy(src, &temp, pixel_size);
    }
  }
}

void ciTransformFlipV(CI_Image* image)
{
  uint8_t* data = (uint8_t*)image->_data;

  size_t pixel_size = ciColorFmtSize(image->_fmt);
  size_t half_h = image->_h / 2;

  uint32_t temp; // Temporary value when swapping

  for (size_t y = 0; y < half_h; y++)
  {
    for (size_t x = 0; x < image->_w; x++)
    {
      void* src = data + (CI_XY_TO_INDEX(x, y, image->_w) * pixel_size);
      void* dst = data + (CI_XY_TO_INDEX(x, image->_h - y, image->_w) * pixel_size);

      memcpy(&temp, dst, pixel_size);

      memcpy(dst, src, pixel_size);
      memcpy(src, &temp, pixel_size);
    }
  }
}

void ciTransformSnip(CI_Image* dst, CI_Image* src, size_t x, size_t y)
{
  size_t pixel_size = ciColorFmtSize(src->_fmt);
  size_t chunk_size = dst->_w * pixel_size;
  size_t jump = src->_w * pixel_size;

  // Pointers that data is being copied from and to
  uint8_t* from = (uint8_t*)src->_data + (CI_XY_TO_INDEX(x, y, src->_w) * pixel_size);
  uint8_t* to = dst->_data;

  for (size_t i = 0; i < dst->_h; i++)
  {
    memcpy(to, from, chunk_size);

    // Increment the to and from pointers
    from += jump;
    to += chunk_size;
  }
}
