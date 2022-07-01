#pragma once

#include "AFX_EXT_SHAPE.h"

#include "Shape.h"
#include "SCanvas.h"

#ifdef _USE_DXFLIB
// dxflib
#pragma warning(push)
#pragma warning(disable: 4996)
#include "dxflib/dl_writer_ascii.h"
#include "dxflib/dl_creationinterface.h"
#pragma warning(pop)
#endif

//#include "misc/SimpleLog.h"

//-----------------------------------------------------------------------------
// DXFLoader
class CDXFProgressThread;
class IDXFLoader {
private:
	static const COLORREF s_dxfColors[256];

public:
	IDXFLoader();
	virtual ~IDXFLoader();

protected:
	BOOL m_bShowProgress;
public:
	virtual BOOL Load(LPCTSTR pszFile, TRefList<CShapeCollection>& shapeCollections, TRectD& rectBound, BOOL bShowProgress = FALSE) = NULL;
	virtual BOOL Save(LPCTSTR pszPath, const TRefList<CShapeCollection>& shapeCollections, BOOL bShowProgress = FALSE) = NULL;

public:
	static COLORREF GetColor(BYTE iCR);
	static BYTE GetApproxColorIndex(COLORREF cr);

protected:
	CDXFProgressThread* m_pDXFThread;
	virtual void OnProgress(int64_t iCurrentPos, int64_t nTotalLength) {}
};


#ifdef _USE_DXFLIB
//-----------------------------------------------------------------------------
// DXFLoaderDefault
class AFX_EXT_CLASS_SHAPE CDXFLoader : private DL_CreationInterface, public IDXFLoader {
	TRefList<CShapeCollection>* m_pCollections;
	CShapeCollection* m_pCurrentCollection;
	CShapeObject* m_pCurrentObject;
	TRectD m_rectBound;

	TList<CShapeGroup> m_blocks;
	CShapeGroup* m_pCurrentBlock;
	//CSimpleLog m_log;

public:
	CDXFLoader();
	virtual ~CDXFLoader();

public:
	virtual BOOL Load(LPCTSTR pszFile, TRefList<CShapeCollection>& shapeCollections, TRectD& rectBound, BOOL bShowProgress = FALSE);
	virtual BOOL Save(LPCTSTR pszPath, const TRefList<CShapeCollection>& shapeCollections, BOOL bShowProgress = FALSE);

protected:
	virtual void processCodeValuePair(unsigned int, char*) {}
	virtual void addLayer(const DL_LayerData& data);
	virtual void addBlock(const DL_BlockData& data);
	virtual void endBlock();
	virtual void addPoint(const DL_PointData& data);
	virtual void addLine(const DL_LineData& data);
	virtual void addArc(const DL_ArcData& data);
	virtual void addCircle(const DL_CircleData& data);
	virtual void addEllipse(const DL_EllipseData& data);

	virtual void addPolyline(const DL_PolylineData& data);
	virtual void addVertex(const DL_VertexData& data);

	virtual void addSpline(const DL_SplineData& data);
	virtual void addControlPoint(const DL_ControlPointData& data);
	virtual void addKnot(const DL_KnotData& data);

	virtual void addInsert(const DL_InsertData& data);

	virtual void addMText(const DL_MTextData& data);
	virtual void addMTextChunk(const char* pszChunk) {}
	virtual void addText(const DL_TextData& data);

	virtual void addDimAlign(const DL_DimensionData& dataD, const DL_DimAlignedData& data) {}
	virtual void addDimLinear(const DL_DimensionData& dataD, const DL_DimLinearData& data) {}
	virtual void addDimRadial(const DL_DimensionData& dataD, const DL_DimRadialData& data) {}
	virtual void addDimDiametric(const DL_DimensionData& dataD, const DL_DimDiametricData& data) {}
	virtual void addDimAngular(const DL_DimensionData& dataD, const DL_DimAngularData& data) {}
	virtual void addDimAngular3P(const DL_DimensionData& dataD, const DL_DimAngular3PData& data) {}
	virtual void addDimOrdinate(const DL_DimensionData& dataD, const DL_DimOrdinateData& data) {}
	virtual void addLeader(const DL_LeaderData& data) {}
	virtual void addLeaderVertex(const DL_LeaderVertexData& data) {}

	virtual void addHatch(const DL_HatchData& data) {}

	virtual void addTrace(const DL_TraceData& data) {}
	virtual void add3dFace(const DL_3dFaceData& data) {}
	virtual void addSolid(const DL_SolidData& data) {}

	virtual void addImage(const DL_ImageData& data) {}
	virtual void linkImage(const DL_ImageDefData& data) {}
	virtual void addHatchLoop(const DL_HatchLoopData& data) {}
	virtual void addHatchEdge(const DL_HatchEdgeData& data) {}
	virtual void endEntity() {}
	virtual void addComment(const char* comment) {}
	virtual void setVariableVector(const char*, double, double, double, int) {}
	virtual void setVariableString(const char*, const char*, int) {}
	virtual void setVariableInt(const char*, int, int) {}
	virtual void setVariableDouble(const char*, double, int) {}
	virtual void endSequence() {}

	virtual void OnProgress(int64_t iCurrentPos, int64_t nTotalLength);

protected:
	COLORREF GetColor();
	CShapeCollection* GetCurrentCollection();
public:
	COLORREF GetColor(const DL_Attributes& attr);
};
#endif

//-----------------------------------------------------------------------------
// DXFLoaderDime
class dimeState;
class dimeEntity;
class dimeLayer;
class AFX_EXT_CLASS_SHAPE CDXFLoaderDime : public IDXFLoader {
protected:
	BOOL m_bStrictToLayerName;
public:
	CDXFLoaderDime(BOOL bStrictToLayerName = TRUE);
	virtual ~CDXFLoaderDime();

public:
	BOOL Load(LPCTSTR pszFile, TRefList<CShapeCollection>& shapeCollections, TRectD& rectBound, BOOL bShowProgress = FALSE);
	BOOL Save(LPCTSTR pszPath, const TRefList<CShapeCollection>& shapeCollections, BOOL bShowProgress = FALSE);

	COLORREF GetColor(const dimeState* pState, const dimeEntity* pEntity);
	COLORREF GetColor(const dimeLayer* pLayer);

	BOOL IsStrictToLayerName() const { return m_bStrictToLayerName; }
	BOOL SetStrictToLayerName(BOOL bStrictToLayerName) { return m_bStrictToLayerName = bStrictToLayerName; }

protected:
	TList<CShapeGroup> m_blocks;
	CShapeGroup* m_pCurrentBlock;

	static bool dimeCallbackFunc(const dimeState* const, dimeEntity *, void *);
	bool dimeCallbackFunc(TRefList<CShapeCollection>* pShapeCollections, const dimeState* const, dimeEntity *);
protected:
	int m_nItem;
	int m_iItemPos;
	static int dimePosCallbackFunc(float, void *);
	int dimePosCallbackFunc(float rate);

};
