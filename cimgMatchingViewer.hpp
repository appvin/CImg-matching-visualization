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
        _imagesRaw(2),
        _imagesDispRaw(2),
        _imagesDisp(2),
        _points(2),
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
    //! returns the width of the first image \c _imagesRaw(0).
    int w0(void) const {return _imagesRaw(0).width();}
    //! returns the height of the first image \c _imagesRaw(0).
    int h0(void) const {return _imagesRaw(0).height();}
    //! returns the width of the second image \c _imagesRaw(1).
    int w1(void) const {return _imagesRaw(1).width();}
    //! returns the height of the second image \c _imagesRaw(1).
    int h1(void) const {return _imagesRaw(1).height();}
    //! returns the first image \c _imagesRaw(0).
    cimg_library::CImg<TI> img0(void) const {return _imagesRaw(0);}
    //! sets the first image \c _imagesRaw(0).
    void img0(const cimg_library::CImg<TI>& _img0){_imagesRaw(0) = _img0; imagesUpdate();}
    //! returns the second image \c _imagesRaw(1).
    cimg_library::CImg<TI> img1(void) const {return _imagesRaw(1);}
    //! sets the second image \c _imagesRaw(1).
    void img1(const cimg_library::CImg<TI>& _img1){_imagesRaw(1) = _img1; imagesUpdate();}
    //! sets the images \c _imagesRaw(0) and \c _imagesRaw(1).
    void imgs(const cimg_library::CImg<TI>& _img0, const cimg_library::CImg<TI>& _img1){img0(_img0); img1(_img1);}
    //! returns the aligning image \c _imagesDispRaw(0).
    cimg_library::CImg<TI> imgAlign(void) const {return _imagesDispRaw(0);}
    //! returns the merging image \c _imagesDispRaw(1).
    cimg_library::CImg<TI> imgMerge(void) const {return _imagesDispRaw(1);}
    void imagesAlign(void){_imagesDispRaw(0) = _imagesRaw.images(0,1).get_append('x');;}
    void imagesMerge(void){_imagesDispRaw(1) = _alpha*_imagesRaw(0)+(1.0-_alpha)*_imagesRaw(1);}
    void imagesUpdate(void){imagesAlign(); imagesMerge();}

    // points
private:
    ///
    /// \brief _points
    /// _points(0): A point set on the first image.
    /// _points(1): A point set on the second image.
    cimg_library::CImgList<TP> _points;
    cimg_library::CImg<int> _correspondences; //!< A set of point-to-point correspondences.
public:
    //! returns the first point set \c _points(0).
    cimg_library::CImg<TP> pt0(void) const {return _points(0);}
    //! sets the first point set \c _points(1).
    void pt0(cimg_library::CImg<TP>& _pt0){_points(0) = _pt0;}
    //! returns the second point set \c _points(1).
    cimg_library::CImg<TP> pt1(void) const {return _points(1);}
    //! sets the second point set \c _points(1).
    void pt1(cimg_library::CImg<TP>& _pt1){_points(1) = _pt1;}
    //! sets the two point set \c _points(0) and \c _points(1).
    void pts(cimg_library::CImg<TP>& _pt0, cimg_library::CImg<TP>& _pt1){pt0(_pt0); pt1(_pt1);}

    //! returns the set of point-to-point correspondences.
    cimg_library::CImg<int> correspondences(void) const {return _correspondences;}
    //! sets the set of point-to-point correspondences.
    void correspondences(const cimg_library::CImg<int>& correspondences){_correspondences = correspondences;}

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
