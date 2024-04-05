
#ifndef CI_COLOR_H_

#define CI_COLOR_H_


#include <stddef.h>
#include <stdint.h>


// A color channel
typedef uint8_t chnl_t;

// A BGR color whose colors can be accessed by channel (foo.r, foo.g, foo.b)
typedef struct
{
  chnl_t b;
  chnl_t g;
  chnl_t r;
}
bgr_t;

// An RGB color whose colors can be accessed by channel (foo.r, foo.g, foo.b)
typedef struct
{
  chnl_t r;
  chnl_t g;
  chnl_t b;
}
rgb_t;

// A BGRA color whose colors can be accessed directly (foo.c) or by channel (foo.r, foo.g etc.)
typedef union 
{
  struct
  {
    chnl_t b;
    chnl_t g;
    chnl_t r;
    chnl_t a;
  };
  uint32_t c;
}
bgra_t;

// An RGBA color whose colors can be accessed directly (foo.c) or by channel (foo.r, foo.g etc.)
typedef union 
{
  struct
  {
    chnl_t r;
    chnl_t g;
    chnl_t b;
    chnl_t a;
  };
  uint32_t c;
}
rgba_t;

// An enum representing different color formats
typedef enum
{
  CI_FMT_NONE,  // Undefines/unknown format

  CI_FMT_BGR,   // Blue green red
  CI_FMT_RGB,   // Red green blue
  CI_FMT_BGRA,  // Blue green red alpha
  CI_FMT_RGBA,  // Red green blue alpha
}
color_fmt_t;


// The size of the color format in bytes (returns 0 if invalid)
size_t ciColorFmtSize(color_fmt_t f);

// The color channel as a floating point value (0-1)
float ciChnlToFloat(chnl_t c);

// The float as a color channel
chnl_t ciChnlFromFloat(float c);


#endif // CI_COLOR_H_
