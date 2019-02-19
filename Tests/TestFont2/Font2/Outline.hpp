#pragma once

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_BBOX_H
#include FT_OUTLINE_H
#include "Geometry.hpp"

namespace acid
{
#define FT_CHECK(r) do { FT_Error err = (r); assert(!err); } while (0)

#define FD_OUTLINE_MAX_POINTS (255 * 2)

	struct WIPCell
	{
		Rect bbox;
		uint32_t value;
		uint32_t from;
		uint32_t to;
		uint32_t startLength;
	};

	struct ContourRange
	{
		uint32_t begin, end;
	};

	struct Outline
	{
		Rect bbox;

		Vector2 *points;
		uint32_t pointCount;
		uint32_t pointCapacity;

		ContourRange *contours;
		uint32_t contourCount;
		uint32_t contourCapacity;

		uint32_t *cells;
		uint32_t cellCountX;
		uint32_t cellCountY;

		uint32_t cornerFixBegin;
	};

	struct PointU16
	{
		uint16_t x, y;
	};

	// private
	static void AddOutlinePoint(Outline *o, const Vector2 &point);
	static void AddOutlineContour(Outline *o, const ContourRange &range);
	static void OutlineAddOddPoint(Outline *o);
	static void ConvertPoint(const FT_Vector *v, Vector2 &out);
	static int32_t MoveToFunc(const FT_Vector *to, Outline *o);
	static int32_t LineToFunc(const FT_Vector *to, Outline *o);
	static int32_t ConicToFunc(const FT_Vector *control, const FT_Vector *to, Outline *o);
	static int32_t CubicToFunc(const FT_Vector *control1, const FT_Vector *control2, const FT_Vector *to, Outline *o);

	void OutlineConvert(FT_Outline *outline, Outline *o, char c);

	void OutlineDecompose(FT_Outline *outline, Outline *o);

	// private
	static uint32_t CellAddRange(uint32_t cell, uint32_t from, uint32_t to);
	static bool IsCellFilled(const Outline *o, const Rect &bbox);
	static bool WipcellAddBezier(const Outline *o, const Outline *u, const uint32_t &i, const uint32_t &j, const uint32_t &contourIndex, WIPCell &cell);
	static bool WipcellFinishContour(const Outline *o, const Outline *u, const uint32_t &contourIndex, WIPCell &cell, uint32_t &maxStartLength);
	static bool ForEachWipcellAddBezier(const Outline *o, const Outline *u, const uint32_t &i, const uint32_t &j, const uint32_t &contourIndex, WIPCell *cells);
	static bool ForEachWipcellFinishContour(const Outline *o, const Outline *u, const uint32_t &contourIndex, WIPCell *cells,
		uint32_t &maxStartLength);
	static void CopyWipcellValues(Outline *u, const WIPCell *cells);
	static void InitWipcells(const Outline *o, WIPCell *cells);
	static uint32_t OutlineAddFilledLine(Outline *o);
	static uint32_t MakeCellFromSingleEdge(const uint32_t &e);
	static void SetFilledCells(const Outline *u, WIPCell *cells, const uint32_t &filledCell);
	static bool TryToFitInCellCount(Outline *o);
	static uint32_t Uint32ToPow2(uint32_t v);

	void OutlineMakeCells(Outline *o);

	void OutlineSubdivide(Outline *o);

//	void OutlineFixCorners(Outline *o);

	static void OutlineFixThinLines(Outline *o); // private

	void OutlineDestroy(Outline *o);

	void OutlineCbox(Outline *o, Rect *cbox);

	static uint16_t GenU16Value(const float &x, const float &min, const float &max); // private

	void OutlineU16Points(Outline *o, Rect *cbox, PointU16 *pout);

	template<typename T>
	static void DynArrayGrow(T **data, uint32_t *capacity, size_t element_size)
	{
		*capacity = *capacity ? *capacity * 2 : 8;
		T *newData = static_cast<T *>(realloc(*data, *capacity * element_size));
		*data = newData;
	}
}
