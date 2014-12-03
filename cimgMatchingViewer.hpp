#ifndef cimgMatchingViewer
#define cimgMatchingViewer

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
        images(4),
        points(2),
        _alpha(1.0)
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
    /// \brief images
    /// images(0): The first image.
    /// images(1): The first image.
    /// images(0): An image alining \c images(0) and \c images(1).
    /// images(0): An image merging \c images(0) and \c images(1).
    cimg_library::CImgList<TI> images;
    double _alpha; //!< Alpha value for merging the two images \c images(0) and \c images(1).
    void imagesAlign(void){images(2) = images.images(0,1).get_append('x');;}
    void imagesMerge(void){images(3) = _alpha*images(0)+(1.0-_alpha)*images(1);}
    void imagesUpdate(void){imagesAlign(); imagesMerge();}
public:
    //! sets the blending parameter.
    void alpha(double alpha = 1.0){_alpha = alpha;}
    //! returns the width of the first image \c images(0).
    int w0(void) const {return images(0).width();}
    //! returns the height of the first image \c images(0).
    int h0(void) const {return images(0).height();}
    //! returns the width of the second image \c images(1).
    int w1(void) const {return images(1).width();}
    //! returns the height of the second image \c images(1).
    int h1(void) const {return images(1).height();}
    //! returns the first image \c images(0).
    cimg_library::CImg<TI> img0(void) const {return images(0);}
    //! sets the first image \c images(0).
    void img0(const cimg_library::CImg<TI>& _img0){images(0) = _img0; imagesUpdate();}
    //! returns the second image \c images(1).
    cimg_library::CImg<TI> img1(void) const {return images(1);}
    //! sets the second image \c images(1).
    void img1(const cimg_library::CImg<TI>& _img1){images(1) = _img1; imagesUpdate();}
    //! sets the images \c images(0) and \c images(1).
    void imgs(const cimg_library::CImg<TI>& _img0, const cimg_library::CImg<TI>& _img1){img0(_img0); img1(_img1);}
    //! returns the aligning image \c images(2).
    cimg_library::CImg<TI> imgAlign(void) const {return images(2);}
    //! returns the merging image \c images(3).
    cimg_library::CImg<TI> imgMerge(void) const {return images(3);}

    // points
private:
    cimg_library::CImgList<TP> points;
public:
    //! returns the first point set \c points(0).
    cimg_library::CImg<TP> pt0(void) const {return points(0);}
    //! sets the first point set \c points(1).
    void pt0(cimg_library::CImg<TP>& _pt0){points(0) = _pt0;}
    //! returns the second point set \c points(1).
    cimg_library::CImg<TP> pt1(void) const {return points(1);}
    //! sets the second point set \c points(1).
    void pt1(cimg_library::CImg<TP>& _pt1){points(1) = _pt1;}
    //! sets the two point set \c points(0) and \c points(1).
    void pts(cimg_library::CImg<TP>& _pt0, cimg_library::CImg<TP>& _pt1){pt0(_pt0); pt1(_pt1);}

    // energy
private:
    std::vector<double> energy; //!< Energy for each point-to-point correspondence.

    // displays
    cimg_library::CImgDisplay _dispEnergy; //!< Display for showing energy of point-to-point correspondences.
    cimg_library::CImgDisplay _dispTrans; //!< Display for showing the transformation between two point sets.
    //@}
};
//------------------------------------------
//
//! \name
//@{
//@}

#endif
