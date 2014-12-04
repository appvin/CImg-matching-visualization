#include <iostream>
#include <string>
#include <vector>
#include <random>
#include <sstream>

#include <CImg.h>

#include "cimgMatchingViewer.hpp"

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

    int radius = 4;
    unsigned char colorPt[] = {255, 0, 0};
    unsigned char colorPt0[] = {255, 0, 0};
    unsigned char colorPt1[] = {0, 255, 0};
    unsigned char colorLine[] = {0, 0, 255};
    unsigned char colorBG[] = {255, 255, 255};
    unsigned char colorFG[] = {0, 0, 0};
    int x0, y0, x1, y1;
    for(int m = 0; m < numPointDraw; ++m)
    {
        x0 = points0(m,0);
        y0 = points0(m,1);
        x1 = points1(m,0)+offset;
        y1 = points1(m,1);
        draw_line_thick(img, x0, y0, x1, y1, colorLine, radius/2);
//        img.draw_circle(x0, y0, radius, colorPt, 1.f, 1);
//        img.draw_line(x1-radius/2, y1-radius/2, x1+radius/2, y1+radius/2, colorPt);
//        img.draw_line(x1+radius/2, y1-radius/2, x1-radius/2, y1+radius/2, colorPt);
        img.draw_circle(x0, y0, radius, colorPt, 1.f);
        img.draw_triangle(x1, y1-radius, x1-radius, y1+radius, x1+radius, y1+radius, colorPt);
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
    const int numPointDraw
)
{
    assert(
        img.spectrum() == 3 &&
        "The spectrum of the input image must be 3."
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
    int numPoint = ptX[0].size();
    assert(
        numPointDraw <= numPoint &&
        numPointDraw >= 0 &&
        "The number of drawn points must be [0, numPoint]."
    );

    cimg_library::CImg<int> points0(numPoint, 2);
    cimg_library::CImg<int> points1(numPoint, 2);
    for(int m = 0; m < numPoint; ++m)
    {
        points0(m,0) = ptX[0][m];
        points0(m,1) = ptY[0][m];
        points1(m,0) = ptX[1][m];
        points1(m,1) = ptY[1][m];
    }
    drawMatching(
        img,
        points0,
        points1,
        offset,
        energy,
        numPointDraw
    );
}

template <typename T>
void drawMatching(
    cimg_library::CImg<T>& img,
    const cimg_library::CImg<int>& points0,
    const cimg_library::CImg<int>& points1,
    const cimg_library::CImg<int>& correspondences,
    const std::vector<double> energy,
    const int offset,
    const int numCorrespondencesDraw
)
{
    assert(
        img.spectrum() == 3 &&
        "The spectrum of the input image must be 3."
    );
    assert(
        points0.height() == 2 &&
        points1.height() == 2 &&
        "The dimensionality of the point sets must be 2."
    );
    assert(
        correspondences.width() == energy.size() &&
        numCorrespondencesDraw <= correspondences.width() &&
        "Each point-to-point correspondences must be assigned its energy."
    );
    int numCorrespondences = correspondences.width();

    int radius = 4;
    unsigned char colorPt[] = {255, 0, 0};
    unsigned char colorLine[] = {0, 0, 255};
    unsigned char colorBG[] = {255, 255, 255};
    unsigned char colorFG[] = {0, 0, 0};
    int x0, y0, x1, y1;
    int i0, i1;
    for(int m = 0; m <= numCorrespondencesDraw; ++m)
    {
        i0 = correspondences(m,0);
        i1 = correspondences(m,1);
        if(i0>=0 && i0<points0.width() && i1>=0 && i1<points1.width())
        {
            x0 = points0(i0,0);
            y0 = points0(i0,1);
            x1 = points1(i1,0)+offset;
            y1 = points1(i1,1);
            draw_line_thick(img, x0, y0, x1, y1, colorLine, radius/2);
    //        img.draw_circle(x0, y0, radius, colorPt, 1.f, 1);
    //        img.draw_line(x1-radius/2, y1-radius/2, x1+radius/2, y1+radius/2, colorPt);
    //        img.draw_line(x1+radius/2, y1-radius/2, x1-radius/2, y1+radius/2, colorPt);
            img.draw_circle(x0, y0, radius, colorPt, 1.f);
            img.draw_triangle(x1, y1-radius, x1-radius, y1+radius, x1+radius, y1+radius, colorPt);
        }
    }

    int c0, c1;
    std::stringstream ss;
    ss << "correspondence#";
    if(numCorrespondencesDraw>=0)
    {
        c0 = correspondences(numCorrespondencesDraw,0);
        c1 = correspondences(numCorrespondencesDraw,1);
        ss << numCorrespondencesDraw << " = (";
        if(c0>=0)   ss << "p" << c0;
        else        ss << "-";
        ss << ",";
        if(c1>=0)   ss << "q" << c1;
        else        ss << "-";
        ss << ") = " << energy[numCorrespondencesDraw];
    }
    img.draw_text(0, 0, ss.str().c_str(), colorFG, colorBG, 1, 25);
}

int main(int argc, char* argv[])
{
    typedef unsigned char T;
    std::random_device rnd;
    std::mt19937 mt(rnd());

    std::cout << "run CImg matching result viewer..." << std::endl;
    std::vector<std::string> strFileInput;
    std::vector<int> width, height;
    cimg_library::CImgList<T> images;

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
            images.insert( getGrayscaledRGB( _img ) );
        }
        else
        {
            images.insert( getGraytoRGB( _img ) );
        }

        width.push_back( images(n).width() );
        height.push_back( images(n).height() );
    }

    /// synthesize a set of points on the images
    std::uniform_int_distribution<> randNumPoint(5, 20);
    std::vector<int> numPoints(2);
    cimg_library::CImgList<int> points(2);
    for(int n = 0; n < numImage; ++n)
    {
        numPoints[n] = randNumPoint(mt);
        points(n) = cimg_library::CImg<int>(numPoints[n], 2);
        std::uniform_int_distribution<> randX(0, width[n]-1);
        std::uniform_int_distribution<> randY(0, height[n]-1);
        for(int m = 0; m < numPoints[n]; ++m)
        {
            points(n)(m,0) = randX(mt);
            points(n)(m,1) = randY(mt);
        }
    }

    /// set point-to-point correspondences
    int numCorrespondences = numPoints[0];
    cimg_library::CImg<int> correspondences(numCorrespondences, 2);
    std::vector<double> energy(numCorrespondences);
    std::uniform_int_distribution<> rand1(-1, numPoints[1]-1);
    std::uniform_real_distribution<> randE(0.0, 1.0);
    for(int m = 0; m < numCorrespondences; ++m)
    {
        correspondences(m,0) = m;
        correspondences(m,1) = rand1(mt);
        energy[m] = randE(mt);
    }

    MatchingViewer<unsigned char, int> view;
    view.alpha(0.5);
    view.images(images(0), images(1));
    view.points(points(0), points(1));
    view.correspondences(correspondences);

    for(int n = 0; n < 2; ++n)
    {
        for(int m = 0; m < view.numberOfPoint(n); ++m)
        {
            std::cout << "pt[" << n << "][" << m << "] = (";
            std::cout << view.point(n)(m,0) << "," << view.point(n)(m,1) << ")" << std::endl;
        }
    }
    for(int c = 0; c < view.numberOfCorrespondences(); ++c)
    {
        int c0 = view.correspondences()(c,0);
        int c1 = view.correspondences()(c,1);
        std::cout << "correspondence[" << c << "] = (";
        if(c0>=0)   std::cout << "p" << c0;
        else        std::cout << "-";
        std::cout << ",";
        if(c1>=0)   std::cout << "q" << c1;
        else        std::cout << "-";
        std::cout << ") = " << energy[c] << std::endl;
    }
    /// draw the set of corresponding points
    bool dispUpdate = false;
    int numPointCur = 0, numPointPrev;
    cimg_library::CImg<T> imgMatch( view.imgAlign() );
    cimg_library::CImg<T> imgShow(imgMatch);
    drawMatching(imgShow, points(0), points(1), correspondences, energy, width[0], numPointCur);

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
            numPointCur = std::min(numCorrespondences-1,numPointCur);
            numPointCur = std::max(numPointCur, -1);
            if(numPointCur != numPointPrev)
            {
                imgShow = imgMatch;
                drawMatching(imgShow, points(0), points(1), correspondences, energy, width[0], numPointCur);
                imgShow.display(disp);
                dispUpdate = false;
                numPointPrev = numPointCur;
            }
        }
    }

    return 0;
}
