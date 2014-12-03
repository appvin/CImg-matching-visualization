#ifndef cimgDrawLineThick
#define cimgDrawLineThick

#include <CImg.h>

template <typename T>
void draw_line_thick(
    cimg_library::CImg<T>& img,
        const int x0,
        const int y0,
        const int x1,
        const int y1,
        const T color[],
        const int radius = 0,
        const float opacity = 1.f
)
{
    assert(
        img.spectrum() == 3 &&
        "The spectrum of the input image must be 3."
    );
    img.draw_line(x0, y0, x1, y1, color);
    if(radius)
    {
        img.draw_line(x0-radius, y0, x1-radius, y1, color).draw_line(x0+radius, y0, x1+radius, y1, color).
        draw_line(x0, y0-radius, x1, y1-radius, color).draw_line(x0, y0+radius, x1, y1+radius, color);
        for (int f = 1-radius, ddFx = 0, ddFy = -(radius<<1), x = 0, y = radius; x<y; ) {
          if (f>=0) { f+=(ddFy+=2); --y; }
          ++x; ++(f+=(ddFx+=2));
          if (x!=y+1) {
              const int x01 = x0-y, x02 = x0+y, y01 = y0-x, y02 = y0+x, x03 = x0-x, x04 = x0+x, y03 = y0-y, y04 = y0+y;
              const int x11 = x1-y, x12 = x1+y, y11 = y1-x, y12 = y1+x, x13 = x1-x, x14 = x1+x, y13 = y1-y, y14 = y1+y;
              img.draw_line(x01,y01, x11,y11,color,opacity).draw_line(x01,y02, x11,y12, color,opacity).
                draw_line(x02,y01, x12,y11, color,opacity).draw_line(x02,y02, x12,y12, color,opacity);
              if (x!=y)
                img.draw_line(x03,y03, x13,y13, color,opacity).draw_line(x04,y04, x14,y14,color,opacity).
                  draw_line(x04,y03, x14,y13, color,opacity).draw_line(x03,y04, x13,y14, color,opacity);
          }
        }
    }
}

template <typename T, typename TP>
void draw_line_thick(
    cimg_library::CImg<T>& img,
    cimg_library::CImg<TP>& points,
    const T color[],
    const int radius = 0,
    const float opacity = 1.f
)
{
    assert(
        img.spectrum() == 3 &&
        "The spectrum of the input image must be 3."
    );
    assert(
        points.width() == 2 &&
        points.height() == 2 &&
        "The number of input points must be 2."
    );
    const int x0 = (int)points(0,0), y0 = (int)points(0,1);
    const int x1 = (int)points(1,0), y1 = (int)points(1,1);
    draw_line_thick(img, x0, y0, x1, y1, color, radius, opacity);
}

#endif
