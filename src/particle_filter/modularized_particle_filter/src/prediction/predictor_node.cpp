#include "modularized_particle_filter/prediction/predictor.hpp"

#include <rclcpp/rclcpp.hpp>

#include <memory>

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<Predictor>());
  rclcpp::shutdown();
  return 0;
}
