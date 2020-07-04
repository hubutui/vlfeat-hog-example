#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>

extern "C" {
#include <vl/pgm.h>
#include <vl/hog.h>
}
#define cimg_display 0

#include "CImg.h"

using namespace cimg_library;

int main(int argc, char *argv[]) {
    if (argc != 4) {
        printf("%s <input image> <feature file> <hog render image>\n", argv[0]);
        exit(1);
    }
    FILE *fp_out = nullptr;
    vl_size cell_size = 32;
    CImg<float> img(argv[1]);
    // img /= 255.0f;
    printf("min: %f\n", img.min());
    printf("max: %f\n", img.max());
    // create hog object
    VlHog *hog = vl_hog_new(VlHogVariantUoctti,
                            9,
                            VL_FALSE);
    // put image in hog object
    vl_hog_put_image(hog,
                     img.data(),
                     img.width(),
                     img.height(),
                     img.spectrum(),
                     cell_size);
    vl_size feature_size = vl_hog_get_dimension(hog)
                            * vl_hog_get_height(hog)
                            * vl_hog_get_width(hog);
    CImg<float> hog_array(feature_size, 1, 1, 1, 0.0f);
    // extract features
    vl_hog_extract(hog, hog_array.data());
    fp_out = fopen(argv[2], "wt");
    printf("writing hog feature to %s\n", argv[2]);
    if (fp_out == nullptr) {
        std::cout << "fail to open file for writing: " << argv[3] << std::endl;
        exit(EXIT_FAILURE);
    }
    for(vl_size i = 0; i < feature_size; i++) {
        fprintf(fp_out, "%f\n", hog_array(i));
    }
    fclose(fp_out);
    CImg<float> render_img(vl_hog_get_glyph_size(hog) * vl_hog_get_width(hog),
                           vl_hog_get_glyph_size(hog) * vl_hog_get_height(hog),
                           1,
                           1,
                           0.0f);
    vl_hog_render(hog,
                  render_img.data(),
                  hog_array.data(),
                  vl_hog_get_width(hog),
                  vl_hog_get_height(hog));

    printf("saving hog render image to: %s\n", argv[3]);
    vl_pgm_write_f(argv[3],
                   render_img.data(),
                   render_img.width(),
                   render_img.height());
    vl_hog_delete(hog);

    return EXIT_SUCCESS;
}