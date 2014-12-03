#include <iostream>
#include <string>
#include <vector>
#include <random>
#include <sstream>

#include <CImg.h>

#include "cimgConvertColor.hpp"
#include "cimgDrawLineThick.hpp"


template <typename T>
void drawMatching(
    cimg_library::CImg<T>& img,
    const cimg_library::CImg<int>& points0,
    const cimg_library::CImg<int>& points1,
    const int offset,
    const std::vector<double> energy,
    const int numPointDraw
)
{
    assert(
        img.spectrum() == 3 &&
        "The spectrum of the input image must be 3."
    );
    assert(
        points0.width() == points1.width() &&
        points0.width() == energy.size() &&
        points0.width() > 0 &&
        points0.height() == 2 &&
        points1.height() == 2 &&
        "The size of point sets must be 2."
    );
    int numPoint = points0.width();
    assert(
        numPointDraw <= numPoint &&
        numPointDraw >= 0 &&
        "The number of drawn points must be [0, numPoint]."
    );

    int radius = 2;
    unsigned char colorPt0[] = {255, 0, 0};
    unsigned char colorPt1[] = {0, 255, 0};
    unsigned char colorLine[] = {0, 0, 255};
    unsigned char colorBG[] = {255, 255, 255};
    unsigned char colorFG[] = {0, 0, 0};
    for(int m = 0; m < numPointDraw; ++m)
    {
        draw_line_thick(img, points0(m,0), points0(m,1), points1(m,0)+offset, points1(m,1), colorLine, radius);
        img.draw_circle(points0(m,0), points0(m,1), radius*2, colorPt0);
        img.draw_circle(points1(m,0)+offset, points1(m,1), radius*2, colorPt1);
    }

    std::stringstream ss;
    ss << numPointDraw << "/" << numPoint << " points: Energy = ";
    if(numPointDraw!=0)
    {
        ss << energy[numPointDraw-1];
    }
    else
    {
        ss << "0.0";
    }
    img.draw_text(0, 0, ss.str().c_str(), colorFG, colorBG, 1, 25);
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
    unsigned char colorBG[] = {255, 255, 255};
    unsigned char colorFG[] = {0, 0, 0};
    cimg_library::CImg<int> points(2,2);
    for(int m = 0; m < numPointDraw; ++m)
    {
        points(0,0) = ptX[0][m];
        points(0,1) = ptY[0][m];
        points(1,0) = ptX[1][m]+offset;
        points(1,1) = ptY[1][m];
        draw_line_thick(img, points, colorLine, radius);
        img.draw_circle(points(0,0), points(0,1), radius, colorPt0);
        img.draw_circle(points(1,0), points(1,1), radius, colorPt1);
    }

    std::stringstream ss;
    if(numPointDraw!=0)
    {
        ss << numPointDraw << "/" << numPoint << " points: Energy = " << energy[numPointDraw-1];
    }
    else
    {
        ss << numPointDraw << "/" << numPoint << " points: Energy = 0.0";
    }
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
    cimg_library::CImg<int> points0(numPoint, 2);
    cimg_library::CImg<int> points1(numPoint, 2);
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
            if(n==0)
            {
                points0(m,0) = randX(mt);
                points0(m,1) = randY(mt);
            }
            else
            {
                points1(m,0) = randX(mt);
                points1(m,1) = randY(mt);
            }
            if(n==0)    energy[m] = randE(mt);
        }
    }

    /// draw the set of corresponding points
    bool dispUpdate = false;
    int numPointCur = 0, numPointPrev;
    cimg_library::CImg<T> imgMatch( listImg.get_append('x') );
    cimg_library::CImg<T> imgShow(imgMatch);
    drawMatching(imgShow, points0, points1, width[0], energy, numPointCur);

    cimg_library::CImgDisplay disp(imgShow, "Matching result");
    int dispSizeWidth = disp.width();
    int dispSizeHeight = disp.height();
    int screenSizeWidth = disp.screen_width();
    int screenSizeHeight = disp.screen_height();
    float scaleWidth = screenSizeWidth/(float)dispSizeWidth;
    float scaleHeight = screenSizeHeight/(float)dispSizeHeight;
    int dispSizeWidthMax = dispSizeWidth*std::min(scaleWidth,scaleHeight);
    int dispSizeHeightMax = dispSizeHeight*std::min(scaleWidth,scaleHeight);

    while(!disp.is_closed())
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
        if(disp.is_keyF())
        {
            if(disp.is_fullscreen())
            {
                disp.resize(dispSizeWidth,dispSizeHeight);
            }
            else
            {
                disp.resize(dispSizeWidthMax, dispSizeHeightMax);
            }
            disp.toggle_fullscreen();
        }
        if(disp.is_keyQ() || disp.is_keyESC())
        {
            disp.close();
        }

        // update the image
        if(dispUpdate)
        {
            numPointCur = std::min(numPoint,numPointCur);
            numPointCur = std::max(numPointCur, 0);
            if(numPointCur != numPointPrev)
            {
                imgShow = imgMatch;
                drawMatching(imgShow, points0, points1, width[0], energy, numPointCur);
                imgShow.display(disp);
                dispUpdate = false;
                numPointPrev = numPointCur;
            }
        }
    }

    return 0;
}
