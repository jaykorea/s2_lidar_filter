#include <ros/ros.h>
#include <sensor_msgs/LaserScan.h>
#include <iostream>
#include "sl_lidar.h"
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

//ROS_INFO("Scan Range is : %d", input_scan.ranges.size());

ros::Time start_time = input_scan.header.stamp;


// for (unsigned int i = 345; i < 360; ++i) {
// //filtered_scan.ranges[count] = input_scan.ranges[i];
// if (input_scan.ranges[i] > 0.1)
// filtered_scan.ranges[count] = input_scan.ranges[i];
// else
// filtered_scan.ranges[count] = NAN;
// filtered_scan.intensities[count] = input_scan.intensities[i];
// ++count;
// }

// for (unsigned int i = 0; i < 15; ++i) {
// //filtered_scan.ranges[count] = input_scan.ranges[i];
// if (input_scan.ranges[i] > 0.1)
// filtered_scan.ranges[count] = input_scan.ranges[i];
// else
// filtered_scan.ranges[count] = NAN;
// filtered_scan.intensities[count] = input_scan.intensities[i];
// ++count;
// }
if (!input_scan.ranges.empty()) {

// for (unsigned int i = 0; i < 3585; ++i) {
// //filtered_scan.ranges[count] = input_scan.ranges[i];
// // if ( (input_scan.ranges[i] > 0.1) &&(258<=ct<=460)) {
// // filtered_scan.ranges[ct] = input_scan.ranges[i];
// // }
// if (input_scan.ranges[i] > 0.1) filtered_scan.ranges[i] = input_scan.ranges[i];// input_scan.ranges[i];

// else
// filtered_scan.ranges[i] = NAN;
// filtered_scan.intensities[i] = input_scan.intensities[i];
// }
// for (unsigned int i = 862; i < 1153; ++i) {
// //filtered_scan.ranges[count] = input_scan.ranges[i];
// // if ( (input_scan.ranges[i] > 0.1) &&(258<=ct<=460)) {
// // filtered_scan.ranges[ct] = input_scan.ranges[i];
// // }
// if (input_scan.ranges[i] > 0.1) filtered_scan.ranges[i] = input_scan.ranges[i];// input_scan.ranges[i];

// else
// filtered_scan.ranges[i] = NAN;
// filtered_scan.intensities[i] = input_scan.intensities[i];
// }

filtered_scan = input_scan;

//input_scan.rnages.size();

float res_per_deg = (int)input_scan.ranges.size() / (float)360.0;
float las_mid_ran = res_per_deg * (float)180.0;
float deg_60 = floor(res_per_deg * (float)60.0);

for (unsigned int i = int(floor(las_mid_ran - deg_60)); i < int(floor(las_mid_ran+deg_60)); ++i) { // left view of lidar frame <---|(0 degree)
//filtered_scan.ranges[count] = input_scan.ranges[i];
// if ( (input_scan.ranges[i] > 0.1) &&(258<=ct<=460)) {
// filtered_scan.ranges[ct] = input_scan.ranges[i];
// }
// if (input_scan.ranges[i] > 0.1) filtered_scan.ranges[i] = NAN;// input_scan.ranges[i];

// else 
filtered_scan.ranges[i] = NAN;
//filtered_scan.intensities[i] = input_scan.intensities[i];
}
// for (unsigned int i = 996; i < 1792; ++i) { // right view of lidar frame (360 degree)|--->
// //filtered_scan.ranges[count] = input_scan.ranges[i];
// // if ( (input_scan.ranges[i] > 0.1) &&(258<=ct<=460)) {
// // filtered_scan.ranges[ct] = input_scan.ranges[i];
// // }
// if (input_scan.ranges[i] > 0.1) filtered_scan.ranges[i] = NAN;// input_scan.ranges[i];

// else filtered_scan.ranges[i] = NAN;
// filtered_scan.intensities[i] = input_scan.intensities[i];
// }
// filtered_scan.ranges.resize(ct);
// filtered_scan.intensities.resize(ct);

filtered_scan.header.frame_id = "base_scan";
filtered_scan.header.stamp = start_time;
// filtered_scan.angle_min = input_scan.angle_min;
// filtered_scan.angle_max = input_scan.angle_max;
// filtered_scan.angle_increment = input_scan.angle_increment;
// filtered_scan.time_increment = input_scan.time_increment;
// filtered_scan.scan_time = input_scan.scan_time;
// filtered_scan.range_min = input_scan.range_min;
// filtered_scan.range_max = input_scan.range_max;
}

// filtered_scan.ranges.resize(ct);
// filtered_scan.intensities.resize(ct);

//ROS_DEBUG("Filtered out %d points from the laser scan.", (int)input_scan.ranges.size() - (int)count);


return true;
}

void input_scan_sub_callback(const sensor_msgs::LaserScan& input_scan)
{
update(input_scan);
filtered_scan_pub.publish(filtered_scan);

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