
#include "ci_color.h"


size_t ciColorFmtSize(color_fmt_t f)
{
  return f & 0x0F;
}
