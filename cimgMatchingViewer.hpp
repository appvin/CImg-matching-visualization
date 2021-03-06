#ifndef cimgMatchingViewer
#define cimgMatchingViewer

#include <string>
#include "cimgConvertColor.hpp"
#include "cimgDrawLineThick.hpp"
#include <CImg.h>

static const unsigned char _colorPt[3] = {255, 0, 0};     //!< Color for points
static const unsigned char _colorLine[3] = {0, 0, 255};   //!< Color for lines
static const unsigned char _colorLineCurrent[3] = {255, 0, 0};   //!< Color for lines of current correspondences
static const unsigned char _colorLineNew[3] = {0, 0, 255};   //!< Color for lines of new correspondences
static const unsigned char _colorTextBg[3] = {255, 255, 255};     //!< Color for background of text area
static const unsigned char _colorTextFg[3] = {0, 0, 0};     //!< Color for foreground of text area

template <typename TI, typename TP>
class MatchingViewer
{
    //------------------------------------------
    //
    //! \name
    //@{
public:
    //! Default constructor
    MatchingViewer(
        const bool flagDebug = false
    ):
        _imagesRaw(2),
        _imagesDispRaw(2),
        _imagesDisp(2),
        _points(2),
        _flagDisplay(0),
        _alpha(1.0),
        _flagDebug(flagDebug)/*,
        _colorPt{255, 0, 0},
        _colorLine{0, 0, 255},
        _colorTextBg{255, 255, 255},
        _colorTextFg{0,0,0}*/
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
    cimg_library::CImg<TI>& image(const int n){return _imagesRaw(n);}
    //! sets \c n-th image \c _imagesRaw(n).
    void image(const int n, const cimg_library::CImg<TI>& _image){_imagesRaw(n) = _image; imagesUpdate();}

    //! returns a list of the images \c _imagesRaw.
    cimg_library::CImgList<TI> images(void) const {return _imagesRaw;}
    cimg_library::CImgList<TI>& images(void){return _imagesRaw;}
    //! sets a list of the images \c _imagesRaw.
    void images(const cimg_library::CImgList<TI>& _images){_imagesRaw = _images; imagesUpdate();}
    //! sets a list of the images \c _imagesRaw.
    void images(const cimg_library::CImg<TI>& _image0, const cimg_library::CImg<TI>& _image1){image(0, _image0); image(1, _image1); imagesUpdate();}
    //! sets a list of the images \c _imagesRaw.
    void images(const std::vector<std::string>& strImage);

    //! returns the aligning image \c _imagesDispRaw(0).
    cimg_library::CImg<TI> imgAlign(void) const {return _imagesDispRaw(0);}
    cimg_library::CImg<TI>& imgAlign(void){return _imagesDispRaw(0);}
    //! returns the merging image \c _imagesDispRaw(1).
    cimg_library::CImg<TI> imgMerge(void) const {return _imagesDispRaw(1);}
    cimg_library::CImg<TI>& imgMerge(void){return _imagesDispRaw(1);}
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
    cimg_library::CImg<TP>& point(const int n){return _points(n);}
    //! sets \c n-th point set \c _points(n).
    void point(const int n, const cimg_library::CImg<TP>& point){_points(n) = point;}

    //! returns a set of point sets \c _points.
    cimg_library::CImgList<TP> points(void) const {return _points;}
    cimg_library::CImgList<TP>& points(void){return _points;}
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
    cimg_library::CImg<int>& correspondences(void){return _correspondences;}
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
    std::vector<double>& energy(){return _energy;}


    // variables for display
private:
    bool _flagDebug; //!< A flag indicating debug mode
public:
    void flagDebug(const bool &flagDebug){_flagDebug = flagDebug;}
    bool flagDebug(void) const {return _flagDebug;}

    // displays
private:
    cimg_library::CImgDisplay _dispEnergy; //!< Display for showing energy of point-to-point correspondences.
    cimg_library::CImgDisplay _dispTrans; //!< Display for showing the transformation between two point sets.
public:
    //! gets \c _dispEnergy
    cimg_library::CImgDisplay dispEnergy(void) const {return _dispEnergy;}
    cimg_library::CImgDisplay& dispEnergy(void){return _dispEnergy;}
    //! sets \c _dispEnergy
    void dispEnergy(const cimg_library::CImgDisplay& dispEnergy){_dispEnergy = dispEnergy;}

private:
    int _flagDisplay; //!< The flag indicating which display is shown.
public:
    void flagDisplay(const int flagDisplay){_flagDisplay = flagDisplay;}
    void displayUpdate(void);
    void displayUpdate(
        const cimg_library::CImg<int>& correspondences,
        const std::vector<double>& energy
    );
    cimg_library::CImg<TI> drawMatching(
        const cimg_library::CImg<TI>& _img,
        const unsigned char colorPt[] = _colorPt,
        const unsigned char colorLine[] = _colorLine
    );
    cimg_library::CImg<TI> drawMatching(
        const cimg_library::CImg<TI>& _img,
        const int numDraw,
        const unsigned char colorPt[] = _colorPt,
        const unsigned char colorLine[] = _colorLine,
        const std::string strTitle = ""
    );
    cimg_library::CImg<TI> drawMatching(
        const cimg_library::CImg<TI>& _img,
        const cimg_library::CImg<int>& correspondences,
        const std::vector<double>& energy,
        const int numDraw,
        const unsigned char colorPt[] = _colorPt,
        const unsigned char colorLine[] = _colorLine,
        const std::string strTitle = ""
    );
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
    cimg_library::CImg<TI> imgShow(_imagesDispRaw(0));

    if(!_flagDebug)
    { // non-debug mode
        _dispEnergy.wait(300);
        drawMatching( imgShow ).display(_dispEnergy);
    }
    else
    { // debug mode
        int numPointCur = 0, numPointPrev;
        bool _flag = true;
        drawMatching( imgShow, numPointCur ).display(_dispEnergy);
        while(_flag)
        {
            // check any user input
            _dispEnergy.wait();
            // retrieve the input
            if(_dispEnergy.wheel()!=0)
            {
                numPointCur += _dispEnergy.wheel();
                _dispEnergy.set_wheel();
            }

            if(_dispEnergy.is_keyARROWDOWN() || _dispEnergy.is_keyARROWLEFT())
            {
                --numPointCur;
            }
            if(_dispEnergy.is_keyARROWUP() || _dispEnergy.is_keyARROWRIGHT())
            {
                ++numPointCur;
            }
            if(_dispEnergy.is_keyQ() || _dispEnergy.is_keyESC())
            {
                _flag = false;
            }

            // update the image
            else
            { // update the image
                numPointCur = std::min(_correspondences.width()-1,numPointCur);
                numPointCur = std::max(numPointCur, -1);
                if(numPointCur != numPointPrev)
                {
                    imgShow = _imagesDispRaw(0);
                    drawMatching(imgShow, numPointCur).display( _dispEnergy );
                    numPointPrev = numPointCur;
                }
            }
        }
    }
}

template <typename TI, typename TP>
void MatchingViewer<TI,TP>::displayUpdate(
    const cimg_library::CImg<int>& correspondences,
    const std::vector<double>& energy
)
{
    _correspondences = correspondences;
    _energy = energy;
    displayUpdate();
}

template <typename TI, typename TP>
cimg_library::CImg<TI> MatchingViewer<TI,TP>::drawMatching(
    const cimg_library::CImg<TI> &_img,
    const unsigned char colorPt[],
    const unsigned char colorLine[]
)
{
    return drawMatching(_img, _correspondences.width(), colorPt, colorLine);
}

template <typename TI, typename TP>
cimg_library::CImg<TI> MatchingViewer<TI,TP>::drawMatching(
    const cimg_library::CImg<TI> &_img,
    const int numDraw,
    const unsigned char colorPt[],
    const unsigned char colorLine[],
    const std::string strTitle
)
{
    cimg_library::CImg<TI> img(_img);
    int offset = _imagesRaw(0).width();
    int radius = 4;
    int x0, y0, x1, y1;
    int i0, i1;

    /// draw matching
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
            img.draw_circle(x1, y1, radius, colorPt, 1.f);
//            img.draw_triangle(x1, y1-radius, x1-radius, y1+radius, x1+radius, y1+radius, _colorPt);
        }
    }

    /// draw energy
    int fontsize = 25;
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
    img.draw_text(0, 0, ss.str().c_str(), _colorTextFg, _colorTextBg, 1, fontsize);

    /// draw title
    if(strTitle.length()>0)
    {
        img.draw_text( (img.width()-strTitle.length()*fontsize)/2, img.height()-fontsize*2, strTitle.c_str(), _colorTextFg, _colorTextBg, 1, fontsize*2);
    }

    return img;
}

template <typename TI, typename TP>
cimg_library::CImg<TI> MatchingViewer<TI,TP>::drawMatching(
    const cimg_library::CImg<TI>& _img,
    const cimg_library::CImg<int>& correspondences,
    const std::vector<double>& energy,
    const int numDraw,
    const unsigned char colorPt[],
    const unsigned char colorLine[],
    const std::string strTitle
)
{
    _correspondences = correspondences;
    _energy = energy;
    return drawMatching(_img, numDraw, colorPt, colorLine, strTitle);
}

template <typename TI, typename TP>
class MatchingViewerMoveMaking :public MatchingViewer<TI,TP>{
    //------------------------------------------
    //
    //! \name
    //@{
public:
    //! Default constructor
    MatchingViewerMoveMaking(){}
    //! Destructor
    ~MatchingViewerMoveMaking(void){}
    //@}

    // point-to-point correspondences
private:
    cimg_library::CImg<int> _correspondencesCurrent; //!< A current set of point-to-point correspondences.
    cimg_library::CImg<int> _correspondencesNew; //!< A new set of point-to-point correspondences.
    cimg_library::CImg<int> _correspondencesFusion; //!< A fused set of point-to-point correspondences.
public:
    //! returns the number of point-to-point correspondences.
    int numberOfCorrespondences(void) const {return _correspondencesCurrent.width();}
    //! returns the set of point-to-point correspondences.
    cimg_library::CImg<int> correspondences(const int c) const {
        if(c==0)        return _correspondencesCurrent;
        else if(c==1)   return _correspondencesNew;
        else            return _correspondencesFusion;
    }
    //! sets the set of point-to-point correspondences.
    void correspondences(
            const cimg_library::CImg<int>& correspondencesCurrent,
            const cimg_library::CImg<int>& correspondencesNew,
            const cimg_library::CImg<int>& correspondencesFusion
    )
    {
        _correspondencesCurrent = correspondencesCurrent;
        _correspondencesNew = correspondencesNew;
        _correspondencesFusion = correspondencesFusion;
    }

    // energy
private:
    std::vector<double> _energyCurrent; //!< Energy for current point-to-point correspondence.
    std::vector<double> _energyNew; //!< Energy for new point-to-point correspondence.
    std::vector<double> _energyFusion; //!< Energy for fused point-to-point correspondence.
public:
    //! returns the number of point-to-point correspondences.
    int numberOfEnergy(void) const {return _energyCurrent.size();}
    //! sets a set of energy \c _energy
    void energy(
        const std::vector<double>& energyCurrent,
        const std::vector<double>& energyNew,
        const std::vector<double>& energyFusion
    )
    {
        _energyCurrent = energyCurrent;
        _energyNew = energyNew;
        _energyFusion = energyFusion;
    }
    //! gets a set of energy \c _energy
    std::vector<double> energy(const int e) const
    {
        if(e==0)        return _energyCurrent;
        else if(e==1)   return _energyNew;
        else            return _energyFusion;
    }

    // displays
    void displayUpdate(void);
    void displayUpdate(
        const cimg_library::CImg<int>& correspondencesCurrent,
        const cimg_library::CImg<int>& correspondencesNew,
        const cimg_library::CImg<int>& correspondencesFusion,
        const std::vector<double>& energyCurrent,
        const std::vector<double>& energyNew,
        const std::vector<double>& energyFusion
    );

    cimg_library::CImg<TI> drawMatching(
        const cimg_library::CImg<TI>& _img,
        const cimg_library::CImg<int>& correspondencesCurrent,
        const cimg_library::CImg<int>& correspondencesNew,
        const cimg_library::CImg<int>& correspondencesFusion,
        const std::vector<double>& energyFusion,
        const int numDraw,
        const unsigned char colorPt[] = _colorPt,
        const unsigned char colorLineCurrent[] = _colorLineCurrent,
        const unsigned char colorLineNew[] = _colorLineNew,
        const std::string strTitle = ""
    );

    cimg_library::CImg<TI> updateImageCurrent(
        const cimg_library::CImg<TI>& _img,
        const int numDraw
    );
    cimg_library::CImg<TI> updateImageNew(
        const cimg_library::CImg<TI>& _img,
        const int numDraw
    );
    cimg_library::CImg<TI> updateImageFusion(
        const cimg_library::CImg<TI>& _img,
        const int numDraw
    );
    cimg_library::CImg<TI> updateImages(
        const cimg_library::CImg<TI>& _img,
        const int numDraw
    );

};

template <typename TI, typename TP>
cimg_library::CImg<TI> MatchingViewerMoveMaking<TI,TP>::updateImageCurrent(
    const cimg_library::CImg<TI>& _img,
    const int numDraw
)
{
    return MatchingViewer<TI,TP>::drawMatching( _img, _correspondencesCurrent, _energyCurrent, numDraw, _colorPt, _colorLineCurrent, "Current matching");
}

template <typename TI, typename TP>
cimg_library::CImg<TI> MatchingViewerMoveMaking<TI,TP>::updateImageNew(
    const cimg_library::CImg<TI>& _img,
    const int numDraw
)
{
    return MatchingViewer<TI,TP>::drawMatching( _img, _correspondencesNew, _energyNew, numDraw, _colorPt, _colorLineNew, "Proposed matching");
}

template <typename TI, typename TP>
cimg_library::CImg<TI> MatchingViewerMoveMaking<TI,TP>::updateImageFusion(
    const cimg_library::CImg<TI>& _img,
    const int numDraw
)
{
    return drawMatching( _img, _correspondencesCurrent, _correspondencesNew, _correspondencesFusion, _energyFusion, numDraw, _colorPt, _colorLineCurrent, _colorLineNew, "Fused matching");
}

template <typename TI, typename TP>
cimg_library::CImg<TI> MatchingViewerMoveMaking<TI,TP>::updateImages(
    const cimg_library::CImg<TI>& _img,
    const int numDraw
)
{
    cimg_library::CImg<TI> imgCurrent = updateImageCurrent(_img, numDraw);
    cimg_library::CImg<TI> imgNew = updateImageNew(_img, numDraw);
    cimg_library::CImg<TI> imgFusion = updateImageFusion(_img, numDraw);
    return imgCurrent.append(imgNew,'y').append(imgFusion,'y');
}

template <typename TI, typename TP>
void MatchingViewerMoveMaking<TI,TP>::displayUpdate(void)
{
    cimg_library::CImg<TI> imgShow(MatchingViewer<TI,TP>::imgAlign());
    updateImages(imgShow, numberOfCorrespondences()).display( MatchingViewer<TI,TP>::dispEnergy() );

    if(!MatchingViewer<TI,TP>::flagDebug())
    { // non-debug mode
        MatchingViewer<TI,TP>::dispEnergy().wait(300);
        updateImages(imgShow, numberOfCorrespondences()).display( MatchingViewer<TI,TP>::dispEnergy() );
    }
    else
    { // debug mode
        int numPointCur = 0, numPointPrev;
        bool _flag = true;
        updateImages(imgShow, numPointCur).display( MatchingViewer<TI,TP>::dispEnergy() );
        while(_flag)
        {
            // check any user input
            MatchingViewer<TI,TP>::dispEnergy().wait();
            // retrieve the input
            if(MatchingViewer<TI,TP>::dispEnergy().wheel()!=0)
            {
                numPointCur += MatchingViewer<TI,TP>::dispEnergy().wheel();
                MatchingViewer<TI,TP>::dispEnergy().set_wheel();
            }

            if(MatchingViewer<TI,TP>::dispEnergy().is_keyARROWDOWN() || MatchingViewer<TI,TP>::dispEnergy().is_keyARROWLEFT())
            {
                --numPointCur;
            }
            if(MatchingViewer<TI,TP>::dispEnergy().is_keyARROWUP() || MatchingViewer<TI,TP>::dispEnergy().is_keyARROWRIGHT())
            {
                ++numPointCur;
            }
            if(MatchingViewer<TI,TP>::dispEnergy().is_keyQ() || MatchingViewer<TI,TP>::dispEnergy().is_keyESC())
            {
                _flag = false;
            }

            // update the image
            else
            { // update the image
                numPointCur = std::min(_correspondencesCurrent.width()-1,numPointCur);
                numPointCur = std::max(numPointCur, -1);
                if(numPointCur != numPointPrev)
                {
                    updateImages(imgShow, numPointCur).display( MatchingViewer<TI,TP>::dispEnergy() );
                    numPointPrev = numPointCur;
                }
            }
        }
    }
}

template <typename TI, typename TP>
void MatchingViewerMoveMaking<TI,TP>::displayUpdate(
    const cimg_library::CImg<int>& correspondencesCurrent,
    const cimg_library::CImg<int>& correspondencesNew,
    const cimg_library::CImg<int>& correspondencesFusion,
    const std::vector<double>& energyCurrent,
    const std::vector<double>& energyNew,
    const std::vector<double>& energyFusion
)
{
    correspondences(
        correspondencesCurrent,
        correspondencesNew,
        correspondencesFusion
    );
    energy(
        energyCurrent,
        energyNew,
        energyFusion
    );
    displayUpdate();
}



template <typename TI, typename TP>
cimg_library::CImg<TI> MatchingViewerMoveMaking<TI,TP>::drawMatching(
    const cimg_library::CImg<TI> &_img,
    const cimg_library::CImg<int>& correspondencesCurrent,
    const cimg_library::CImg<int>& correspondencesNew,
    const cimg_library::CImg<int>& correspondencesFusion,
    const std::vector<double>& energyFusion,
    const int numDraw,
    const unsigned char colorPt[],
    const unsigned char colorLineCurrent[],
    const unsigned char colorLineNew[],
    const std::string strTitle
)
{
    cimg_library::CImg<TI> img(_img);
    int offset = MatchingViewer<TI,TP>::image(0).width();
    int radius = 4;
    int x0, y0, x1, y1;
    int i0, i1;

    bool flagFusion;
    /// draw matching
    for(int m = 0; m <= numDraw; ++m)
    {
        i0 = correspondencesFusion(m,0);
        if(correspondencesFusion(m,1) == 1)
        {
            flagFusion = true;
            i1 = correspondencesNew(m,1);
        }
        else
        {
            flagFusion = false;
            i1 = correspondencesCurrent(m,1);
        }
        if(i0>=0 && i0< MatchingViewer<TI,TP>::point(0).width() && i1>=0 && i1<MatchingViewer<TI,TP>::point(1).width())
        {
            x0 = MatchingViewer<TI,TP>::point(0)(i0,0);
            y0 = MatchingViewer<TI,TP>::point(0)(i0,1);
            x1 = MatchingViewer<TI,TP>::point(1)(i1,0)+offset;
            y1 = MatchingViewer<TI,TP>::point(1)(i1,1);
            if(flagFusion)
            {
                draw_line_thick(img, x0, y0, x1, y1, colorLineNew, radius/2);
            }
            else
            {
                draw_line_thick(img, x0, y0, x1, y1, colorLineCurrent, radius/2);
            }
            img.draw_circle(x0, y0, radius, colorPt, 1.f);
            img.draw_circle(x1, y1, radius, colorPt, 1.f);
//            img.draw_triangle(x1, y1-radius, x1-radius, y1+radius, x1+radius, y1+radius, _colorPt);
        }
    }

    /// draw energy
    int fontsize = 25;
    int c0, c1;
    std::stringstream ss;
    ss << "correspondence#";
    if(numDraw>=0)
    {
        c0 = correspondencesFusion(numDraw,0);
        if(correspondencesFusion(numDraw,1) == 1)
        {
            c1 = correspondencesNew(numDraw,1);
        }
        else
        {
            c1 = correspondencesCurrent(numDraw,1);
        }
        ss << numDraw << " = (";
        if(c0>=0)   ss << "p" << c0;
        else        ss << "-";
        ss << ",";
        if(c1>=0)   ss << "q" << c1;
        else        ss << "-";
        ss << ") = " << energyFusion[numDraw];
    }
    img.draw_text(0, 0, ss.str().c_str(), _colorTextFg, _colorTextBg, 1, fontsize);

    /// draw title
    if(strTitle.length()>0)
    {
        img.draw_text( (img.width()-strTitle.length()*fontsize)/2, img.height()-fontsize*2, strTitle.c_str(), _colorTextFg, _colorTextBg, 1, fontsize*2);
    }

    return img;
}

#endif
