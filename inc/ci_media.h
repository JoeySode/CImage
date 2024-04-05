
#ifndef CI_MEDIA_H_

#define CI_MEDIA_H_


#include "ci_image.h"
#include "ci_result.h"


// Loads the bitmap file and initializes the image with its data
ci_result_t ciMediaLoadBMP(image_t* p_image, const char* path);

// Saves the image to the given path as a bitmap file
ci_result_t ciMediaSaveBMP(image_t* p_image, const char* path);


#endif // CI_MEDIA_H_
