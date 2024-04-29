


#include "ci_image.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>


ci_result_t ciImageCreate(CI_Image* image, size_t width, size_t height, color_fmt_t fmt)
{
  // Allocate the data
  image->_data = (void*)calloc(width * height, ciColorFmtSize(fmt));

  if (!image->_data)
  {
    free(image);
    return CI_ERR_ALLOC;
  }

  // Set remaining values
  image->_w = width;
  image->_h = height;

  image->_fmt = fmt;

  // Done
  return CI_SUCCESS;
}

void ciImageDestroy(CI_Image* image)
{
  // Free data if any
  if (image->_data)
  {
    free(image->_data);
  }

  // Uninitialize data
  image->_data = NULL;

  image->_w = 0;
  image->_h = 0;

  image->_fmt = CI_FMT_NONE;
}


// Copies the first image to the second
ci_result_t ciImageCopy(CI_Image* dst, CI_Image* src)
{
  size_t data_size = src->_w * src->_h * ciColorFmtSize(src->_fmt);

  // Allocate the data
  dst->_data = (void*)malloc(data_size);

  if (!dst->_data)
  {
    return CI_ERR_ALLOC;
  }

  // Copy data
  memcpy(dst->_data, src->_data, data_size);

  dst->_w = src->_w;
  dst->_h = src->_h;

  dst->_fmt = src->_fmt;

  // Done
  return CI_SUCCESS;
}

size_t ciImageGetIndexXY(CI_Image* image, size_t x, size_t y)
{
  return (y * image->_w) + x;
}

size_t ciImageGetNumPixels(CI_Image* image)
{
  return image->_w * image->_h;
}

size_t ciImageGetSize(CI_Image* image)
{
  return image->_w * image->_h * ciColorFmtSize(image->_fmt);
}
