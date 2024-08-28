#pragma once

#include "joints/jolt_joint_3d.hpp"
#include "servers/jolt_physics_server_3d.hpp"

class JoltDistanceConstraint3D final : public JoltJoint3D {
	GDCLASS_QUIET(JoltDistanceConstraint3D, JoltJoint3D)

public:
	enum Param {
		PARAM_LIMITS_SPRING_FREQUENCY = JoltPhysicsServer3D::DISTANCE_CONSTRAINT_LIMITS_SPRING_FREQUENCY,
		PARAM_LIMITS_SPRING_DAMPING = JoltPhysicsServer3D::DISTANCE_CONSTRAINT_LIMITS_SPRING_DAMPING,
		PARAM_DISTANCE_MIN = JoltPhysicsServer3D::DISTANCE_CONSTRAINT_DISTANCE_MIN,
		PARAM_DISTANCE_MAX = JoltPhysicsServer3D::DISTANCE_CONSTRAINT_DISTANCE_MAX
	};

private:
	static void _bind_methods();

public:
	double get_limit_spring_frequency() const { return limit_spring_frequency; }

	void set_limit_spring_frequency(double p_value);

	double get_limit_spring_damping() const { return limit_spring_damping; }

	void set_limit_spring_damping(double p_value);

	double get_distance_min() const { return distance_min; }

	void set_distance_min(double p_value);

	double get_distance_max() const { return distance_max; }

	void set_distance_max(double p_value);

private:
	void _configure(PhysicsBody3D* p_body_a, PhysicsBody3D* p_body_b) override;

	void _update_jolt_param(Param p_param);

	void _param_changed(Param p_param);

	double limit_spring_frequency = 0.0;

	double limit_spring_damping = 0.0;

	double distance_min = 0.0;

	double distance_max = INFINITY;
};
