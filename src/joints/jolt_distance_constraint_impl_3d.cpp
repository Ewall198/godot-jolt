#include "jolt_distance_constraint_impl_3d.hpp"

#include "objects/jolt_body_impl_3d.hpp"
#include "spaces/jolt_space_3d.hpp"

JoltDistanceConstraintImpl3D::JoltDistanceConstraintImpl3D(
	const JoltJointImpl3D& p_old_joint,
	JoltBodyImpl3D* p_body_a,
	JoltBodyImpl3D* p_body_b,
	const Vector3& p_local_a,
	const Vector3& p_local_b
)
	: JoltJointImpl3D(
		  p_old_joint,
		  p_body_a,
		  p_body_b,
		  Transform3D({}, p_local_a),
		  Transform3D({}, p_local_b)
	  ) {
	rebuild();
}

double JoltDistanceConstraintImpl3D::get_jolt_param(JoltParameter p_param) const {
	switch (p_param) {
		case JoltPhysicsServer3D::DISTANCE_CONSTRAINT_LIMITS_SPRING_FREQUENCY: {
			return limit_spring_frequency;
		} break;
		case JoltPhysicsServer3D::DISTANCE_CONSTRAINT_LIMITS_SPRING_DAMPING: {
			return limit_spring_damping;
		} break;
		case JoltPhysicsServer3D::DISTANCE_CONSTRAINT_DISTANCE_MIN: {
			return distance_min;
		} break;
		case JoltPhysicsServer3D::DISTANCE_CONSTRAINT_DISTANCE_MAX: {
			return distance_max;
		} break;
		default: {
			ERR_FAIL_D_REPORT(vformat("Unhandled parameter: '%d'.", p_param));
		} break;
	}
}

void JoltDistanceConstraintImpl3D::set_jolt_param(JoltParameter p_param, double p_value) {
	switch (p_param) {
		case JoltPhysicsServer3D::DISTANCE_CONSTRAINT_LIMITS_SPRING_FREQUENCY: {
			limit_spring_frequency = p_value;
			_limit_spring_changed();
		} break;
		case JoltPhysicsServer3D::DISTANCE_CONSTRAINT_LIMITS_SPRING_DAMPING: {
			limit_spring_damping = p_value;
			_limit_spring_changed();
		} break;
		case JoltPhysicsServer3D::DISTANCE_CONSTRAINT_DISTANCE_MIN: {
			distance_min = p_value;
			_distance_changed();
		} break;
		case JoltPhysicsServer3D::DISTANCE_CONSTRAINT_DISTANCE_MAX: {
			distance_max = p_value;
			_distance_changed();
		} break;
		default: {
			ERR_FAIL_REPORT(vformat("Unhandled parameter: '%d'.", p_param));
		} break;
	}
}

void JoltDistanceConstraintImpl3D::rebuild() {
	destroy();

	JoltSpace3D* space = get_space();

	if (space == nullptr) {
		return;
	}

	const JPH::BodyID body_ids[2] = {
		body_a != nullptr ? body_a->get_jolt_id() : JPH::BodyID(),
		body_b != nullptr ? body_b->get_jolt_id() : JPH::BodyID()
	};

	const JoltWritableBodies3D jolt_bodies = space->write_bodies(body_ids, count_of(body_ids));

	auto* jolt_body_a = static_cast<JPH::Body*>(jolt_bodies[0]);
	auto* jolt_body_b = static_cast<JPH::Body*>(jolt_bodies[1]);

	ERR_FAIL_COND(jolt_body_a == nullptr && jolt_body_b == nullptr);

	Transform3D shifted_ref_a;
	Transform3D shifted_ref_b;

	_shift_reference_frames(Vector3(), Vector3(), shifted_ref_a, shifted_ref_b);

	jolt_ref = _build_constraint(jolt_body_a, jolt_body_b, shifted_ref_a, shifted_ref_b);

	space->add_joint(this);

	_update_enabled();
	_update_iterations();
}

JPH::Constraint* JoltDistanceConstraintImpl3D::_build_constraint(
	JPH::Body* p_jolt_body_a,
	JPH::Body* p_jolt_body_b,
	const Transform3D& p_shifted_ref_a,
	const Transform3D& p_shifted_ref_b
) {
	JPH::DistanceConstraintSettings constraint_settings;
	constraint_settings.mSpace = JPH::EConstraintSpace::LocalToBodyCOM;
	constraint_settings.mPoint1 = to_jolt_r(p_shifted_ref_a.origin);
	constraint_settings.mPoint2 = to_jolt_r(p_shifted_ref_b.origin);
	constraint_settings.mMinDistance = (float)distance_min;
	constraint_settings.mMaxDistance = (float)distance_max;
	constraint_settings.mLimitsSpringSettings.mFrequency = (float)limit_spring_frequency;
	constraint_settings.mLimitsSpringSettings.mDamping = (float)limit_spring_damping;

	if (p_jolt_body_a == nullptr) {
		return constraint_settings.Create(JPH::Body::sFixedToWorld, *p_jolt_body_b);
	} else if (p_jolt_body_b == nullptr) {
		return constraint_settings.Create(*p_jolt_body_a, JPH::Body::sFixedToWorld);
	} else {
		return constraint_settings.Create(*p_jolt_body_a, *p_jolt_body_b);
	}
}

void JoltDistanceConstraintImpl3D::_limit_spring_changed() {
	rebuild();
	_wake_up_bodies();
}

void JoltDistanceConstraintImpl3D::_limit_distance_changed() {
	rebuild();
	_wake_up_bodies();
}

void JoltDistanceConstraintImpl3D::_distance_changed() {
	rebuild();
	_wake_up_bodies();
}
