
#include "ci_media.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>


#define CI_BITMAP_HEAD    0x4D42    // "BM"


typedef struct image_t__
{
  void* data;       // A pointer to the image's data
  size_t w;         // The image's width
  size_t h;         // The image's height
  color_fmt_t fmt;  // The image's color format
}
image_t__;


// A struct representing a bitmap header
typedef struct __attribute__((packed))
{
  uint16_t head;          // The head of the bitmap (should  be "BM")
  uint32_t file_size;     // The size of the file in bytes
  uint32_t reserved;      // Reserved space; should be 0
  uint32_t data_offset;   // The offset of the pixel data
}
bitmap_head_t;

// A struct representing a bitmap info header
typedef struct __attribute__((packed))
{
  uint32_t size;          // The size of this header in bytes
  int32_t w;              // The width of the stored image
  int32_t h;              // The height of the stored image
  uint16_t num_planes;    // The number of planes; should be 1
  uint16_t pixel_size_b;  // The size of each pixel in bits
  uint32_t compression;   // Compression type
  uint32_t image_size;    // The size of the image data in bytes
  int32_t x_ppm;          // The image's horizontal resolution (pixels per meter)
  int32_t y_ppm;          // The image's vertical resolution (pixels per meter)
  uint32_t num_colors;    // The number of colors used in the bitmap
  uint32_t num_im_colors; // The number of important colors in the bitmap
}
bitmap_info_header_t;


// Returns the color format corresponding to the given pixel size and
static color_fmt_t ciMediaColorFmtFromBIH(uint16_t pixel_size_b)
{
  switch (pixel_size_b)
  {
    case 24:
      return CI_FMT_BGR;
    case 32:
      return CI_FMT_BGRA;
    default:
      return CI_FMT_NONE;
  }
}


ci_result_t ciMediaLoadBMP(image_t* p_image, const char* path)
{
  // Open the file
  FILE* f = fopen(path, "rb");

  if (!f)
    return CI_ERR_FILE;

  // Read the headers from the file
  bitmap_head_t fh;
  bitmap_info_header_t ih;

  if (fread(&fh, sizeof(bitmap_head_t), 1, f) != 1)
  {
    fclose(f);
    return CI_ERR_IN;

  }

  if (fh.head != CI_BITMAP_HEAD)
  {
    fclose(f);
    return CI_ERR_INVALID;
  }

  if (fread(&ih, sizeof(bitmap_info_header_t), 1, f) != 1)
  {
    fclose(f);
    return CI_ERR_IN;
  }

  // Get the color format of the bitmap
  color_fmt_t fmt = ciMediaColorFmtFromBIH(ih.pixel_size_b);

  if (fmt == CI_FMT_NONE)
  {
    fclose(f);
    return CI_ERR_INVALID;
  }

  // Allocate the image
  image_t__* image = (image_t__*)malloc(sizeof(image_t__));

  if (!image)
  {
    fclose(f);
    return CI_ERR_ALLOC;
  }

  // Allocate the image's data
  image->data = (void*)malloc(ih.image_size);

  if (!image->data)
  {
    free(image);
    fclose(f);
    return CI_ERR_ALLOC;
  }

  // Read the pixel data
  if (fread(image->data, 1, ih.image_size, f) != ih.image_size)
  {
    free(image->data);
    free(image);
    fclose(f);
    return CI_ERR_IN;
  }

  // Finish image initialization
  image->w = ih.w;
  image->h = ih.h;

  image->fmt = fmt;

  // Done
  fclose(f);

  *p_image = image;

  return CI_SUCCESS;
}

ci_result_t ciMediaSaveBMP(image_t image, const char* path)
{
  // Open the file
  FILE* f = fopen(path, "wb");

  if (!f)
    return CI_ERR_FILE;

  // Set up the headers
  size_t image_size = image->w * image->h * ciColorFmtSize(image->fmt);

  bitmap_head_t fh;

  fh.head = CI_BITMAP_HEAD;
  fh.file_size = sizeof(bitmap_head_t) + sizeof(bitmap_info_header_t) + image_size;
  fh.reserved = 0;
  fh.data_offset = sizeof(bitmap_head_t) + sizeof(bitmap_info_header_t);

  bitmap_info_header_t ih;

  ih.size = sizeof(bitmap_info_header_t);
  ih.w = image->w;
  ih.h = image->h;
  ih.num_planes = 1;
  ih.pixel_size_b = ciColorFmtSize(image->fmt) * 8;
  ih.compression = 0;
  ih.image_size = image_size;
  ih.x_ppm = 0;
  ih.y_ppm = 0;
  ih.num_colors = 0;
  ih.num_im_colors = 0;

  // Write the headers
  if (fwrite(&fh, sizeof(bitmap_head_t), 1, f) != 1)
  {
    fclose(f);
    return CI_ERR_OUT;
  }

  if (fwrite(&ih, sizeof(bitmap_info_header_t), 1, f) != 1)
  {
    fclose(f);
    return CI_ERR_OUT;
  }

  // Write the pixel data
  if (fwrite(image->data, 1, image_size, f) != image_size)
  {
    fclose(f);
    return CI_ERR_OUT;
  }

  fclose(f);

  // Done
  return CI_SUCCESS;
}
