
#ifndef CI_COLOR_H_
#define CI_COLOR_H_


#include <stddef.h>
#include <stdint.h>


// A BGR color whose colors can be accessed by channel (foo.r, foo.g, foo.b)
typedef struct bgr_t
{
  uint8_t b;
  uint8_t g;
  uint8_t r;
}
bgr_t;

// An RGB color whose colors can be accessed by channel (foo.r, foo.g, foo.b)
typedef struct rgb_t
{
  uint8_t r;
  uint8_t g;
  uint8_t b;
}
rgb_t;

// A BGRA color whose colors can be accessed directly (foo.c) or by channel (foo.r, foo.g etc.)
typedef union bgra_t
{
  struct
  {
    uint8_t b;
    uint8_t g;
    uint8_t r;
    uint8_t a;
  };
  uint32_t c;
}
bgra_t;

// An RGBA color whose colors can be accessed directly (foo.c) or by channel (foo.r, foo.g etc.)
typedef union rgba_t
{
  struct
  {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
  };
  uint32_t c;
}
rgba_t;

// An enum representing different color formats
typedef enum color_fmt_t
{
  CI_FMT_NONE = 0x00, // Undefines/unknown format

  CI_FMT_BGR  = 0x13, // Blue green red
  CI_FMT_RGB  = 0x23, // Red green blue
  CI_FMT_BGRA = 0x14, // Blue green red alpha
  CI_FMT_RGBA = 0x24, // Red green blue alpha
}
color_fmt_t;


// The size of the color format in bytes (returns 0 if invalid)
size_t ciColorFmtSize(color_fmt_t f);


#endif // CI_COLOR_H_
