#pragma once
#include <algorithm>
#include <array>
#include <deque>

#include "Mesh.h"


class CubeGenerator final
{
	struct Point final
	{
	public:
		std::array<size_t, 3> coordinates = { 0,0,0 };
		Point() = default;
		Point(std::array<size_t, 3> _coord) :coordinates(_coord) {};
		Point(size_t byte) { coordinates[byte] = 1; }
		Point operator+(Point& other)
		{
			return Point(std::array<size_t, 3>({ coordinates[0] + other.coordinates[0],
			coordinates[1] + other.coordinates[1],
			coordinates[2] + other.coordinates[2] }));
		}
	};

public:
	std::deque<Point> vertex;
	CubeGenerator(size_t edgeLength)
	{
		vertex = halfeCube();
		std::deque<Point> pointsInverted(vertex.size());
		std::transform(vertex.begin(), vertex.end(), pointsInverted.begin(), [](Point point)
			{
				for (size_t i = 0; i < 3; ++i)
					point.coordinates[i] = point.coordinates[i] ? 0 : 1;
				return point;
			});
		vertex.insert(vertex.end(), pointsInverted.begin(), pointsInverted.end());
	}

	std::deque<Point> halfeCube() const
	{
		std::deque<Point> points;
		for (size_t i = 0; i < 3; ++i)
			for (size_t j = 0; j < 3; ++j)
				points.emplace_back(i);
		points.push_back(points.front());
		points.pop_front();
		size_t j = points.size();
		for (size_t i = 0; i < j; ++++++i)
		{
			points[i] = Point();
			points.push_back(points[i + 1]);
			points.push_back(points[i + 2]);
			points.push_back(points[i + 1] + points[i + 2]);
		}
		return points;
	}
	static Mesh generateMesh(float span = 2)
	{
		Mesh mesh;
		CubeGenerator generator(span);
		for(int i=0;i<generator.vertex.size();++i)
		{
			std::array<float, 3> vertexPos = {
				generator.vertex[i].coordinates[0]* span,generator.vertex[i].coordinates[1] * span ,generator.vertex[i].coordinates[2] * span };
			mesh.vertices.push_back(Vertex{vertexPos, {1,0,0}, {1,1,1}});
			mesh.indices.push_back(i);
		}
		return mesh;
	}
};
