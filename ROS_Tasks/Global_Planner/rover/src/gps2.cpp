#include <rclcpp/rclcpp.hpp>
#include <geometry_msgs/msg/twist.hpp>
#include <sensor_msgs/msg/nav_sat_fix.hpp>
#include <cmath>
#include <iostream>
#include <chrono>
#include <functional>
#include <memory>
#include <unistd.h>


using namespace std::chrono_literals;
long double woah,target_lat_, target_lon_, current_lat_,current_lon_,set_lat,set_lon,x_mov,y_mov,distance,bearing,permb,permd,origin_lat_ = 0.00000,origin_lon_ = 0.00000;
int n = 0,m = -1;
int q = 0;
int flag;
int temp;
class GpsNavigator : public rclcpp::Node {
public:
    GpsNavigator() : Node("gps")
    {if (m == -1)
        {std::cout << "Input target latitude and longitude:" << std::endl;
        std::cin >> target_lat_;
        std::cin >> target_lon_;
        m++;
        movement_pub_ = this->create_publisher<geometry_msgs::msg::Twist>("/cmd_vel", 10);
        gps_sub_ = this->create_subscription<sensor_msgs::msg::NavSatFix>("/gps", 10, std::bind(&GpsNavigator::gpsCallback, this, std::placeholders::_1));}
    
    }

    rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr movement_pub_;
    rclcpp::Subscription<sensor_msgs::msg::NavSatFix>::SharedPtr gps_sub_;
    rclcpp::TimerBase::SharedPtr timer_;
    rclcpp::TimerBase::SharedPtr timer_func;

    void gpsCallback( sensor_msgs::msg::NavSatFix::SharedPtr msg) {
        auto twist_msg = geometry_msgs::msg::Twist();
        
        if (m == 0)
        {
        
        set_lat = msg->latitude;
        set_lon = msg->longitude;
        
        std::cout << set_lat << std::endl;

        twist_msg.linear.x = 0.0;
        twist_msg.angular.z = 1.0;
        movement_pub_->publish(twist_msg);
        sleep(6);

        twist_msg.linear.x = 0.0;
        twist_msg.angular.z = -1.0;
        movement_pub_->publish(twist_msg);
        sleep(8);

        twist_msg.linear.x = 0.0;
        twist_msg.angular.z = 0.0;
        movement_pub_->publish(twist_msg);

        twist_msg.linear.x = 1.0;
        twist_msg.angular.z = 0.0;
        movement_pub_->publish(twist_msg);
        sleep(3);

        twist_msg.linear.x = 0.0;
        twist_msg.angular.z = 0.0;
        movement_pub_->publish(twist_msg);
        m++;
        
    }
        
        current_lat_ = msg->latitude;
        current_lon_ = msg->longitude;
        navigateToTarget();
        q++;
        
    }


   

    void navigateToTarget() {
    auto twist_msg = geometry_msgs::msg::Twist();
    if (n==0 && q > 18){
        sleep(2);
        
        if (std::abs(current_lon_ - set_lon) > std::abs(set_lat - current_lat_))
        {
            if (current_lon_ - set_lon > 0.00002)
            {flag = 0;}
            else
            {flag = 1;}

        }
        else
        
        {
             if (set_lat - current_lat_ > 0.00002)
            {flag = 2;}
           else
            {flag = 3;}
        }
      
        
            twist_msg.linear.x = 0.0;  
            twist_msg.angular.z = 0.0;
            movement_pub_->publish(twist_msg);


        twist_msg.linear.x = -1.5;
        twist_msg.angular.z = 0.0;
        movement_pub_->publish(twist_msg);
        sleep(2.5);

        twist_msg.linear.x = 0.0;
        twist_msg.angular.z = 0.0;
        movement_pub_->publish(twist_msg);
        sleep(4.5);

        if (flag == 0)
        {std::cout << "flag0";
        twist_msg.linear.x = 0.0;
        twist_msg.angular.z = 0.0;
        movement_pub_->publish(twist_msg);
            twist_msg.linear.x = 0.0;
            twist_msg.angular.z = 1.0;
            movement_pub_->publish(twist_msg);
            sleep(8);
        }
        else if (flag == 1)
        {std::cout << "flag1";
        twist_msg.linear.x = 0.0;
        twist_msg.angular.z = 0.0;
        movement_pub_->publish(twist_msg);
            twist_msg.linear.x = 0.0;  
            twist_msg.angular.z = -1.0;
            movement_pub_->publish(twist_msg);
            sleep(12);
        }
        else if (flag == 2)
        { std::cout << "flag2";
        twist_msg.linear.x = 0.0;
        twist_msg.angular.z = 0.0;
        movement_pub_->publish(twist_msg);
            twist_msg.linear.x = 0.0;  
            twist_msg.angular.z = 1.0;
            movement_pub_->publish(twist_msg);
            sleep(8);
            
            twist_msg.linear.x = 0.0;  
            twist_msg.angular.z = 0.0;
            movement_pub_->publish(twist_msg);
            twist_msg.linear.x = 0.0;  
            twist_msg.angular.z = 1.0;
            movement_pub_->publish(twist_msg);
            sleep(8);
            twist_msg.linear.x = 0.0;  
            twist_msg.angular.z = 0.0;
            movement_pub_->publish(twist_msg);}
        else if (flag == 3)
        {std::cout << "flag3";
            twist_msg.linear.x = 0.0;  
        twist_msg.angular.z = 0.0;
        movement_pub_->publish(twist_msg);}
            else
            {}
        std::tie(permd, permb) = calculateDistanceAndBearing(set_lat, set_lon, target_lat_, target_lon_);
        



        if (permb < 0)
        permb += 2 * M_PI;

        if (permb > 1.5 * M_PI)
        permb -= 1.5 * M_PI;
        else if (permb > M_PI)
        permb -= M_PI;
        else if (permb > M_PI/2)
        permb -= M_PI/2;
        else
        permb = permb;

        if (((target_lat_ - current_lat_ > 0) && (target_lon_ - current_lon_ < 0)) || ((target_lat_ - current_lat_ < 0) && (target_lon_ - current_lon_ > 0))) 
        {permb = M_PI_2 - permb;}

    x_mov = permd * std::cos(permb);
    y_mov = permd * std::sin(permb);
    
    if (target_lat_ - current_lat_ > 0)
    {twist_msg.angular.z = 0.0;
    twist_msg.linear.x = 0.5;
    movement_pub_->publish(twist_msg);
    }
    else if (target_lat_ - current_lat_ < 0)
    {
    twist_msg.angular.z = 0.0;
    twist_msg.linear.x = -0.5;
    movement_pub_->publish(twist_msg);
    }
    else
    {}
    

    long double time = std::abs(x_mov);
    std::cout << time << " Move in x" << std::endl;
    
    auto inter = std::chrono::milliseconds(static_cast<int>(time * 1000));
    timer_ = this->create_wall_timer(inter, std::bind(&GpsNavigator::Turn, this));
    n++;
    }
    else
    {q ++;}

    }

void Turn(){ 
auto twist_msg = geometry_msgs::msg::Twist();
        if (n == 1){
        twist_msg.linear.x = 0.0;
        twist_msg.angular.z = 0.0;
        movement_pub_->publish(twist_msg);
        sleep(2);


        if (target_lon_ - current_lon_ > 0)
        {twist_msg.linear.x = 0.0;
        twist_msg.angular.z = -1.0;
        movement_pub_->publish(twist_msg);
         }
        else
        {
        twist_msg.linear.x = 0.0;
        twist_msg.angular.z = 1.0;
        movement_pub_->publish(twist_msg);
        }

        // if ((target_lat_ - set_lat > 0) && (target_lon_ - set_lon > 0))
        // {twist_msg.linear.x = 0.0;
        // twist_msg.angular.z = 1.0;
        // movement_pub_->publish(twist_msg);
        // std::cout << "first quad" << std::endl;
        // n++;}

        // else if ((target_lat_ - set_lat > 0) && (target_lon_ - set_lon < 0))
        // {twist_msg.linear.x = 0.0;
        // twist_msg.angular.z = -1.0;
        // movement_pub_->publish(twist_msg);
        // std::cout << "second quad" << std::endl;
        // n++;}

        // else if ((target_lat_ - set_lat < 0) && (target_lon_ - set_lon < 0))
        // {twist_msg.linear.x = 0.0;
        // twist_msg.angular.z = -1.0;
        // movement_pub_->publish(twist_msg);
        // std::cout << "third quad" << std::endl;
        // n++;}

        // else if ((target_lat_ - set_lat < 0) && (target_lon_ - set_lon > 0))
        // {twist_msg.linear.x = 0.0;
        // twist_msg.angular.z = 1.0;
        // movement_pub_->publish(twist_msg);
        // std::cout << "fourth quad" << std::endl;
        // n++;}
        // else
        // {end();}
        n++;
        sleep(10);
        std::cout << "Rotating" << std::endl;
        Straight();
        
        
        }
}

void Straight(){
    std::tie(distance,bearing) = calculateDistanceAndBearing(current_lat_, current_lon_, target_lat_, target_lon_);
    if (n == 2)
    {
    auto twist_msg = geometry_msgs::msg::Twist();
    twist_msg.angular.z = 0.0;
    twist_msg.linear.x = 0.0;
    movement_pub_->publish(twist_msg);
    
    twist_msg.angular.z = 0.0;
    twist_msg.linear.x = 0.5;
    movement_pub_->publish(twist_msg);
    n++;
    
    long double time = std::abs(y_mov);
    std::cout << time << " Move in y" << std::endl;
    auto inter = std::chrono::milliseconds(static_cast<int>(time * 1000));
    timer_ = this->create_wall_timer(inter, std::bind(&GpsNavigator::end, this));
    }

}


void end(){
auto twist_msg = geometry_msgs::msg::Twist();
twist_msg.linear.x = 0.0;
twist_msg.angular.z = 0.0;
movement_pub_->publish(twist_msg);
RCLCPP_INFO(this->get_logger(), "Target Reached\n");
rclcpp::shutdown();
exit(0);
}



    std::pair<long double, long double> calculateDistanceAndBearing(long double lat1, long double lon1, long double lat2, long double lon2) {
        const long double deg_to_rad = M_PI / 180.0;
        lat1 *= deg_to_rad;
        lon1 *= deg_to_rad;
        lat2 *= deg_to_rad;
        lon2 *= deg_to_rad;

        const long double dlat = lat2 - lat1;
        const long double dlon = lon2 - lon1;

        const long double a = std::sin(dlat / 2) * std::sin(dlat / 2) + std::cos(lat1) * std::cos(lat2) * std::sin(dlon / 2) * std::sin(dlon / 2);
        const long double c = 2 * std::atan2(std::sqrt(a), std::sqrt(1 - a));
        long double distance =  c * 6378137;

        long double y = std::sin(dlon) * std::cos(lat2);
        long double x = std::cos(lat1) * std::sin(lat2) - std::sin(lat1) * std::cos(lat2) * std::cos(dlon);
        long double bearing = std::atan2(y, x);

        return {distance, bearing};
    }




};






int main(int argc, char *argv[]) {
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<GpsNavigator>());
    rclcpp::shutdown();
    return 0;
}
