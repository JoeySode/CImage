# CImage

CImage is a simple lightweight C image library that allows you to create and modify images, as well as letting you read and save bitmap files.

NOTE: CImage's coordinate system is (0, 0) being the bottom left pixel

## Instalation

Enter the CCImageGeo directory and run 'make'. This will (hopefully) compile the object files and the final library file 'libcimage.a' into './build'.
To clean, run 'make clean' which will remove the build folder entirely.

## Usage

```
image_t image;
ciMediaLoadBMP(&image, "example.bmp");

bgra_t* pixels = (bgra_t*)image.data;
for (size_t i = 0; i < image.w * image.h)
    pixels[i].r = 255;

ciMediaSaveBMP(&image, "result.bmp");
ciDestroyImage(&image);
```

## Contributing

Pull requests are welcome and appreciated.

## License

[MIT](https://choosealicense.com/licenses/mit/)