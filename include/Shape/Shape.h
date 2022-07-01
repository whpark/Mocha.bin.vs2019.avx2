#pragma once

#include "AFX_EXT_SHAPE.h"

#include "ShapeObject.h"
#include "ShapeCollection.h"
#include "ShapeGroup.h"
#include "ShapeDot.h"
#include "ShapeVertex.h"
#include "ShapeLine.h"
#include "ShapePolyLine.h"
#include "ShapeSpline.h"
#include "ShapeCircle.h"
#include "ShapeArc.h"
#include "ShapeEllipse.h"
#include "ShapeComplex.h"
#include "ShapeText.h"
#include "ShapeDMTX.h"
#include "ShapeCrossLine.h"
#include "SplineToArc.h"

#define EXT_DXF _T(".dxf")
#define EXT_USHAPE _T(".ushape")

typedef TRefList<CShapeObject> CShapes;
typedef TRefList<CShapeCollection> CShapeDXF;

AFX_EXT_API_SHAPE BOOL LoadUShape(LPCTSTR pszFilePath, TRefList<CShapeCollection>& layers, CRect2d& rectBound);
AFX_EXT_API_SHAPE BOOL SaveUShape(LPCTSTR pszFilePath, const TRefList<CShapeCollection>& layers);
AFX_EXT_API_SHAPE BOOL SerializeUShape(CArchive& ar, TRefList<CShapeCollection>& layers, CRect2d& rectBound);
