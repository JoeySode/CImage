
#include "ci_color.h"


size_t ciColorFmtSize(color_fmt_t f)
{
  switch (f)
  {
    case CI_FMT_BGR:
    case CI_FMT_RGB:
      return 3;

    case CI_FMT_BGRA:
    case CI_FMT_RGBA:
      return 4;

    default:
      return 0;
  }
}

float ciChnlToFloat(chnl_t c)
{
  return (float)c / 255.0f;
}

chnl_t ciChnlFromFloat(float c)
{
  return (chnl_t)(c * 255.0f);
}
