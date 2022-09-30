#include <ros/ros.h>
#include <sensor_msgs/LaserScan.h>
#include <iostream>
#include <math.h>


#define DEG2RAD(x) ((x)*M_PI/180.)


using namespace std;

ros::Publisher filtered_scan_pub;
ros::Subscriber input_scan_sub;
sensor_msgs::LaserScan filtered_scan;

bool update(const sensor_msgs::LaserScan& input_scan)
{
filtered_scan.ranges.resize(input_scan.ranges.size());
filtered_scan.intensities.resize(input_scan.intensities.size());
ros::Time start_time = input_scan.header.stamp;

if (!input_scan.ranges.empty()) {

    filtered_scan = input_scan;

    float res_per_deg = (int)input_scan.ranges.size() / (float)360.0;
    float las_mid_ran = res_per_deg * (float)180.0;
    float deg_60 = floor(res_per_deg * (float)60.0);

    for (unsigned int i = int(floor(las_mid_ran - deg_60)); i < int(floor(las_mid_ran+deg_60)); i++) { // left view of lidar frame <---|(0 degree)
        filtered_scan.ranges[i] = NAN;
}
        filtered_scan.header.frame_id = "base_scan";
        filtered_scan.header.stamp = start_time;

    //ROS_INFO("ranges size :  %d", input_scan.ranges.size());
    return true;
}

return false;
//ROS_DEBUG("Filtered out %d points from the laser scan.", (int)input_scan.ranges.size() - (int)count);


}

void input_scan_sub_callback(const sensor_msgs::LaserScan& input_scan)
{
    if(update(input_scan)) filtered_scan_pub.publish(filtered_scan);
    else{
        //sensor_msgs::LaserScan laser_nul;
        //filtered_scan_pub.publish(laser_nul);
        ROS_INFO("'''LiDAR RECEIVING ERROR'''");
    }
}


int main(int argc, char** argv){
ros::init(argc, argv, "rplidar_filter_node");
ros::NodeHandle n;
input_scan_sub = n.subscribe("/scan_rp", 50, input_scan_sub_callback);
filtered_scan_pub = n.advertise<sensor_msgs::LaserScan>("/scan_rp_filtered", 50);
ros::Rate loop_rate(20);

while(ros::ok())
  {
    ros::spinOnce();
    loop_rate.sleep();
  }
}