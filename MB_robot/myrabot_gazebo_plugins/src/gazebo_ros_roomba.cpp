//#include <boost/bind.hpp>
#include <nav_msgs/Odometry.h>
#include <sensor_msgs/JointState.h>
#include <geometry_msgs/Twist.h>
#include <turtlebot_node/TurtlebotSensorState.h>

#include "sensors/SensorManager.hh"
#include "sensors/RaySensor.hh"

#include <turtlebot_plugins/gazebo_ros_create.h>

#include <ros/time.h>

using namespace gazebo;

enum {LEFT= 0, RIGHT=1, FRONT=2, REAR=3};

GazeboRosCreate::GazeboRosCreate()
{
  this->spinner_thread_ = new boost::thread( boost::bind( &GazeboRosCreate::spin, this) );

  wheel_speed_ = new float[2];
  wheel_speed_[LEFT] = 0.0;
  wheel_speed_[RIGHT] = 0.0;

  set_joints_[0] = false;
  set_joints_[1] = false;
  set_joints_[2] = false;
  set_joints_[3] = false;
  joints_[0].reset();
  joints_[1].reset();
  joints_[2].reset();
  joints_[3].reset();
}

GazeboRosCreate::~GazeboRosCreate()
{
  rosnode_->shutdown();
  this->spinner_thread_->join();
  delete this->spinner_thread_;
  delete [] wheel_speed_;
  delete rosnode_;
}
    
void GazeboRosCreate::Load(physics::ModelPtr _parent, sdf::ElementPtr _sdf )
{
  this->my_world_ = _parent->GetWorld();

  this->my_parent_ = _parent;
  if (!this->my_parent_)
  {
    ROS_FATAL("Gazebo_ROS_Create controller requires a Model as its parent");
    return;
  }


  this->node_namespace_ = "";
  if (_sdf->HasElement("node_namespace"))
    this->node_namespace_ = _sdf->GetElement("node_namespace")->GetValueString() + "/";


  left_wheel_joint_name_ = "left_wheel_joint";
  if (_sdf->HasElement("left_wheel_joint"))
    left_wheel_joint_name_ = _sdf->GetElement("left_wheel_joint")->GetValueString();

  right_wheel_joint_name_ = "right_wheel_joint";
  if (_sdf->HasElement("right_wheel_joint"))
    right_wheel_joint_name_ = _sdf->GetElement("right_wheel_joint")->GetValueString();

  front_castor_joint_name_ = "front_castor_joint";
  if (_sdf->HasElement("front_castor_joint"))
    front_castor_joint_name_ = _sdf->GetElement("front_castor_joint")->GetValueString();

  rear_castor_joint_name_ = "rear_castor_joint";
  if (_sdf->HasElement("rear_castor_joint"))
    rear_castor_joint_name_ = _sdf->GetElement("rear_castor_joint")->GetValueString();

  wheel_sep_ = 0.34;
  if (_sdf->HasElement("wheel_separation"))
    wheel_sep_ = _sdf->GetElement("wheel_separation")->GetValueDouble();

  wheel_sep_ = 0.34;
  if (_sdf->HasElement("wheel_separation"))
    wheel_sep_ = _sdf->GetElement("wheel_separation")->GetValueDouble();

  wheel_diam_ = 0.15;
  if (_sdf->HasElement("wheel_diameter"))
    wheel_diam_ = _sdf->GetElement("wheel_diameter")->GetValueDouble();

  torque_ = 10.0;
  if (_sdf->HasElement("torque"))
    torque_ = _sdf->GetElement("torque")->GetValueDouble();


  //base_geom_name_ = "base_geom";
  base_geom_name_ = "base_footprint_geom_base_link";
  if (_sdf->HasElement("base_geom"))
    base_geom_name_ = _sdf->GetElement("base_geom")->GetValueString();
  base_geom_ = my_parent_->GetChildCollision(base_geom_name_);
  if (!base_geom_)
  {
    // This is a hack for ROS Diamond back. E-turtle and future releases
    // will not need this, because it will contain the fixed-joint reduction
    // in urdf2gazebo
    base_geom_ = my_parent_->GetChildCollision("base_footprint_geom");
    if (!base_geom_)
    {
      ROS_ERROR("Unable to find geom[%s]",base_geom_name_.c_str());
      return;
    }
  }

  base_geom_->SetContactsEnabled(true);
  contact_event_ = base_geom_->ConnectContact(boost::bind(&GazeboRosCreate::OnContact, this, _1, _2));

  // Get then name of the parent model
  std::string modelName = _sdf->GetParent()->GetValueString("name");

  // Listen to the update event. This event is broadcast every
  // simulation iteration.
  this->updateConnection = event::Events::ConnectWorldUpdateStart(
      boost::bind(&GazeboRosCreate::UpdateChild, this));
  gzdbg << "plugin model name: " << modelName << "\n";

  /*wall_sensor_ = boost::shared_dynamic_cast<sensors::RaySensor>(
    sensors::SensorManager::Instance()->GetSensor("wall_sensor"));
  if (!wall_sensor_)
  {
    ROS_ERROR("Unable to find sensor[wall_sensor]");
    return;
  }

  left_cliff_sensor_ = boost::shared_dynamic_cast<sensors::RaySensor>(
    sensors::SensorManager::Instance()->GetSensor("left_cliff_sensor"));
  right_cliff_sensor_ = boost::shared_dynamic_cast<sensors::RaySensor>(
    sensors::SensorManager::Instance()->GetSensor("right_cliff_sensor"));
  leftfront_cliff_sensor_ = boost::shared_dynamic_cast<sensors::RaySensor>(
    sensors::SensorManager::Instance()->GetSensor("leftfront_cliff_sensor"));
  rightfront_cliff_sensor_ = boost::shared_dynamic_cast<sensors::RaySensor>(
    sensors::SensorManager::Instance()->GetSensor("rightfront_cliff_sensor"));*/

  if (!ros::isInitialized())
  {
    int argc = 0;
    char** argv = NULL;
    ros::init(argc, argv, "gazebo_myrabot", ros::init_options::NoSigintHandler|ros::init_options::AnonymousName);
  }

  rosnode_ = new ros::NodeHandle( node_namespace_ );

  cmd_vel_sub_ = rosnode_->subscribe("/cmd_vel", 1, &GazeboRosCreate::OnCmdVel, this );

  sensor_state_pub_ = rosnode_->advertise<turtlebot_node::TurtlebotSensorState>("sensor_state", 1);
  odom_pub_ = rosnode_->advertise<nav_msgs::Odometry>("/odom", 1);

  joint_state_pub_ = rosnode_->advertise<sensor_msgs::JointState>("/joint_states_roomba", 1);

  js_.name.push_back( left_wheel_joint_name_ );
  js_.position.push_back(0);
  js_.velocity.push_back(0);
  js_.effort.push_back(0);

  js_.name.push_back( right_wheel_joint_name_ );
  js_.position.push_back(0);
  js_.velocity.push_back(0);
  js_.effort.push_back(0);

  js_.name.push_back( front_castor_joint_name_ );
  js_.position.push_back(0);
  js_.velocity.push_back(0);
  js_.effort.push_back(0);

  js_.name.push_back( rear_castor_joint_name_ );
  js_.position.push_back(0);
  js_.velocity.push_back(0);
  js_.effort.push_back(0);

  prev_update_time_ = 0;
  last_cmd_vel_time_ = 0;


  sensor_state_.bumps_wheeldrops = 0x0;

  //TODO: fix this

  joints_[LEFT] = my_parent_->GetJoint(left_wheel_joint_name_);
  joints_[RIGHT] = my_parent_->GetJoint(right_wheel_joint_name_);
  joints_[FRONT] = my_parent_->GetJoint(front_castor_joint_name_);
  joints_[REAR] = my_parent_->GetJoint(rear_castor_joint_name_);

  if (joints_[LEFT]) set_joints_[LEFT] = true;
  if (joints_[RIGHT]) set_joints_[RIGHT] = true;
  if (joints_[FRONT]) set_joints_[FRONT] = true;
  if (joints_[REAR]) set_joints_[REAR] = true;

  //initialize time and odometry position
  prev_update_time_ = last_cmd_vel_time_ = this->my_world_->GetSimTime();
  odom_pose_[0] = 0.0;
  odom_pose_[1] = 0.0;
  odom_pose_[2] = 0.0;
}


void GazeboRosCreate::OnContact(const std::string &name, const physics::Contact &contact)
{
  float y_overlap = 0.16495 * sin( 10 * (M_PI/180.0) );

  for (unsigned int j=0; j < (unsigned int)contact.count; j++)
  {
    // Make sure the contact is on the front bumper
    if (contact.positions[j].x > 0.012 && contact.positions[j].z < 0.06 && 
        contact.positions[j].z > 0.01)
    {
      // Right bump sensor
      if (contact.positions[j].y <= y_overlap)
        sensor_state_.bumps_wheeldrops |= 0x1; 
      // Left bump sensor
      if (contact.positions[j].y >= -y_overlap)
        sensor_state_.bumps_wheeldrops |= 0x2; 
    }
  }

}

void GazeboRosCreate::UpdateChild()
{
  common::Time time_now = this->my_world_->GetSimTime();
  common::Time step_time = time_now - prev_update_time_;
  prev_update_time_ = time_now;

  double wd, ws;
  double d1, d2;
  double dr, da;

  wd = wheel_diam_;
  ws = wheel_sep_;

  d1 = d2 = 0;
  dr = da = 0;

  // Distance travelled by front wheels
  if (set_joints_[LEFT])
    d1 = step_time.Double() * (wd / 2) * joints_[LEFT]->GetVelocity(0);
  if (set_joints_[RIGHT])
    d2 = step_time.Double() * (wd / 2) * joints_[RIGHT]->GetVelocity(0);

  // Can see NaN values here, just zero them out if needed
  if (isnan(d1)) {
    ROS_WARN_THROTTLE(0.1, "Gazebo ROS Create plugin. NaN in d1. Step time: %.2f. WD: %.2f. Velocity: %.2f", step_time.Double(), wd, joints_[LEFT]->GetVelocity(0));
    d1 = 0;
  }

  if (isnan(d2)) {
    ROS_WARN_THROTTLE(0.1, "Gazebo ROS Create plugin. NaN in d2. Step time: %.2f. WD: %.2f. Velocity: %.2f", step_time.Double(), wd, joints_[RIGHT]->GetVelocity(0));
    d2 = 0;
  }

  dr = (d1 + d2) / 2;
  da = (d2 - d1) / ws;

  // Compute odometric pose
  odom_pose_[0] += dr * cos( odom_pose_[2] );
  odom_pose_[1] += dr * sin( odom_pose_[2] );
  odom_pose_[2] += da;

  // Compute odometric instantaneous velocity
  odom_vel_[0] = dr / step_time.Double();
  odom_vel_[1] = 0.0;
  odom_vel_[2] = da / step_time.Double();


  if (set_joints_[LEFT])
  {
    joints_[LEFT]->SetVelocity( 0, wheel_speed_[LEFT] / (wd/2.0) );
    joints_[LEFT]->SetMaxForce( 0, torque_ );
  }
  if (set_joints_[RIGHT])
  {
    joints_[RIGHT]->SetVelocity( 0, wheel_speed_[RIGHT] / (wd / 2.0) );
    joints_[RIGHT]->SetMaxForce( 0, torque_ );
  }

  nav_msgs::Odometry odom;
  odom.header.stamp.sec = time_now.sec;
  odom.header.stamp.nsec = time_now.nsec;
  odom.header.frame_id = "odom";
  odom.child_frame_id = "base_footprint";
  odom.pose.pose.position.x = odom_pose_[0];
  odom.pose.pose.position.y = odom_pose_[1];
  odom.pose.pose.position.z = 0;

  btQuaternion qt;
  qt.setEuler(0,0,odom_pose_[2]);

  odom.pose.pose.orientation.x = qt.getX();
  odom.pose.pose.orientation.y = qt.getY();
  odom.pose.pose.orientation.z = qt.getZ();
  odom.pose.pose.orientation.w = qt.getW();

  double pose_cov[36] = { 1e-3, 0, 0, 0, 0, 0,
                          0, 1e-3, 0, 0, 0, 0,
                          0, 0, 1e6, 0, 0, 0,
                          0, 0, 0, 1e6, 0, 0,
                          0, 0, 0, 0, 1e6, 0,
                          0, 0, 0, 0, 0, 1e3};

  memcpy( &odom.pose.covariance[0], pose_cov, sizeof(double)*36 );
  memcpy( &odom.twist.covariance[0], pose_cov, sizeof(double)*36 );

  odom.twist.twist.linear.x = 0;
  odom.twist.twist.linear.y = 0;
  odom.twist.twist.linear.z = 0;

  odom.twist.twist.angular.x = 0;
  odom.twist.twist.angular.y = 0;
  odom.twist.twist.angular.z = 0;

  odom_pub_.publish( odom ); 

  js_.header.stamp.sec = time_now.sec;
  js_.header.stamp.nsec = time_now.nsec;
  if (this->set_joints_[LEFT])
  {
    js_.position[0] = joints_[LEFT]->GetAngle(0).GetAsRadian();
    js_.velocity[0] = joints_[LEFT]->GetVelocity(0);
  }

  if (this->set_joints_[RIGHT])
  {
    js_.position[1] = joints_[RIGHT]->GetAngle(0).GetAsRadian();
    js_.velocity[1] = joints_[RIGHT]->GetVelocity(0);
  }

  if (this->set_joints_[FRONT])
  {
    js_.position[2] = joints_[FRONT]->GetAngle(0).GetAsRadian();
    js_.velocity[2] = joints_[FRONT]->GetVelocity(0);
  }

  if (this->set_joints_[REAR])
  {
    js_.position[3] = joints_[REAR]->GetAngle(0).GetAsRadian();
    js_.velocity[3] = joints_[REAR]->GetVelocity(0);
  }

  joint_state_pub_.publish( js_ );

  this->UpdateSensors();

  //timeout if didn't receive cmd in a while
  common::Time time_since_last_cmd = time_now - last_cmd_vel_time_;
  if (time_since_last_cmd.Double() > 0.6)
  {
    wheel_speed_[LEFT] = 0;
    wheel_speed_[RIGHT] = 0;
  }

}

void GazeboRosCreate::UpdateSensors()
{
  /*if (wall_sensor_->GetRange(0) < 0.04)
    sensor_state_.wall = true;
  else
    sensor_state_.wall = false;

  if (left_cliff_sensor_->GetRange(0) > 0.02)
    sensor_state_.cliff_left = true;
  else
    sensor_state_.cliff_left = false;

  if (right_cliff_sensor_->GetRange(0) > 0.02)
    sensor_state_.cliff_right = true;
  else
    sensor_state_.cliff_right = false;

  if (rightfront_cliff_sensor_->GetRange(0) > 0.02)
    sensor_state_.cliff_front_right = true;
  else
    sensor_state_.cliff_front_right = false;

  if (leftfront_cliff_sensor_->GetRange(0) > 0.02)
    sensor_state_.cliff_front_left = true;
  else
    sensor_state_.cliff_front_left = false;*/

  sensor_state_pub_.publish(sensor_state_);

  // Reset the bump sensors
  sensor_state_.bumps_wheeldrops = 0x0;
}

void GazeboRosCreate::OnCmdVel( const geometry_msgs::TwistConstPtr &msg)
{
  last_cmd_vel_time_ = this->my_world_->GetSimTime();
  double vr, va;
  vr = msg->linear.x;
  va = msg->angular.z;

  wheel_speed_[LEFT] = vr - va * (wheel_sep_) / 2;
  wheel_speed_[RIGHT] = vr + va * (wheel_sep_) / 2;
}

void GazeboRosCreate::spin()
{
  while(ros::ok()) ros::spinOnce();
}

GZ_REGISTER_MODEL_PLUGIN(GazeboRosCreate);

