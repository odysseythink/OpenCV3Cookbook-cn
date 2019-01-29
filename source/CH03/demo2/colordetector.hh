/**
 * @file      colordetector.hh
 * @brief     
 * @details   
 * @author    ranwei
 * @version     
 * @date      2019/1/12 11:13:43:482
 * @copyright R_W
 * @par         (c) COPYRIGHT 2010-2018 by R_W Systems, Inc.    
 *                        All rights reserved.
 *                                                                    
 *       This software is confidential and proprietary to R_W 
 *     Systems, Inc.  No part of this software may be reproduced,    
 *     stored, transmitted, disclosed or used in any form or by any means
 *     other than as expressly provided by the written license agreement    
 *     between R_W Systems and its licensee.
 * @par History      
 *         1.Date         -- 2019/1/12 11:13:43:482
 *           Author       -- ranwei
 *           Modification -- Created file
 *
 */
#ifndef __COLORDETECTOR_HH__
#define __COLORDETECTOR_HH__

#ifdef  COLORDETECTOR_GLOBAL
#define COLORDETECTOR_EXT
#else
#define COLORDETECTOR_EXT extern
#endif /* COLORDETECTOR_GLOBAL */

/*============================================================================*/
/*                                  @INCLUDES                                 */
/*============================================================================*/
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>


 
/** @addtogroup COLORDETECTOR
  * @{
  */
 
/*============================================================================*/
/*                             @MACROS & @TYPEDEFS                            */
/*============================================================================*/
                                                                                
/*============================================================================*/
/*                             @GLOBAL VIRIABLES                              */
/*============================================================================*/
                                                                                
/*============================================================================*/
/*                                   @FUNCS                                   */
/*============================================================================*/
                                                                                
/*============================================================================*/
/*                                   @CLASS                                   */
/*============================================================================*/
/*! @class CColorDetector 
 *  @brief 颜色检测类 @anchor CColorDetector_Details
 *  
 */ 		
class CColorDetector
{
public:
    CColorDetector();
    /* extra constructor for Lab color space example */
    CColorDetector(bool useLab);

    /* full constructor */
    CColorDetector(uchar blue, uchar green, uchar red, int mxDist=100, bool useLab=false);

    // Computes the distance from target color.
    int GetDistanceToTargetColor(const cv::Vec3b& color) const;

    // Computes the city-block distance between two colors.
    int GetColorDistance(const cv::Vec3b& color1, const cv::Vec3b& color2) const ;

    // Processes the image. Returns a 1-channel binary image.
    cv::Mat Process(const cv::Mat &image);

    cv::Mat operator()(const cv::Mat &image) ;

    // Getters and setters

    // Sets the color distance threshold.
    // Threshold must be positive, otherwise distance threshold
    // is set to 0.
    void SetColorDistanceThreshold(int distance);

    // Gets the color distance threshold
    int GetColorDistanceThreshold() const ;

    // Sets the color to be detected
    // given in BGR color space
    void SetTargetColor(uchar blue, uchar green, uchar red);
    // Sets the color to be detected
    void SetTargetColor(cv::Vec3b color) ;

    // Gets the color to be detected
    cv::Vec3b GetTargetColor() const ;


private:
    int m_nMinDist;/*!< minimum acceptable distance */
    cv::Vec3b m_iTarget; /*!< target color */
    cv::Mat m_iConverted;/*!< image containing color converted image */
    bool m_bUseLab;
    cv::Mat m_iResult;/*!< image containing resulting binary map */
};                                                                                

/**
  * @}
  */ 
		
#endif /* __COLORDETECTOR_HH__ */
/****************** (C) COPYRIGHT 2010-2018 R_W ******END OF FILE**************/

