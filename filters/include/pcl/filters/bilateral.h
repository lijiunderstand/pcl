/*
 * Software License Agreement (BSD License)
 *
 *  Point Cloud Library (PCL) - www.pointclouds.org
 *  Copyright (c) 2010-2011, Willow Garage, Inc.
 *
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above
 *     copyright notice, this list of conditions and the following
 *     disclaimer in the documentation and/or other materials provided
 *     with the distribution.
 *   * Neither the name of Willow Garage, Inc. nor the names of its
 *     contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 *  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 *  COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 *  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 *  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 *  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 *  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 *  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 *
 */

#ifndef PCL_FILTERS_BILATERAL_H_
#define PCL_FILTERS_BILATERAL_H_

#include <pcl/filters/filter.h>
#include <pcl/kdtree/kdtree.h>

namespace pcl
{
  /** \brief A bilateral filter implementation for point cloud data. Uses the intensity data channel.
    * \note For more information please see 
    * <b>C. Tomasi and R. Manduchi. Bilateral Filtering for Gray and Color Images.
    * In Proceedings of the IEEE International Conference on Computer Vision,
    * 1998.</b>
    * \author Luca Penasa
    */
  template<typename PointT>
  class BilateralFilter : public Filter<PointT>
  {
    using Filter<PointT>::input_;
    using Filter<PointT>::indices_;
    typedef typename Filter<PointT>::PointCloud PointCloud;
    typedef typename pcl::KdTree<PointT>::Ptr KdTreePtr;

    public:
      /** \brief Constructor. 
        * Sets \ref sigma_s_ to 0 and \ref sigma_r_ to MAXDBL
        */
      BilateralFilter () : sigma_s_ (0), 
                           sigma_r_ (std::numeric_limits<double>::max ())
      {
      }


      /** \brief Filter the input data and store the results into output
        * \param[out] output the resultant point cloud message
        */
      void
      applyFilter (PointCloud &output);

      /** \brief Compute the intensity average for a single point
        * \param[in] pid the point index to compute the weight for
        * \param[in] indices the set of nearest neighor indices 
        * \param[in] distances the set of nearest neighbor distances
        * \return the intensity average at a given point index
        */
      double 
      computePointWeight (const int pid, const std::vector<int> &indices, const std::vector<float> &distances);

      /** \brief Set the half size of the Gaussian bilateral filter window.
        * \param[in] sigma_s the half size of the Gaussian bilateral filter window to use
        */
      inline void 
      setHalfSize (const double sigma_s)
      {
        sigma_s_ = sigma_s;
      }

      /** \brief Get the half size of the Gaussian bilateral filter window as set by the user. */
      double 
      getHalfSize ()
      {
        return (sigma_s_);
      }

      /** \brief Set the standard deviation parameter
        * \param[in] sigma_r the new standard deviation parameter
        */
      void
      setStdDev (const double sigma_r)
      {
        sigma_r_ = sigma_r;
      }

      /** \brief Get the value of the current standard deviation parameter of the bilateral filter. */
      double 
      getStdDev ()
      {
        return (sigma_r_);
      }

      /** \brief Provide a pointer to the search object.
        * \param[in] tree a pointer to the spatial search object.
        */
      void
      setSearchMethod (const KdTreePtr &tree)
      {
        tree_ = tree;
      }

    private:

      /** \brief The bilateral filter Gaussian distance kernel.
        * \param[in] x the spatial distance (distance or intensity)
        * \param[in] sigma standard deviation
        */
      inline double
      kernel (double x, double sigma)
      {
        return (exp (- (x*x)/(2*sigma*sigma)));
      }

      /** \brief The half size of the Gaussian bilateral filter window (e.g., spatial extents in Euclidean). */
      double sigma_s_;
      /** \brief The standard deviation of the bilateral filter (e.g., standard deviation in intensity). */
      double sigma_r_;

      /** \brief A pointer to the spatial search object. */
      KdTreePtr tree_;
  };
}

#endif // PCL_FILTERS_BILATERAL_H_
