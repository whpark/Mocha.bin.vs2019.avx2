#pragma once

#include "AFX_EXT_XMU.h"

// MatHelper

#include "Coordinate.hpp"

#include "misc/TString.h"

#pragma pack(push, 8)


//-----------------------------------------------------------------------------
// Mat Load/Store
template < class ARCHIVE >
BOOL LoadMat(ARCHIVE& ar, cv::Mat& mat) {
	DStringA str;
	ar >> str;
	if (str != "mat")
		return FALSE;

	int rows, cols, type;
	ar >> rows >> cols >> type;
	if ( (rows < 0) || (cols < 0) )
		return FALSE;
	if ( (rows == 0) || (cols == 0) ) {
		mat.release();
		return TRUE;
	}
	mat = cv::Mat::zeros(rows, cols, type);
	if (mat.empty())
		return FALSE;
	for (int i = 0; i < rows; i++) {
		ar.Read(mat.ptr(i), (UINT)(mat.cols*mat.elemSize()));	// step -> (cols*elemSize())
	}
	return TRUE;
}
template < class ARCHIVE >
BOOL StoreMat(ARCHIVE& ar, const cv::Mat& mat) {
	DStringA str("mat");
	ar << str;

	ar << mat.rows;
	ar << mat.cols;
	ar << mat.type();
	for (int i = 0; i < mat.rows; i++) {
		ar.Write(mat.ptr(i), (UINT)(mat.cols*mat.elemSize()));	// step -> (cols*elemSize())
	}

	return TRUE;
}
template < class FILE >
BOOL ReadMat(FILE& file, cv::Mat& mat) {
#define _READ_FILE(var) if ( file.Read(&var, sizeof(var)) != sizeof(var) ) break;
	do {
		int rows, cols, type;
		// read sign
		{
			BYTE buf[3];
			BYTE b = (BYTE)sizeof(buf);
			_READ_FILE(b);
			if (b != sizeof(buf))
				break;
			_READ_FILE(buf);
			if (memcmp(buf, "mat", b) != 0)
				break;
		}

		_READ_FILE(rows);
		_READ_FILE(cols);
		_READ_FILE(type);

		if ( (rows < 0) || (cols < 0) )
			break;
		if ( (rows == 0) || (cols == 0) ) {
			mat.release();
			return TRUE;
		}

		mat = cv::Mat::zeros(rows, cols, type);
		if (mat.empty())
			break;

		for (int i = 0; i < rows; i++)
			file.Read(mat.ptr(i), (UINT)(mat.cols*mat.elemSize()));	// step -> (cols*elemSize())

	} while (false);
#undef _READ_FILE
	return FALSE;
}
template < class FILE >
BOOL SaveMat(FILE& file, const cv::Mat& mat) {
	BYTE buf[4] = { 3, 'm', 'a', 't' };
	file.Write(buf, sizeof(buf));
	file.Write(&mat.rows, sizeof(int));
	file.Write(&mat.cols, sizeof(int));
	int type = mat.type();
	file.Write(&type, sizeof(type));
	for (int i = 0; i < mat.rows; i++) {
		file.Write(mat.ptr(i), (UINT)(mat.cols*mat.elemSize()));	// step -> (cols*elemSize())
	}

	return TRUE;
}


//-----------------------------------------------------------------------------
// 외곽선 글자
template <class TSTR >
void putTextC(cv::InputOutputArray img, TSTR& psz, cv::Point org,
			  int fontFace, double fontScale, cv::Scalar color,
			  int thickness = 1, int lineType = 8,
			  bool bottomLeftOrigin = false, bool bOutline = true)
{
	CStringA str(psz);
	cv::Size size = cv::getTextSize((LPCSTR)str, fontFace, fontScale, thickness, NULL);

	if (bOutline) {
		cv::Scalar crBkgnd;
		crBkgnd = cv::Scalar(255, 255, 255) - color;
		//if (crBkgnd == Scalar(0, 0, 0))
		//	crBkgnd = Scalar(1, 1, 1);
		int iShift = _max(1, thickness/2);
		cv::putText(img, (LPCSTR)str, CPoint2i(org)-CSize2i(size)/2+CPoint2i(0, iShift), fontFace, fontScale, crBkgnd, thickness, lineType, bottomLeftOrigin);
		cv::putText(img, (LPCSTR)str, CPoint2i(org)-CSize2i(size)/2+CPoint2i(iShift, 0), fontFace, fontScale, crBkgnd, thickness, lineType, bottomLeftOrigin);
		cv::putText(img, (LPCSTR)str, CPoint2i(org)-CSize2i(size)/2+CPoint2i(-iShift, 0), fontFace, fontScale, crBkgnd, thickness, lineType, bottomLeftOrigin);
		cv::putText(img, (LPCSTR)str, CPoint2i(org)-CSize2i(size)/2+CPoint2i(0, -iShift), fontFace, fontScale, crBkgnd, thickness, lineType, bottomLeftOrigin);
	}

	cv::putText(img, (LPCSTR)str, CPoint2i(org)-CSize2i(size)/2, fontFace, fontScale, color, thickness, lineType, bottomLeftOrigin);
}


//-----------------------------------------------------------------------------
// Mat 
BOOL AFX_EXT_CLASS_XMU UseGPU(BOOL bUse);
BOOL AFX_EXT_CLASS_XMU IsGPUEnabled();
BOOL AFX_EXT_CLASS_XMU ConvertColor(const cv::Mat& imgSrc, cv::Mat& imgDest, int eCode);
BOOL AFX_EXT_CLASS_XMU ResizeImage(const cv::Mat& imgSrc, cv::Mat& imgDest, double dScale, int eInterpolation = cv::INTER_LINEAR/*, int eScaleDownMethod = cv::INTER_AREA*/);
BOOL AFX_EXT_CLASS_XMU MatchTemplate(const cv::Mat& img, const cv::Mat& imgTempl, cv::Mat& matResult, int method);
BOOL AFX_EXT_CLASS_XMU MatchTemplate(const cv::Mat& img, const cv::Mat& imgTempl, int method, CPoint2d& ptBest, double& dMinMax, double& dRate, double dScale = 0.0, int eInterpolation = cv::INTER_LINEAR);

//-----------------------------------------------------------------------------
// Mat to DC
BOOL AFX_EXT_CLASS_XMU CalcViewPosition(const cv::Size& sizeView, const CRect& rectView, CRect& rectImage/* out */, CRect& rectDst/* out */);	// image size -> display rect and source rect
BOOL AFX_EXT_CLASS_XMU MatToDC            (const cv::Mat& img, const cv::Size& sizeView, CDC& dc, const CRect& rect, CBitmap const& mask = CBitmap());	// mask : monochrome bitmap, background of img value must be zero.
BOOL AFX_EXT_CLASS_XMU MatToDCTransparent (const cv::Mat& img, const cv::Size& sizeView, CDC& dc, const CRect& rect, COLORREF crTransparent);
BOOL AFX_EXT_CLASS_XMU MatToDCAlphaBlend  (const cv::Mat& img, const cv::Size& sizeView, CDC& dc, const CRect& rect, BLENDFUNCTION blend);
BOOL AFX_EXT_CLASS_XMU MatToMatTransparent(const cv::Mat& imgSource, cv::Mat& imgTarget, const cv::Mat& matMask);
BOOL AFX_EXT_CLASS_XMU MatToMatTransparent(const cv::Mat& imgSource, cv::Mat& imgTarget, const cv::Scalar& crTransparent);

#pragma pack(pop)
