#pragma once

#include "AFX_EXT_XMU.h"

// CMatView

#include "Coordinate.hpp"
#include "misc/xUtil.h"
#include "xMathUtil/CoordTrans.h"
#include "xMathUtil/CoordTransEx.h"

#pragma pack(push, 8)


//---------------------------------------------------------------------------------------------------------------------------------
// IMatImage
//
class IMatImage {
public:
	bool m_bUseGPU = true;
protected:
	int m_eScaleDownMethod = cv::INTER_AREA;
	int m_eScaleUpMethod = cv::INTER_LANCZOS4;

public:
	IMatImage() {};
	virtual ~IMatImage() {};

public:
	virtual bool empty() const					= NULL;
	virtual int depth() const					= NULL;
	virtual int type() const					= NULL;
	virtual int channels() const				= NULL;
	virtual int GetWidth() const				= NULL;
	virtual int GetHeight() const				= NULL;
	virtual cv::Size size() const { return cv::Size(GetWidth(), GetHeight()); }

	virtual cv::Mat GetROI(cv::Rect& rc) const { return GetResizedImage(rc, 0.0, -1, -1); }
	virtual cv::Mat GetResizedImage(cv::Rect& rc, double dScale, int eScaleDownMethod = -1, int eScaleUpMethod = -1) const = NULL;

	template < typename T > T& at(const cv::Point& pt) { cv::Rect rc(pt, cv::Size(1, 1)); return GetROI(rc).at<T>(0, 0); };

public:
	int GetScaleDownMethod() const { return m_eScaleDownMethod; }
	int GetScaleUpMethod() const { return m_eScaleUpMethod; }
	virtual void SetResizingMethod(int eScaleDownMethod = -1, int eScaleUpMethod = -1, bool bThumbnailInBkgnd = true) {
		if (eScaleUpMethod >= 0)
			m_eScaleUpMethod = eScaleUpMethod;
		if (eScaleDownMethod >= 0)
			m_eScaleDownMethod = eScaleDownMethod;
	}
};


//---------------------------------------------------------------------------------------------------------------------------------
// C2dMatArray : Thumbnail Cache
//
class AFX_EXT_CLASS_XMU C2dMatArray : public IMatImage {

public:
	typedef struct T_ITEM {
		cv::Mat img;
		mutable TCopyTransparent<std::recursive_mutex> mtxThumbnail;
		//mutable bool bTumbnail = false;
		std::vector<cv::Mat> thumbnails;
	} T_ITEM, *P_ITEM;
	typedef std::vector<std::pair<int, int>> T_THUMBNAIL_SIZES;
	typedef std::function<bool(CPoint2i&, const C2dMatArray::T_ITEM&)> F_NOTIFIER;

protected:
	std::mutex m_mtxSet;
	std::vector<T_ITEM> m_set;
	cv::Mat m_imgWhole;
	CSize2i m_sizeArray;
	CSize2i m_sizeImage;
protected:
	F_NOTIFIER m_funcNotifier;

public:
	C2dMatArray() : m_evtThumbnail(false, true) {
	}
	virtual ~C2dMatArray() {
		Destroy();
	}

	// Copies Matrix member only. NO ( Thumbnail Maker, Callback Function )
	C2dMatArray& operator = (const C2dMatArray& B);

	bool Create(const CSize2i& size);																						// 아무것도 없이 그냥 배열만 생성. 데이터 없음.
	bool Create(const cv::Mat& imgWhole, const CSize2i& sizePieceImage, const CSize2i& size);								// imgWhole 을 연결하여 생성.
	void Destroy();

	bool SetThumbnailMaker(const T_THUMBNAIL_SIZES& sizesThumbnail = { {1, 8}, {1, 16}, }, int nThreadThumbnailMaker = 0);	// Thumbnail Maker 가 동작중일때는 변경 안됨
	bool StartThumbnailMaker();
	bool StopThumbnailMaker();
	bool IsThumbnailMakerRunning() const { return m_threadsThumbnailWorker.size() ? true : false; }
	bool UpdateThumbnail(bool bThumbnailInBkgnd);
	bool UpdateThumbnail() { return UpdateThumbnail(IsThumbnailMakerRunning()); }

	const CSize2i& GetArraySize() const { return m_sizeArray; }
	bool SetPartialImage(const CPoint2i& pos, const cv::Mat& img, bool bCopy = false, bool bUpdateThumbnail = true, bool bThumbnailInBkgnd = true);

	void SetNotifier(F_NOTIFIER funcNotifier) { m_funcNotifier = funcNotifier; }
	void ResetNotifier()	{ m_funcNotifier = nullptr; }

public:
	bool empty() const		{ return (m_sizeImage.cx <= 0) || (m_sizeImage.cy <= 0); }
	int depth() const		{ return m_set.size() ? m_set[0].img.depth() : 0; }
	int type() const		{ return m_set.size() ? m_set[0].img.type() : 0; }
	int channels() const	{ return m_set.size() ? m_set[0].img.channels() : 0; }
	cv::Size size() const	{ return cv::Size(m_sizeImage.cx, m_sizeImage.cy); }
	int GetWidth() const	{ return m_sizeImage.cx; }
	int GetHeight() const	{ return m_sizeImage.cy; }

	void SetResizingMethod(int eScaleDownMethod = -1, int eScaleUpMethod = -1, bool bThumbnailInBkgnd = true);

public:
	CPoint2i GetPos(int iIndex) const;
	T_ITEM& GetItem(const CPoint2i& pos);
	const T_ITEM& GetItem(const CPoint2i& pos) const;

public:
	cv::Mat const& GetImage() const { return m_imgWhole; }
	cv::Mat GetMergedImage() const { cv::Rect rc(cv::Point(0, 0), size()); return GetROI(rc); }
	cv::Mat GetROI(cv::Rect& rc) const { return GetResizedImage(rc, 0.0, -1, -1); }
	cv::Mat GetResizedImage(cv::Rect& rc, double dScale, int eScaleDownMethod = -1, int eScaleUpMethod = -1) const;
	cv::Mat GetResizedImage(cv::Rect& rc, cv::Size size, int eScaleDownMethod = -1, int eScaleUpMethod = -1) const;

protected:
	CEvent m_evtThumbnail;
	bool m_bStopThumbnailMaker = false;
	T_THUMBNAIL_SIZES m_sizesThumbnail;
	int m_nThreadThumbnailMaker = 0;
	std::mutex m_mtxThumbnailWork;
	std::vector<std::unique_ptr<std::thread>> m_threadsThumbnailWorker;
	std::deque<std::unique_ptr<std::pair<CPoint2i, P_ITEM>>> m_dequeThumbnailWork;
	void ThumbnailMaker();
	bool MakeThumbnail(T_ITEM& item);

	bool Resize(cv::Mat& imgSrc, cv::Mat& imgDest, const cv::Size& size, int eResizingMethod);
};

#pragma pack(pop)
