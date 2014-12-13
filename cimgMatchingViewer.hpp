#ifndef cimgMatchingViewer
#define cimgMatchingViewer

#include <string>
#include "cimgConvertColor.hpp"
#include "cimgDrawLineThick.hpp"
#include <CImg.h>

template <typename TI, typename TP>
class MatchingViewer
{
    //------------------------------------------
    //
    //! \name
    //@{
public:
    //! Default constructor
    MatchingViewer(void):
        _imagesRaw(2),
        _imagesDispRaw(2),
        _imagesDisp(2),
        _points(2),
        _flagDisplay(0),
        _alpha(1.0),
        colorPt{255, 0, 0},
        colorLine{0, 0, 255},
        colorBG{255, 255, 255},
        colorFG{0,0,0}
    {}
    //! Destructor
    ~MatchingViewer(void){}
    //@}

    //------------------------------------------
    //
    //! \name Private member variables and functions to access them
    //@{
    // images
private:
    ///
    /// \brief _imagesRaw
    /// _imagesRaw(0): The first image.
    /// _imagesRaw(1): The second image.
    cimg_library::CImgList<TI> _imagesRaw;
    ///
    /// \brief _imagesDispRaw
    /// _imagesDispRaw(0): An image alining \c _imagesRaw(0) and \c _imagesRaw(1).
    /// _imagesDispRaw(1): An image merging \c _imagesRaw(0) and \c _imagesRaw(1).
    cimg_library::CImgList<TI> _imagesDispRaw;
    cimg_library::CImgList<TI> _imagesDisp;
    double _alpha; //!< Alpha value for merging the two images \c _imagesRaw(0) and \c _imagesRaw(1).
public:
    //! sets the blending parameter.
    void alpha(double alpha = 1.0){_alpha = alpha;}
    //! returns the width of \c n-th image \c _imagesRaw(n).
    int width(const int n) const {return _imagesRaw(n).width();}
    //! returns the height of \c n-th image \c _imagesRaw(n).
    int height(const int n) const {return _imagesRaw(n).height();}

    //! returns \c n-th image \c _imagesRaw(n).
    cimg_library::CImg<TI> image(const int n) const {return _imagesRaw(n);}
    //! sets \c n-th image \c _imagesRaw(n).
    void image(const int n, const cimg_library::CImg<TI>& _image){_imagesRaw(n) = _image; imagesUpdate();}

    //! returns a list of the images \c _imagesRaw.
    cimg_library::CImgList<TI> images(void) const {return _imagesRaw;}
    //! sets a list of the images \c _imagesRaw.
    void images(const cimg_library::CImgList<TI>& _images){_imagesRaw = _images; imagesUpdate();}
    //! sets a list of the images \c _imagesRaw.
    void images(const cimg_library::CImg<TI>& _image0, const cimg_library::CImg<TI>& _image1){image(0, _image0); image(1, _image1); imagesUpdate();}
    //! sets a list of the images \c _imagesRaw.
    void images(const std::vector<std::string>& strImage);

    //! returns the aligning image \c _imagesDispRaw(0).
    cimg_library::CImg<TI> imgAlign(void) const {return _imagesDispRaw(0);}
    //! returns the merging image \c _imagesDispRaw(1).
    cimg_library::CImg<TI> imgMerge(void) const {return _imagesDispRaw(1);}
    void imagesAlign(void){_imagesDispRaw(0) = _imagesRaw.images(0,1).get_append('x');;}
    void imagesMerge(void){_imagesDispRaw(1) = _alpha*_imagesRaw(0)+(1.0-_alpha)*_imagesRaw(1);}
    void imagesUpdate(void);//{imagesAlign(); imagesMerge();}

    // points
private:
    ///
    /// \brief _points
    /// _points(0): A point set on the first image.
    /// _points(1): A point set on the second image.
    cimg_library::CImgList<TP> _points;
public:
    //! returns the number of points of \c n-th point set \c _points(n).
    int numberOfPoint(const int n) const {return _points(n).width();}
    //! returns \c n-th point set \c _points(n).
    cimg_library::CImg<TP> point(const int n) const {return _points(n);}
    //! sets \c n-th point set \c _points(n).
    void point(const int n, const cimg_library::CImg<TP>& point){_points(n) = point;}

    //! returns a set of point sets \c _points.
    cimg_library::CImgList<TP> points(void) const {return _points;}
    //! sets a set of point sets \c _points.
    void points(const cimg_library::CImgList<TP>& points){_points = points;}
    //! sets a set of point sets \c _points.
    void points(const cimg_library::CImg<TP>& point0, const cimg_library::CImg<TP>& point1){point(0, point0); point(1, point1);}

    // point-to-point correspondences
private:
    cimg_library::CImg<int> _correspondences; //!< A set of point-to-point correspondences.
public:
    //! returns the number of point-to-point correspondences.
    int numberOfCorrespondences(void) const {return _correspondences.width();}
    //! returns the set of point-to-point correspondences.
    cimg_library::CImg<int> correspondences(void) const {return _correspondences;}
    //! sets the set of point-to-point correspondences.
    void correspondences(const cimg_library::CImg<int>& correspondences){_correspondences = correspondences;}

    // energy
private:
    std::vector<double> _energy; //!< Energy for each point-to-point correspondence.
public:
    //! returns the number of point-to-point correspondences.
    int numberOfEnergy(void) const {return _energy.size();}
    //! sets a set of energy \c _energy
    void energy(const std::vector<double>& energy){_energy = energy;}
    //! sets a set of energy \c _energy
    std::vector<double> energy() const {return _energy;}

    // displays
    cimg_library::CImgDisplay _dispEnergy; //!< Display for showing energy of point-to-point correspondences.
    cimg_library::CImgDisplay _dispTrans; //!< Display for showing the transformation between two point sets.
    int _flagDisplay; //!< The flag indicating which display is shown.
    void flagDisplay(const int flagDisplay){_flagDisplay = flagDisplay;}
    void displayUpdate(void);
    cimg_library::CImg<TI> drawMatching(const cimg_library::CImg<TI>& _img);

    // variables for display
private:

    const unsigned char colorPt[3];
    const unsigned char colorLine[3];
    const unsigned char colorBG[3];
    const unsigned char colorFG[3];
    //@}
};
//------------------------------------------
//
//! \name
//@{
//@}


template <typename TI, typename TP>
void MatchingViewer<TI,TP>::imagesUpdate(void)
{
    imagesAlign();
    imagesMerge();
}

template <typename TI, typename TP>
void MatchingViewer<TI,TP>::images(const std::vector<std::string>& strImage)
{
    assert(
        _imagesRaw.size() == strImage.size() &&
        "The number of the given images must be same as one of the prepared image objects."
    );

    cimg_library::CImg<TI> _img;
    for(int n = 0; n < strImage.size(); ++n)
    {
        _img = cimg_library::CImg<TI>( strImage[n].c_str() );
        if(_img.spectrum() == 3)
        {
            image(n, cimg_library::CImg<TI>( getGrayscaledRGB( _img ) ));
        }
        else
        {
            image(n, cimg_library::CImg<TI>( getGraytoRGB( _img ) ));
        }
    }
    imagesUpdate();
}

template <typename TI, typename TP>
void MatchingViewer<TI,TP>::displayUpdate(void)
{
//    std::cout << "  1" << _dispEnergy.is_closed() << std::endl;
//    _dispEnergy = _imagesDispRaw(0);
////    _imagesDispRaw(0).display(_dispEnergy);
//    std::cout << "  2" << _dispEnergy.is_closed() << std::endl;
//    _dispEnergy.show();
//    std::cout << "  3" << _dispEnergy.is_closed() << std::endl;
//    while(!_dispEnergy.is_closed())
//    {
//        _dispEnergy.wait();
//        if(_dispEnergy.is_keyQ() || _dispEnergy.is_keyESC())
//        {
//            _dispEnergy.close();
//            std::cout << "  pushed" << std::endl;
//        }
//    }
//    std::cout << "  4" << _dispEnergy.is_closed() << std::endl;
//    _dispEnergy = _imagesDispRaw(0);
    drawMatching(_imagesDispRaw(0)).display(_dispEnergy);
    bool _flag = true;
    while(_flag)
    {
        _dispEnergy.wait();
        if(_dispEnergy.is_keyQ() || _dispEnergy.is_keyESC())
        {
            _flag = false;
            std::cout << "  pushed" << std::endl;
        }
    }
}

template <typename TI, typename TP>
cimg_library::CImg<TI> MatchingViewer<TI,TP>::drawMatching(const cimg_library::CImg<TI> &_img)
{
    cimg_library::CImg<TI> img(_img);
    int offset = _imagesRaw(0).width();
    int radius = 4;
    int x0, y0, x1, y1;
    int i0, i1;
    int numDraw = _correspondences.width();
    for(int m = 0; m <= numDraw; ++m)
    {
        i0 = _correspondences(m,0);
        i1 = _correspondences(m,1);
        if(i0>=0 && i0< _points(0).width() && i1>=0 && i1<_points(1).width())
        {
            x0 = _points(0)(i0,0);
            y0 = _points(0)(i0,1);
            x1 = _points(1)(i1,0)+offset;
            y1 = _points(1)(i1,1);
            draw_line_thick(img, x0, y0, x1, y1, colorLine, radius/2);
            img.draw_circle(x0, y0, radius, colorPt, 1.f);
            img.draw_triangle(x1, y1-radius, x1-radius, y1+radius, x1+radius, y1+radius, colorPt);
        }
    }

    int c0, c1;
    std::stringstream ss;
    ss << "correspondence#";
    if(numDraw>=0)
    {
        c0 = _correspondences(numDraw,0);
        c1 = _correspondences(numDraw,1);
        ss << numDraw << " = (";
        if(c0>=0)   ss << "p" << c0;
        else        ss << "-";
        ss << ",";
        if(c1>=0)   ss << "q" << c1;
        else        ss << "-";
        ss << ") = " << _energy[numDraw];
    }
    img.draw_text(0, 0, ss.str().c_str(), colorFG, colorBG, 1, 25);

    return img;
}


#endif
