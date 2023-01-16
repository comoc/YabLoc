#include "corrector_manager/init_area.hpp"

#include <boost/geometry/geometry.hpp>

namespace pcdless
{

InitArea::InitArea(const sensor_msgs::msg::PointCloud2 & msg)
{
  pcl::PointCloud<pcl::PointXYZL>::Ptr points{new pcl::PointCloud<pcl::PointXYZL>()};
  pcl::fromROSMsg(msg, *points);

  if (points->empty()) return;

  polygon poly;

  std::optional<uint32_t> last_label = std::nullopt;
  for (const pcl::PointXYZL p : *points) {
    if (last_label) {
      if ((*last_label) != p.label) {
        if (last_label < 512)
          init_areas_.push_back(poly);
        else
          deinit_areas_.push_back(poly);

        poly.outer().clear();
      }
    }
    poly.outer().push_back(point(p.x, p.y));
    last_label = p.label;
  }

  if (last_label < 512)
    init_areas_.push_back(poly);
  else
    deinit_areas_.push_back(poly);
}

bool InitArea::is_inside(const Eigen::Vector3d & xyz, bool * init_area) const
{
  if (init_areas_.empty() && deinit_areas_.empty()) return false;

  const point query(xyz.x(), xyz.y());
  for (const polygon & poly : init_areas_) {
    if (boost::geometry::within(query, poly)) {
      *init_area = true;
      return true;
    }
  }
  for (const polygon & poly : deinit_areas_) {
    if (boost::geometry::within(query, poly)) {
      *init_area = false;
      return true;
    }
  }

  return false;
}

}  // namespace pcdless