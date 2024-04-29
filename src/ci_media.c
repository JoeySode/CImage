
#include "ci_media.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>


#define CI_bmp_file_header    0x4D42    // "BM"


// A struct representing a bitmap header
typedef struct __attribute__((packed))
{
  uint16_t head;          // The head of the bitmap (should  be "BM")
  uint32_t file_size;     // The size of the file in bytes
  uint32_t reserved;      // Reserved space; should be 0
  uint32_t data_offset;   // The offset of the pixel data
}
bmp_file_header_t;

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
bmp_info_header_t;


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


ci_result_t ciMediaLoadBMP(CI_Image* image, const char* path)
{
  // Open the file
  FILE* f = fopen(path, "rb");

  if (!f)
    return CI_ERR_FILE;

  // Read the headers from the file
  bmp_file_header_t fh;
  bmp_info_header_t ih;

  if (fread(&fh, sizeof(bmp_file_header_t), 1, f) != 1)
  {
    fclose(f);
    return CI_ERR_IN;

  }

  if (fh.head != CI_bmp_file_header)
  {
    fclose(f);
    return CI_ERR_INVALID;
  }

  if (fread(&ih, sizeof(bmp_info_header_t), 1, f) != 1)
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

  // Allocate the image's data
  image->_data = (void*)malloc(ih.image_size);

  if (!image->_data)
  {
    free(image);
    fclose(f);
    return CI_ERR_ALLOC;
  }

  // Read the pixel data
  if (fread(image->_data, 1, ih.image_size, f) != ih.image_size)
  {
    free(image->_data);
    free(image);
    fclose(f);
    return CI_ERR_IN;
  }

  // Finish image initialization
  image->_w = ih.w;
  image->_h = ih.h;

  image->_fmt = fmt;

  // Done
  fclose(f);

  return CI_SUCCESS;
}

ci_result_t ciMediaSaveBMP(CI_Image* image, const char* path)
{
  // Open the file
  FILE* f = fopen(path, "wb");

  if (!f)
    return CI_ERR_FILE;

  // Set up the headers
  size_t image_size = image->_w * image->_h * ciColorFmtSize(image->_fmt);

  bmp_file_header_t fh = (bmp_file_header_t)
  {
    .head = CI_bmp_file_header,
    .file_size = sizeof(bmp_file_header_t) + sizeof(bmp_info_header_t) + image_size,
    .reserved = 0,
    .data_offset = sizeof(bmp_file_header_t) + sizeof(bmp_info_header_t),
  };

  bmp_info_header_t ih = (bmp_info_header_t)
  {
    .size = sizeof(bmp_info_header_t),
    .w = image->_w,
    .h = image->_h,
    .num_planes = 1,
    .pixel_size_b = ciColorFmtSize(image->_fmt) * 8,
    .compression = 0,
    .image_size = image_size,
    .x_ppm = 0,
    .y_ppm = 0,
    .num_colors = 0,
    .num_im_colors = 0,
  };

  // Write the headers
  if (fwrite(&fh, sizeof(bmp_file_header_t), 1, f) != 1)
  {
    fclose(f);
    return CI_ERR_OUT;
  }

  if (fwrite(&ih, sizeof(bmp_info_header_t), 1, f) != 1)
  {
    fclose(f);
    return CI_ERR_OUT;
  }

  // Write the pixel data
  if (fwrite(image->_data, 1, image_size, f) != image_size)
  {
    fclose(f);
    return CI_ERR_OUT;
  }

  // Done
  fclose(f);

  return CI_SUCCESS;
}
