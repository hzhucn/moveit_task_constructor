#include <ros/ros.h>

#include <moveit_task_constructor/task.h>

#include <moveit_task_constructor/subtasks/current_state.h>
#include <moveit_task_constructor/subtasks/cartesian_position_motion.h>

using namespace moveit::task_constructor;

int main(int argc, char** argv){
	ros::init(argc, argv, "test_plan_current_state");

	Task t;

	t.addStart( std::make_shared<subtasks::CurrentState>("current state") );

	{
		auto move= std::make_shared<subtasks::CartesianPositionMotion>("lift object");
		move->setGroup("arm");
		move->setLink("s_model_tool0");
		move->setMinMaxDistance(0.05, 0.2);

		geometry_msgs::Vector3Stamped direction;
		direction.header.frame_id= "world";
		direction.vector.x= 1.0;
		move->along(direction);
		t.addAfter(move);
	}

	{
		auto move= std::make_shared<subtasks::CartesianPositionMotion>("lift object");
		move->setGroup("arm");
		move->setLink("s_model_tool0");
		move->setMinDistance(0.01);

		geometry_msgs::PointStamped target;
		target.header.frame_id= "world";
		target.point.y= 0.7;
		target.point.z= 1.5;
		move->towards(target);
		t.addAfter(move);
	}

	t.plan();

	return 0;
}