#ifndef cimgConvertColor
#define cimgConvertColor

#include <CImg.h>

template <typename T>
cimg_library::CImg<T> getRGBtoGray(
    const cimg_library::CImg<T>& _img
)
{
    assert(
        _img.spectrum() == 3 &&
        "The spectrum of the input image must be 3."
    );
    return _img.get_RGBtoYCbCr().get_channel(0);
}

template <typename T>
cimg_library::CImg<T> getGraytoRGB(
    const cimg_library::CImg<T>& _img
)
{
    assert(
        _img.spectrum() == 1 &&
        "The spectrum of the input image must be 1."
    );
    cimg_library::CImg<T> img(_img.width(), _img.height(), 1, 3, 0);
    for(int c = 0; c < 3; ++c)
    {
        img.get_shared_channel(c).assign( _img );
    }
    return img;
}

template <typename T>
cimg_library::CImg<T> getGrayscaledRGB(
    const cimg_library::CImg<T>& _img
)
{
    assert(
        _img.spectrum() == 3 &&
        "The spectrum of the input image must be 3."
    );
    return getGraytoRGB( getRGBtoGray(_img) );
}

#endif
