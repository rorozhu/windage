/* ========================================================================
 * PROJECT: windage Library
 * ========================================================================
 * This work is based on the original windage Library developed by
 *   Woonhyuk Baek
 *   Woontack Woo
 *   U-VR Lab, GIST of Gwangju in Korea.
 *   http://windage.googlecode.com/
 *   http://uvr.gist.ac.kr/
 *
 * Copyright of the derived and new portions of this work
 *     (C) 2009 GIST U-VR Lab.
 *
 * This framework is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This framework is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this framework; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * For further information please contact 
 *   Woonhyuk Baek
 *   <windage@live.com>
 *   GIST U-VR Lab.
 *   Department of Information and Communication
 *   Gwangju Institute of Science and Technology
 *   1, Oryong-dong, Buk-gu, Gwangju
 *   South Korea
 * ========================================================================
 ** @author   Woonhyuk Baek
 * ======================================================================== */

#ifndef _HOMOGRAPHY_ESM_H_
#define _HOMOGRAPHY_ESM_H_

#include <vector>
#include <cv.h>

#include "wMatrix.h"
#include "TemplateMinimization.h"

namespace windage
{
	class HomographyESM : public TemplateMinimization
	{
	private:
		double HOMOGRAPHY_DELTA;
		static const int HOMOGRAPHY_COUNT = 9;

		int q;
		int p;

		std::vector<Vector2> dI;
		std::vector<Vector2> dwI;
		std::vector<std::vector<Vector2>> dwx;

		std::vector<double> se;
		std::vector<double> sxc;

		CvMat* JacobianSum;
		CvMat* JacobianSumT;
		CvMat* Jacobian;
		CvMat* JacobianInvers;
		CvMat* dS;
		CvMat* JacobianTdS;
		CvMat* dx;

	public:
		HomographyESM(int width=150, int height=150) : TemplateMinimization(width, height)
		{
			this->HOMOGRAPHY_DELTA = 5.0;

			this->q = (this->width/this->SAMPLING_STEP) * (this->height/this->SAMPLING_STEP) - (int)(2*this->DELTA) + 2;
			this->p = HOMOGRAPHY_COUNT - 1;

			JacobianSum		= cvCreateMat(q, p, CV_64F);
			JacobianSumT	= cvCreateMat(p, q, CV_64F);
			Jacobian		= cvCreateMat(p, p, CV_64F);
			JacobianInvers	= cvCreateMat(p, p, CV_64F);
			dS				= cvCreateMat(q, 1, CV_64F);
			JacobianTdS		= cvCreateMat(p, 1, CV_64F);
			dx				= cvCreateMat(p, 1, CV_64F);

			cvZero(JacobianSum);
			cvZero(JacobianSumT);
			cvZero(Jacobian);
		}
		~HomographyESM()
		{
			if(templateImage)	cvReleaseImage(&templateImage);
			if(samplingImage)	cvReleaseImage(&samplingImage);

			if(JacobianSum)		cvReleaseMat(&JacobianSum);
			if(JacobianSumT)	cvReleaseMat(&JacobianSumT);
			if(Jacobian)		cvReleaseMat(&Jacobian);
			if(JacobianInvers)	cvReleaseMat(&JacobianInvers);
			if(dS)				cvReleaseMat(&dS);
			if(JacobianTdS)		cvReleaseMat(&JacobianTdS);
			if(dx)				cvReleaseMat(&dx);
		}

		inline Matrix3 GetHomography(){return this->homography;};
		inline void SetInitialHomography(Matrix3 homography){this->homography = homography;};
		
		bool AttatchTemplateImage(IplImage* image);
		bool Initialize();
		double UpdateHomography(IplImage* image, double* delta = NULL);
	};
}

#endif