#include "jolt_distance_constraint_3d.hpp"

#include "servers/jolt_physics_server_3d.hpp"

namespace {
using ServerParamJolt = JoltPhysicsServer3D::DistanceConstraintParamJolt;
} // namespace

void JoltDistanceConstraint3D::_bind_methods() {
	// TODO(ewall198): Make sure that the physics server is jolt before allowing any interactions.
	BIND_METHOD(JoltDistanceConstraint3D, get_limit_spring_frequency);
	BIND_METHOD(JoltDistanceConstraint3D, set_limit_spring_frequency, "value");

	BIND_METHOD(JoltDistanceConstraint3D, get_limit_spring_damping);
	BIND_METHOD(JoltDistanceConstraint3D, set_limit_spring_damping, "value");

	BIND_METHOD(JoltDistanceConstraint3D, get_distance_min);
	BIND_METHOD(JoltDistanceConstraint3D, set_distance_min, "value");

	BIND_METHOD(JoltDistanceConstraint3D, get_distance_max);
	BIND_METHOD(JoltDistanceConstraint3D, set_distance_max, "value");

	ADD_GROUP("Distance", "distance_");

	BIND_PROPERTY("distance_min", Variant::FLOAT, "suffix:m");
	BIND_PROPERTY("distance_max", Variant::FLOAT, "suffix:m");

	ADD_GROUP("Limit Spring", "limit_spring_");

	BIND_PROPERTY("limit_spring_frequency", Variant::FLOAT, "suffix:hz");
	BIND_PROPERTY("limit_spring_damping", Variant::FLOAT);
}

void JoltDistanceConstraint3D::set_limit_spring_frequency(double p_value) {
	if (limit_spring_frequency == p_value) {
		return;
	}

	limit_spring_frequency = p_value;

	_param_changed(PARAM_LIMITS_SPRING_FREQUENCY);
}

void JoltDistanceConstraint3D::set_limit_spring_damping(double p_value) {
	if (limit_spring_damping == p_value) {
		return;
	}

	limit_spring_damping = p_value;

	_param_changed(PARAM_LIMITS_SPRING_DAMPING);
}

void JoltDistanceConstraint3D::set_distance_min(double p_value) {
	if (distance_min == p_value) {
		return;
	}

	distance_min = p_value;

	_param_changed(PARAM_DISTANCE_MIN);
}

void JoltDistanceConstraint3D::set_distance_max(double p_value) {
	if (distance_max == p_value) {
		return;
	}

	distance_max = p_value;

	_param_changed(PARAM_DISTANCE_MAX);
}

void JoltDistanceConstraint3D::_configure(PhysicsBody3D* p_body_a, PhysicsBody3D* p_body_b) {
	JoltPhysicsServer3D* physics_server = _get_jolt_physics_server();
	ERR_FAIL_NULL(physics_server);

	const Vector3 global_position = get_global_position();

	physics_server->joint_make_distance_constraint(
		rid,
		p_body_a->get_rid(),
		node_a_anchor,
		p_body_b != nullptr ? p_body_b->get_rid() : RID(),
		p_body_b != nullptr ? node_b_anchor : global_position
	);

	_update_jolt_param(PARAM_LIMITS_SPRING_FREQUENCY);
	_update_jolt_param(PARAM_LIMITS_SPRING_DAMPING);
	_update_jolt_param(PARAM_DISTANCE_MIN);
	_update_jolt_param(PARAM_DISTANCE_MAX);
}

void JoltDistanceConstraint3D::_update_jolt_param(Param p_param) {
	QUIET_FAIL_COND(_is_invalid());

	JoltPhysicsServer3D* physics_server = _get_jolt_physics_server();
	QUIET_FAIL_NULL(physics_server);

	double* value = nullptr;

	switch (p_param) {
		case PARAM_LIMITS_SPRING_FREQUENCY: {
			value = &limit_spring_frequency;
		} break;
		case PARAM_LIMITS_SPRING_DAMPING: {
			value = &limit_spring_damping;
		} break;
		case PARAM_DISTANCE_MIN: {
			value = &distance_min;
		} break;
		case PARAM_DISTANCE_MAX: {
			value = &distance_max;
		} break;
		default: {
			ERR_FAIL_REPORT(vformat("Unhandled parameter: `%d`.", p_param));
		} break;
	}

	physics_server->distance_constraint_set_jolt_param(rid, ServerParamJolt(p_param), *value);
}

void JoltDistanceConstraint3D::_param_changed(Param p_param) {
	// ewall198: This always calls _update_jolt_param since Godot hasn't implemented Distance Constraints. Keeping this function for style consistency.
	_update_jolt_param(p_param);
}
