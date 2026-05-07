#include "FrameHeader.h"

namespace
{
	static void assertMath(bool condition, const char* message)
	{
		if (!condition)
		{
			ERROR(string("MathUtilityDeepTest failed: ") + message);
		}
	}

	static void testScalarMath()
	{
		assertMath(getMin(3, 5) == 3, "min");
		assertMath(getMax(3, 5) == 5, "max");
		assertMath(isZero(0.0f), "zero float");
		assertMath(isZero(0.0), "zero double");
		assertMath(isEqual(1.0f, 1.0f + 0.000001f), "equal float");
		assertMath(indexToX(17, 5) == 2, "index to x");
		assertMath(indexToY(17, 5) == 3, "index to y");
		assertMath(intPosToIndex(2, 3, 5) == 17, "pos to index");
		assertMath(isEven(8), "is even");
		assertMath(!isEven(9), "is odd");
		assertMath(isPow2(8), "pow2");
		assertMath(!isPow2(7), "not pow2");
		assertMath(divide(10, 2) == 5.0f, "divide int");
		assertMath(divide(10, 0) == 0.0f, "divide zero");
		assertMath(generateBatchCount(10ull, 3) == 4, "batch count");
		assertMath(generateBatchSize(10ull, 3, 0) == 3, "batch size 0");
		assertMath(pow2(5) == 32, "pow2 func");
		assertMath(pow10(3) == 1000, "pow10");
		assertMath(inversePow10(2) > 0.009f && inversePow10(2) < 0.011f, "inverse pow10");
		assertMath(ceiling(1.2f) == 2, "ceiling float");
		assertMath(floor_(-1.2f) == -2, "floor float");
		assertMath(roundDouble(2.6) == 3, "round double");
		assertMath(round_(-2.6f) == -3, "round float");
		assertMath(step(5.0f, 5.0f) == 1, "step");
		assertMath(sign(-5) == -1, "sign");
		assertMath(frac(3.75f) > 0.74f && frac(3.75f) < 0.76f, "frac");
		assertMath(fmod(7.5f, 2.0f) > 1.49f && fmod(7.5f, 2.0f) < 1.51f, "fmod");
	}

	static void testClampsAndRanges()
	{
		int value = 10;
		clampRef(value, 0, 5);
		assertMath(value == 5, "clampRef");
		assertMath(clamp(10, 0, 5) == 5, "clamp");
		assertMath(clampMin(-1, 3) == 3, "clampMin");
		assertMath(clampZero(-1) == 0, "clampZero");
		assertMath(clampMax(10, 5) == 5, "clampMax");
		float angle = 7.0f;
		clampRadian360(angle);
		assertMath(angle >= 0.0f && angle <= TWO_PI_RADIAN, "clamp radian 360");
		angle = 200.0f;
		clampDegree180(angle);
		assertMath(angle <= 180.0f, "clamp degree 180");
		assertMath(inFixedRange(5, 1, 5), "fixed range");
		assertMath(inRange(5, 10, 1), "range int");
		assertMath(inRange(Vector2(1, 2), Vector2(0, 0), Vector2(2, 3)), "range vec2");
		assertMath(inRange(Vector3(1, 2, 3), Vector3(0, 0, 0), Vector3(2, 3, 4)), "range vec3");
	}

	static void testVectorMath()
	{
		Vector2 v2(3.0f, 4.0f);
		Vector3 v3(1.0f, 2.0f, 2.0f);
		assertMath(approxEqualFloat(getSquaredLength(v2), 25.0f), "squared length v2");
		assertMath(approxEqualFloat(getLength(v2), 5.0f), "length v2");
		assertMath(approxEqualFloat(getLength(v3), 3.0f), "length v3");
		assertMath(dot(Vector2(1, 2), Vector2(3, 4)) == 11.0f, "dot v2");
		assertMath(dot(Vector3(1, 2, 3), Vector3(4, 5, 6)) == 32.0f, "dot v3");
		Vector3 crossValue = cross(Vector3(1, 0, 0), Vector3(0, 1, 0));
		assertMath(approxEqualFloat(crossValue.z, 1.0f), "cross");
		Vector2 normalized2 = normalize(Vector2(3, 4));
		assertMath(approxEqualFloat(normalized2.x, 0.6f, 0.01f), "normalize v2");
		Vector3 normalized3 = normalize(Vector3(0, 3, 4));
		assertMath(approxEqualFloat(normalized3.y, 0.6f, 0.01f), "normalize v3");
		assertMath(approxEqualFloat(toDegree(MATH_PI), 180.0f, 0.01f), "degree");
		assertMath(approxEqualFloat(toRadian(180.0f), MATH_PI, 0.01f), "radian");
		assertMath(lengthLess(Vector2(1, 1), 2.0f), "length less");
		assertMath(lengthGreater(Vector2(3, 4), 4.0f), "length greater");
		assertMath(approxEqualFloat(getAngleBetweenVector(Vector2(1, 0), Vector2(0, 1)), HALF_PI_RADIAN, 0.01f), "angle vec2");
		assertMath(approxEqualFloat(getAngleBetweenVector(Vector3(1, 0, 0), Vector3(0, 0, 1)), HALF_PI_RADIAN, 0.01f), "angle vec3");
	}

	static void testAnglesAndRotation()
	{
		float angle = 3.5f;
		clampRadian180(angle);
		assertMath(angle <= MATH_PI && angle >= -MATH_PI, "clamp radian 180");
		float degree = 450.0f;
		clampDegree360(degree);
		assertMath(degree >= 0.0f && degree <= 360.0f, "clamp degree 360");
		Vector3 angleVec = getVectorFromAngle(0.0f);
		assertMath(approxEqualFloat(angleVec.x, 0.0f, 0.01f), "vector angle x");
		assertMath(approxEqualFloat(angleVec.z, 1.0f, 0.01f), "vector angle z");
		Vector3 rotated = rotateVector3AroundY(Vector3(1, 0, 0), HALF_PI_RADIAN);
		assertMath(fabs(rotated.x) < 0.05f, "rotate x");
		assertMath(fabs(rotated.z) > 0.9f, "rotate z");
	}

	static int compareIntAsc(const int& a, const int& b)
	{
		return a - b;
	}

	static void testSortingAndSearch()
	{
		Vector<int> values;
		values.add(5);
		values.add(1);
		values.add(9);
		values.add(3);
		quickSort(values);
		assertMath(values[0] == 1 && values[3] == 9, "quickSort vector");
		quickSort(values, false);
		assertMath(values[0] == 9 && values[3] == 1, "quickSort desc");
		quickSort(values, compareIntAsc, true);
		assertMath(values[0] == 1 && values[3] == 9, "quickSort compare");
		quickSortNonRecursive(values, false);
		assertMath(values[0] == 9 && values[3] == 1, "quickSort non recursive");

		ArrayList<8, int> arr;
		arr.add(4);
		arr.add(2);
		arr.add(8);
		arr.add(6);
		quickSort(arr);
		assertMath(arr[0] == 2 && arr[3] == 8, "arraylist sort");

		Vector<int> searchList;
		searchList.add(1);
		searchList.add(3);
		searchList.add(5);
		searchList.add(7);
		assertMath(binarySearch(searchList, 0, searchList.size() - 1, 5), "binary search hit");
		assertMath(!binarySearch(searchList, 0, searchList.size() - 1, 6), "binary search miss");

		Vector<float> dist;
		dist.add(0.0f);
		dist.add(5.0f);
		dist.add(10.0f);
		dist.add(20.0f);
		assertMath(findPointIndex(dist, 0.0f, 0, 3) == 0, "find point 0");
		assertMath(findPointIndex(dist, 7.0f, 0, 3) == 1, "find point 7");
		assertMath(findPointIndex(dist, 12.0f, 0, 3) == 2, "find point 12");
		assertMath(findPointIndex(dist, 20.0f, 0, 3) == 3, "find point end");
	}

	static void testExpressionsAndFormats()
	{
		assertMath(calculateInt("1+2*3") == 7, "calc int");
		assertMath(calculateInt("(1+2)*3") == 9, "calc int bracket");
		assertMath(approxEqualFloat(calculateFloat("1.5+2.5*2.0"), 6.5f, 0.01f), "calc float");
		assertMath(approxEqualFloat(calculateFloat("(1.5+2.5)*2.0"), 8.0f, 0.01f), "calc float bracket");
	}

	static void testGeometry()
	{
		Vector2 intersect2;
		assertMath(intersectLine2(Line2D(Vector2(0, 0), Vector2(10, 10)), Line2D(Vector2(0, 10), Vector2(10, 0)), intersect2), "line2");
		assertMath(approxEqualFloat(intersect2.x, 5.0f, 0.01f), "line2 x");
		assertMath(approxEqualFloat(intersect2.y, 5.0f, 0.01f), "line2 y");

		Vector2 sectionIntersect;
		assertMath(intersectLineSection(Line2D(Vector2(0, 0), Vector2(10, 10)), Line2D(Vector2(0, 10), Vector2(10, 0)), sectionIntersect, true), "section");
		assertMath(approxEqualFloat(sectionIntersect.x, 5.0f, 0.01f), "section x");

		TriangleIntersect triIntersect;
		assertMath(intersectLineTriangle(Line2(Vector2(-1, 1), Vector2(10, 1)), Triangle2(Vector2(0, 0), Vector2(10, 0), Vector2(0, 10)), triIntersect, true), "triangle");
		assertMath(approxEqualFloat(triIntersect.mIntersectPoint.x, 0.0f, 0.1f) || approxEqualFloat(triIntersect.mIntersectPoint.y, 1.0f, 0.1f), "triangle point");

		assertMath(approxEqualFloat(getDistanceToLine(Vector2(0, 1), Line2D(Vector2(-1, 0), Vector2(1, 0))), 1.0f, 0.01f), "distance");
		Vector2 projection = getProjection(Vector2(2, 0), Vector2(4, 0));
		assertMath(approxEqualFloat(projection.x, 2.0f, 0.01f), "projection x");

		Vector3 out3;
		assertMath(intersectLine3IgnoreY(Line3(Vector3(0, 0, 0), Vector3(10, 0, 10)), Line3(Vector3(0, 0, 10), Vector3(10, 0, 0)), out3), "line3 ignore y");
		assertMath(intersectIgnoreY(Line3(Vector3(-1, 0, 1), Vector3(11, 0, 1)), Rect3(Vector3(0, 0, 0), Vector3(10, 10, 10)), nullptr, true), "rect3 ignore y");
		assertMath(intersectLineAABB(Line2(Vector2(-1, 5), Vector2(11, 5)), Rect(0, 0, 10, 10), nullptr), "aabb");
		assertMath(intersectEdge(Line2D(Vector2(-1, 5), Vector2(11, 5)), Rect(0, 0, 10, 10), nullptr, true), "edge");
	}
}

void MathUtilityDeepTest::test()
{
	testScalarMath();
	testClampsAndRanges();
	testVectorMath();
	testAnglesAndRotation();
	testSortingAndSearch();
	testExpressionsAndFormats();
	testGeometry();
}
