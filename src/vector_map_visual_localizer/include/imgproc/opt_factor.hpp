#pragma once
#include <eigen3/Eigen/Geometry>
#include <sophus/geometry.hpp>

#include <memory>

namespace imgproc::opt
{
class Vertex
{
public:
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW
  using Ptr = std::shared_ptr<Vertex>;

  Vertex(const Eigen::Quaternionf & qf, const Eigen::Vector3f & vp, const Sophus::SO3f & dR)
  : vp_(vp), dR_(dR), index(index_max++), q_(qf.coeffs().cast<double>())
  {
  }

  Vertex(const Vertex & v) : vp_(v.vp_), dR_(v.dR_), index(v.index), q_(v.q_) {}

  Sophus::SO3f so3f() const
  {
    Eigen::Quaternionf qf;
    qf.coeffs() = q_.cast<float>();
    return Sophus::SO3f(qf);
  }

  const Eigen::Vector3f vp_;
  const Sophus::SO3f dR_;
  const int index;
  Eigen::Vector4d q_;

private:
  static int index_max;
};

}  // namespace imgproc::opt