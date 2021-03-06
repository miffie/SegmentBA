/**
* This file is part of SegmentBA.
*
* Copyright (C) 2017 Zhejiang University
* For more information see <https://github.com/ZJUCVG/SegmentBA>
* If you use this code, please cite the corresponding publications as 
* listed on the above website.
*
* SegmentBA is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* SegmentBA is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with SegmentBA. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef _CAMERA_ARRAY_CALIBRATION_PARAMETER_H_
#define _CAMERA_ARRAY_CALIBRATION_PARAMETER_H_

#include "IntrinsicMatrix.h"
#include "RigidTransformation.h"
#include "EssentialMatrix.h"
#include "FundamentalMatrix.h"

#define CAMERAS_NUMBER 2
typedef ubyte CameraIndex;

class CameraArrayCalibrationParameter
{
public:
	inline const IntrinsicMatrix& GetIntrinsicMatrix(const CameraIndex &iCam) const { return m_Ks[iCam]; }
	inline void SetIntrinsicMatrixes(const float &fx, const float &fy, const float &cx, const float &cy)
	{
		for(CameraIndex iCam = 0; iCam < CAMERAS_NUMBER; ++iCam)
			m_Ks[iCam].Set(fx, fy, cx, cy);
	}
	inline void SetIntrinsicMatrixes(const IntrinsicMatrix &K)
	{
		for(CameraIndex iCam = 0; iCam < CAMERAS_NUMBER; ++iCam)
			m_Ks[iCam] = K;
	}
	inline void SetRelativePose(const CameraIndex &iCam, const RigidTransformation3D &T) { m_Ts[iCam] = T; m_TsScaled[iCam] = T; }
	inline const RigidTransformation3D& GetRelativePose(const CameraIndex &iCam) const { return m_Ts[iCam]; }
	inline const RigidTransformation3D& GetRelativePoseScaled(const CameraIndex &iCam) const { return m_TsScaled[iCam]; }
	inline void ScaleRelativePoses(const float &s)
	{
		if(s == 1)
		{
			for(CameraIndex iCam = 0; iCam < CAMERAS_NUMBER; ++iCam)
			{
				m_TsScaled[iCam].tX() = m_Ts[iCam].tX();
				m_TsScaled[iCam].tY() = m_Ts[iCam].tY();
				m_TsScaled[iCam].tZ() = m_Ts[iCam].tZ();
			}
		}
		else
		{
			for(CameraIndex iCam = 0; iCam < CAMERAS_NUMBER; ++iCam)
			{
				m_TsScaled[iCam].tX() = m_Ts[iCam].tX() * s;
				m_TsScaled[iCam].tY() = m_Ts[iCam].tY() * s;
				m_TsScaled[iCam].tZ() = m_Ts[iCam].tZ() * s;
			}
		}
	}
	inline void SaveB(FILE *fp) const
	{
		for(CameraIndex iCam = 0; iCam < CAMERAS_NUMBER; ++iCam)
		{
			m_Ks[iCam].SaveB(fp);
			m_Ts[iCam].SaveB(fp);
		}
	}
	inline void LoadB(FILE *fp)
	{
		for(CameraIndex iCam = 0; iCam < CAMERAS_NUMBER; ++iCam)
		{
			m_Ks[iCam].LoadB(fp);
			m_Ts[iCam].LoadB(fp);
			m_TsScaled[iCam] = m_Ts[iCam];
		}
	}
	inline bool Load(const char *fileName)
	{
		FILE *fp = fopen(fileName, "r");
		if(!fp)
			return false;
		for(CameraIndex iCam = 0; iCam < CAMERAS_NUMBER; ++iCam)
		{
			m_Ks[iCam].Load(fp);
			m_Ts[iCam].Load(fp);
		}
		printf("[CameraArrayCalibrationParameter] Loaded \'%s\'\n", fileName);
		fclose(fp);
		return true;
	}
protected:
	IntrinsicMatrix m_Ks[CAMERAS_NUMBER];
	RigidTransformation3D m_Ts[CAMERAS_NUMBER], m_TsScaled[CAMERAS_NUMBER];
};

#endif