/**
 * @file      colordetector.cpp
 * @brief     
 * @details   
 * @author    ranwei
 * @version     
 * @date      2019/1/12 11:14:42:39
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
 *         1.Date         -- 2019/1/12 11:14:42:39
 *           Author       -- ranwei
 *           Modification -- Created file
 *
 */

#define  COLORDETECTOR_GLOBAL

/* includes-------------------------------------------------------------------*/
#include "colordetector.hh"
#include <vector>



/** @defgroup COLORDETECTOR                                            
  * @brief COLORDETECTOR system modules                                
  * @{                                                                         
  */
    
/* Private typedef&macro&definde----------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* External functions --------------------------------------------------------*/
/* External variables --------------------------------------------------------*/
                                                                                
/** @defgroup COLORDETECTOR_Private_Functions                          
  * @{                                                                         
  */                                                                            
                                                                                
/**                                                                             
  * @}                                                                         
  */	                                                                        
                                                                                
/** @defgroup COLORDETECTOR_CLASS_Functions                          
  * @{                                                                         
  */                                                                            

/**
  * @brief      空构造器：参数被初始化为默认值。
  * @anchor     CColorDetector.CColorDetector
  * @details    
  * @note       
  * @par   History      :
  *           1.Date    -- 2019/1/12 11:21:52:406
  *             Author  -- ranwei
  *             Details -- Created function
  */
CColorDetector::CColorDetector()
    : m_nMinDist(100), m_iTarget(0,0,0), m_bUseLab(false) 
{

}

/**
  * @brief      初始化useLab参数的构造器。
  * @anchor     CColorDetector.CColorDetector
  * @details    extra constructor for Lab color space example。
  * @param[in]  useLab: 。
  * @note       
  * @par   History      :
  *           1.Date    -- 2019/1/12 12:22:8:385
  *             Author  -- ranwei
  *             Details -- Created function
  */
CColorDetector::CColorDetector(bool useLab)
    : m_nMinDist(100), m_iTarget(0,0,0), m_bUseLab(useLab) 
{
}

/**
  * @brief      全功能构造器。
  * @anchor     CColorDetector.CColorDetector
  * @details    
  * @param[in]  blue: .
  * @param[in]  green: .
  * @param[in]  red: .
  * @param[in]  mxDist: .
  * @param[in]  useLab: .
  * @note       
  * @par   History      :
  *           1.Date    -- 2019/1/12 12:23:55:230
  *             Author  -- ranwei
  *             Details -- Created function
  */
CColorDetector::CColorDetector(uchar blue, uchar green, uchar red, int mxDist, bool useLab)
    : m_nMinDist(mxDist), m_bUseLab(useLab)
{
    
    SetTargetColor(blue, green, red);/* target color */
}

// Computes the distance from target color.
int CColorDetector::GetDistanceToTargetColor(const cv::Vec3b& color) const
{
    return GetColorDistance(color, m_iTarget);
}

// Computes the city-block distance between two colors.
int CColorDetector::GetColorDistance(const cv::Vec3b& color1, const cv::Vec3b& color2) const {

    return abs(color1[0]-color2[0])+
              abs(color1[1]-color2[1])+
              abs(color1[2]-color2[2]);

    // Or:
    // return static_cast<int>(cv::norm<int,3>(cv::Vec3i(color[0]-color2[0],color[1]-color2[1],color[2]-color2[2])));

    // Or:
    // cv::Vec3b dist;
    // cv::absdiff(color,color2,dist);
    // return cv::sum(dist)[0];
}

cv::Mat CColorDetector::operator()(const cv::Mat &image) 
{
    cv::Mat input;

    if (m_bUseLab) { // Lab conversion
        cv::cvtColor(image, input, CV_BGR2Lab);
    }
    else {
        input = image;
    }

    cv::Mat output;
    // compute absolute difference with target color
    cv::absdiff(input,cv::Scalar(m_iTarget),output);
    // split the channels into 3 images
    std::vector<cv::Mat> images;
    cv::split(output,images);
    // add the 3 channels (saturation might occurs here)
    output= images[0]+images[1]+images[2];
    // apply threshold
    cv::threshold(output,  // input image
                output,  // output image
                m_nMinDist, // threshold (must be < 256)
                255,     // max value
           cv::THRESH_BINARY_INV); // thresholding type

    return output;
}

// Getters and setters

// Sets the color distance threshold.
// Threshold must be positive, otherwise distance threshold
// is set to 0.
void CColorDetector::SetColorDistanceThreshold(int distance) {

    if (distance<0)
        distance=0;
    m_nMinDist= distance;
}

// Gets the color distance threshold
int CColorDetector::GetColorDistanceThreshold() const {

    return m_nMinDist;
}

// Sets the color to be detected
// given in BGR color space
void CColorDetector::SetTargetColor(uchar blue, uchar green, uchar red) 
{
    // BGR order
    m_iTarget = cv::Vec3b(blue, green, red);

    if (m_bUseLab) {
        // Temporary 1-pixel image
        cv::Mat tmp(1, 1, CV_8UC3);
        tmp.at<cv::Vec3b>(0, 0) = cv::Vec3b(blue, green, red);

        // Converting the target to Lab color space
        cv::cvtColor(tmp, tmp, CV_BGR2Lab);

        m_iTarget = tmp.at<cv::Vec3b>(0, 0);
    }
}

// Sets the color to be detected
void CColorDetector::SetTargetColor(cv::Vec3b color) 
{
    m_iTarget= color;
}

// Gets the color to be detected
cv::Vec3b CColorDetector::GetTargetColor() const 
{
    return m_iTarget;
}


cv::Mat CColorDetector::Process(const cv::Mat &image) 
{
    cv::Mat output;
    // compute absolute difference with target color
    cv::absdiff(image,cv::Scalar(m_iTarget),output);
    // split the channels into 3 images
    std::vector<cv::Mat> images;
    cv::split(output,images);
    // add the 3 channels (saturation might occurs here)
    output= images[0]+images[1]+images[2];
    // apply threshold
    cv::threshold(output, // same input/output image
                  output,
                  m_nMinDist, // threshold (must be < 256)
                  255, // max value
                  cv::THRESH_BINARY_INV); // thresholding mode
    return output;

}


/**                                                                             
  * @}                                                                         
  */	                                                                        
                                                                                
/**                                                                             
  * @}                                                                         
  */
                                                                                
/****************** (C) COPYRIGHT 2010-2018 R_W ******END OF FILE**************/

