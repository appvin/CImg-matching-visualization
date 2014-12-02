#include <iostream>
#include <string>
#include <vector>
#include <random>
#include <sstream>

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

template <typename T>
void drawMatching(
    cimg_library::CImg<T>& img,
    const std::vector< std::vector<int> > ptX,
    const std::vector< std::vector<int> > ptY,
    const int offset,
    const std::vector<double> energy,
    const int numPoint,
    const int numPointDraw
)
{
    assert(
        img.spectrum() == 3 &&
        "The spectrum of the input image must be 3."
    );
    assert(
        numPointDraw <= numPoint &&
        numPointDraw >= 0 &&
        "The number of drawn points must be [0, numPoint]."
    );
    assert(
        ptX.size() == 2 &&
        ptY.size() == 2 &&
        ptX[0].size() == ptY[0].size() &&
        ptX[0].size() == ptX[1].size() &&
        ptY[0].size() == ptY[1].size() &&
        ptX[0].size() == energy.size() &&
        "The size of point sets must be 2."
    );

    int radius = 2;
    unsigned char colorPt0[] = {255, 0, 0};
    unsigned char colorPt1[] = {0, 255, 0};
    unsigned char colorLine[] = {0, 0, 255};
    unsigned char colorFG[] = {255, 255, 255};
    unsigned char colorBG[] = {0, 0, 0};
    int x0, y0, x1, y1;
    for(int m = 0; m <= numPointDraw; ++m)
    {
        x0 = ptX[0][m];
        y0 = ptY[0][m];
        x1 = ptX[1][m]+offset;
        y1 = ptY[1][m];
        draw_line_thick(img, x0, y0, x1, y1, colorLine, radius);
        img.draw_circle(x0, y0, radius, colorPt0);
        img.draw_circle(x1, y1, radius, colorPt1);
    }

    std::stringstream ss;
    ss << numPointDraw+1 << "/" << numPoint << ": Energy = " << energy[numPointDraw];
    img.draw_text(0, 0, ss.str().c_str(), colorFG, colorBG, 1, 25);
}

int main(int argc, char* argv[])
{
    typedef unsigned char T;
    std::cout << "run CImg matching result viewer..." << std::endl;
    std::vector<std::string> strFileInput;
    std::vector<int> width, height;
    cimg_library::CImgList<T> listImg;

    /// set input images
    int numImage = 2;
    if(argc > numImage)
    {
        for(int n = 1; n <= numImage; ++n)
        {
            strFileInput.push_back(argv[n]);
        }
    }
    else
    {
        strFileInput.push_back("img1.ppm");
        strFileInput.push_back("img2.ppm");
    }
    for(auto it = strFileInput.begin(); it != strFileInput.end(); ++it)
    {
        int n = it - strFileInput.begin();
        std::cout << "str[" << n << "] = " << *it << std::endl;
        cimg_library::CImg<T> _img(it->c_str());
        if(_img.spectrum() == 3)
        {
            listImg.insert( getGrayscaledRGB( _img ) );
        }
        else
        {
            listImg.insert( getGraytoRGB( _img ) );
        }

        width.push_back( listImg(n).width() );
        height.push_back( listImg(n).height() );
    }

    /// synthesize a set of corresponding points
    int numPoint = 10;
    std::vector< std::vector<int> > ptX(2, std::vector<int>(numPoint));
    std::vector< std::vector<int> > ptY(2, std::vector<int>(numPoint));
    std::vector<double> energy(numPoint);
    std::random_device rnd;
    std::mt19937 mt(rnd());
    std::uniform_real_distribution<> randE(0.0, 1.0);
    for(int n = 0; n < numImage; ++n)
    {
        std::uniform_int_distribution<> randX(0, width[n]-1);
        std::uniform_int_distribution<> randY(0, height[n]-1);
        for(int m = 0; m < numPoint; ++m)
        {
            ptX[n][m] = randX(mt);
            ptY[n][m] = randY(mt);
            if(n==0)    energy[m] = randE(mt);
        }
    }

    /// draw the set of corresponding points
    bool dispUpdate = false;
    int numPointCur = 0, numPointPrev;
    cimg_library::CImg<T> imgMatch( listImg.get_append('x') );
    cimg_library::CImg<T> imgShow(imgMatch);
    drawMatching(imgShow, ptX, ptY, width[0], energy, numPoint, numPointCur);

    cimg_library::CImgDisplay disp(imgShow, "Matching result");
    while(!disp.is_closed() && !disp.key())
    {
        // check any user input
        disp.wait();
        // retrieve the input
        if(disp.wheel()!=0)
        {
            numPointCur += disp.wheel();
            disp.set_wheel();
            dispUpdate = true;
        }
        if(disp.is_keyARROWDOWN() || disp.is_keyARROWLEFT())
        {
            --numPointCur;
            dispUpdate = true;
        }
        if(disp.is_keyARROWUP() || disp.is_keyARROWRIGHT())
        {
            ++numPointCur;
            dispUpdate = true;
        }

        // update the image
        if(dispUpdate)
        {
            numPointCur = std::min(numPoint-1,numPointCur);
            numPointCur = std::max(numPointCur, 0);
            if(numPointCur != numPointPrev)
            {
                imgShow = imgMatch;
                drawMatching(imgShow, ptX, ptY, width[0], energy, numPoint, numPointCur);
                imgShow.display(disp);
                dispUpdate = false;
                numPointPrev = numPointCur;
            }
        }
    }

    return 0;
}
