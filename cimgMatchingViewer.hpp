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
