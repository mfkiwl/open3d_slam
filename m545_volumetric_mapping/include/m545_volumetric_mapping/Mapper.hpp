/*
 * Mapper.hpp
 *
 *  Created on: Sep 26, 2021
 *      Author: jelavice
 */

#pragma once

#include <Eigen/Geometry>
#include <open3d/geometry/PointCloud.h>
#include <open3d/pipelines/registration/Registration.h>
#include <tf2_ros/transform_listener.h>
#include <tf2_ros/buffer.h>
#include <tf2/LinearMath/Transform.h>
#include "m545_volumetric_mapping/Parameters.hpp"
#include "m545_volumetric_mapping/time.hpp"
#include "m545_volumetric_mapping/croppers.hpp"


namespace m545_mapping{


class Mapper {

public:

	using PointCloud = open3d::geometry::PointCloud;

	Mapper();
	~Mapper()=default;

	void addRangeMeasurement(const PointCloud &cloud, const ros::Time &timestamp);
	const PointCloud &getMap() const;
	const PointCloud &getDenseMap() const;
	PointCloud *getMapPtr();
	PointCloud *getDenseMapPtr();
	void setParameters(const MapperParameters &p, const SpaceCarvingParameters &carvingParams, const m545_mapping::LocalMapParameters &localMapParams);
	bool isMatchingInProgress() const;
	bool isManipulatingMap() const;

	Eigen::Isometry3d getMapToOdom() const;
	Eigen::Isometry3d getMapToRangeSensor() const;
	void carve(const PointCloud &scan, const CroppingVolume &cropper,const SpaceCarvingParameters &params, PointCloud *map,Timer *timer) const;

	mutable PointCloud toRemove_;
	mutable PointCloud scanRef_;
	mutable PointCloud mapRef_;
private:

	void update(const MapperParameters &p);
	void estimateNormalsIfNeeded(PointCloud *pcl) const;
	void insertScanInMap(std::shared_ptr<PointCloud> wideCroppedCloud,
			const open3d::pipelines::registration::RegistrationResult &result, const PointCloud &rawScan);
  bool isMatchingInProgress_ = false;
  bool isManipulatingMap_ = false;

  PointCloud map_;
  PointCloud denseMap_;
  tf2_ros::Buffer tfBuffer_;
  tf2_ros::TransformListener tfListener_;
  ros::Time lastMeasurementTimestamp_;
  Eigen::Isometry3d mapToOdom_ = Eigen::Isometry3d::Identity();
  Eigen::Isometry3d odomToRangeSensorPrev_ = Eigen::Isometry3d::Identity();
  Eigen::Isometry3d mapToRangeSensor_ = Eigen::Isometry3d::Identity();
  MapperParameters params_;
  m545_mapping::LocalMapParameters localMapParams_;
  SpaceCarvingParameters carvingParameters_;
  open3d::pipelines::registration::ICPConvergenceCriteria icpCriteria_;
  std::mutex mapManipulationMutex_;
  Timer carvingTimer_;
  Timer carveDenseMapTimer_;

  std::shared_ptr<CroppingVolume> scanMatcherCropper_;
  std::shared_ptr<CroppingVolume> mapBuilderCropper_;
  std::shared_ptr<CroppingVolume> denseMapCropper_;


};

} /* namespace m545_mapping */
