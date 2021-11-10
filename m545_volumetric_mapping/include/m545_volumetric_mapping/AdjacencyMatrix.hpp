/*
 * AdjacencyMatrix.hpp
 *
 *  Created on: Nov 10, 2021
 *      Author: jelavice
 */
#pragma once

#include <vector>
#include <unordered_map>
#include <set>
#include <Eigen/Core>
#include "m545_volumetric_mapping/Voxel.hpp"
#include "m545_volumetric_mapping/typedefs.hpp"

namespace m545_mapping {
class AdjacencyMatrix {

public:
	AdjacencyMatrix() = default;
	using SubmapId = int64;

	void addNode(SubmapId id1, SubmapId id2);
	bool isAdjacent(SubmapId id1, SubmapId id2) const;

private:

	std::unordered_map<SubmapId, std::set<SubmapId>> adjacency_;


};
} //namespace m545_mapping
