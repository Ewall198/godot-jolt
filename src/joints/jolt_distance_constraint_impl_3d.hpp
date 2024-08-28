#pragma once

#include "joints/jolt_joint_impl_3d.hpp"
#include "servers/jolt_physics_server_3d.hpp"

class JoltDistanceConstraintImpl3D final : public JoltJointImpl3D {
	using JoltParameter = JoltPhysicsServer3D::DistanceConstraintParamJolt;

public:
	JoltDistanceConstraintImpl3D(
		const JoltJointImpl3D& p_old_joint,
		JoltBodyImpl3D* p_body_a,
		JoltBodyImpl3D* p_body_b,
		const Transform3D& p_local_ref_a,
		const Transform3D& p_local_ref_b
	);

	PhysicsServer3D::JointType get_type() const override { return PhysicsServer3D::JOINT_TYPE_MAX; }

	double get_jolt_param(JoltParameter p_param) const;

	double set_jolt_param(JoltParameter p_param, double p_value);

	Vector3 get_local_a() const { return local_ref_a.origin; }

	void set_local_a(const Vector3& p_local_a);

	Vector3 get_local_b() const { return local_ref_b.origin; }

	void set_local_b(const Vector3& p_local_b);

	//float get_applied_force() const;

	//float get_applied_torque() const;

	void rebuild() override;

private:
	JPH::Constraint* _build_constraint(
		JPH::Body* p_jolt_body_a,
		JPH::Body* p_jolt_body_b,
		const Transform3D& p_shifted_ref_a,
		const Transform3D& p_shifted_ref_b
	);

	void _points_changed();

	void _limit_spring_changed();

	void _limit_distance_changed();

	double limit_spring_frequency = 0.0;

	double limit_spring_damping = 0.0;

	double distance_min = 0.0;

	double distance_max = INFINITY;
};
