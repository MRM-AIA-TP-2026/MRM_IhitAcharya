#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/imu.hpp>
#include <sensor_msgs/msg/nav_sat_fix.hpp>
#include <sensor_msgs/msg/laser_scan.hpp>
#include <sensor_msgs/msg/image.hpp>

class SensorNode : public rclcpp::Node
{
public:
    SensorNode() : Node("sensor_node")
    {
        // IMU Publisher
        imu_publisher_ = this->create_publisher<sensor_msgs::msg::Imu>("/odom", 10);

        // GPS Publisher
        gps_publisher_ = this->create_publisher<sensor_msgs::msg::NavSatFix>("/gps/fix", 10);

        // Lidar Publisher
        lidar_publisher_ = this->create_publisher<sensor_msgs::msg::LaserScan>("/scan", 10);

        // Stereo Camera Publishers
        left_camera_publisher_ = this->create_publisher<sensor_msgs::msg::Image>("/cameraL", 10);
        right_camera_publisher_ = this->create_publisher<sensor_msgs::msg::Image>("/cameraR", 10);

        // Timers for periodic publishing
        imu_timer_ = this->create_wall_timer(std::chrono::milliseconds(100), std::bind(&SensorNode::publishImuData, this));
        gps_timer_ = this->create_wall_timer(std::chrono::milliseconds(1000), std::bind(&SensorNode::publishGpsData, this));
        lidar_timer_ = this->create_wall_timer(std::chrono::milliseconds(100), std::bind(&SensorNode::publishLidarData, this));
        camera_timer_ = this->create_wall_timer(std::chrono::milliseconds(100), std::bind(&SensorNode::publishStereoImages, this));
    }

private:
    // Publisher and timer for IMU data
    void publishImuData()
    {
        auto imu_msg = sensor_msgs::msg::Imu();
        imu_msg.header.stamp = this->get_clock()->now();
        imu_msg.header.frame_id = "imu_link";
        imu_msg.orientation.x = 0.0;
        imu_msg.orientation.y = 0.0;
        imu_msg.orientation.z = 0.0;
        imu_msg.orientation.w = 1.0;
        imu_publisher_->publish(imu_msg);
    }

    // Publisher and timer for GPS data
    void publishGpsData()
    {
        auto gps_msg = sensor_msgs::msg::NavSatFix();
        gps_msg.header.stamp = this->get_clock()->now();
        gps_msg.header.frame_id = "gps_link";
        gps_msg.latitude = 37.7749;  // Example latitude
        gps_msg.longitude = -122.4194; // Example longitude
        gps_msg.altitude = 0.0;
        gps_publisher_->publish(gps_msg);
    }

    // Publisher and timer for Lidar data
    void publishLidarData()
    {
        auto lidar_msg = sensor_msgs::msg::LaserScan();
        lidar_msg.header.stamp = this->get_clock()->now();
        lidar_msg.header.frame_id = "lidar_link";
        lidar_msg.angle_min = -1.57; // -90 degrees
        lidar_msg.angle_max = 1.57;  // 90 degrees
        lidar_msg.angle_increment = 0.01;
        lidar_msg.range_min = 0.1;
        lidar_msg.range_max = 10.0;

        int num_readings = (lidar_msg.angle_max - lidar_msg.angle_min) / lidar_msg.angle_increment;
        lidar_msg.ranges.resize(num_readings, 5.0);  // Example ranges
        lidar_publisher_->publish(lidar_msg);
    }

    // Publisher and timer for stereo camera data
    void publishStereoImages()
    {
        auto left_image = sensor_msgs::msg::Image();
        left_image.header.stamp = this->get_clock()->now();
        left_image.header.frame_id = "left_camera";
        left_image.width = 640;
        left_image.height = 480;
        left_image.encoding = "rgb8";
        left_image.step = left_image.width * 3;
        left_image.data.resize(left_image.step * left_image.height, 0); // Example black image

        auto right_image = sensor_msgs::msg::Image();
        right_image.header.stamp = this->get_clock()->now();
        right_image.header.frame_id = "right_camera";
        right_image.width = 640;
        right_image.height = 480;
        right_image.encoding = "rgb8";
        right_image.step = right_image.width * 3;
        right_image.data.resize(right_image.step * right_image.height, 0); // Example black image

        left_camera_publisher_->publish(left_image);
        right_camera_publisher_->publish(right_image);
    }

    // Publishers
    rclcpp::Publisher<sensor_msgs::msg::Imu>::SharedPtr imu_publisher_;
    rclcpp::Publisher<sensor_msgs::msg::NavSatFix>::SharedPtr gps_publisher_;
    rclcpp::Publisher<sensor_msgs::msg::LaserScan>::SharedPtr lidar_publisher_;
    rclcpp::Publisher<sensor_msgs::msg::Image>::SharedPtr left_camera_publisher_;
    rclcpp::Publisher<sensor_msgs::msg::Image>::SharedPtr right_camera_publisher_;

    // Timers
    rclcpp::TimerBase::SharedPtr imu_timer_;
    rclcpp::TimerBase::SharedPtr gps_timer_;
    rclcpp::TimerBase::SharedPtr lidar_timer_;
    rclcpp::TimerBase::SharedPtr camera_timer_;
};

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<SensorNode>());
    rclcpp::shutdown();
    return 0;
}