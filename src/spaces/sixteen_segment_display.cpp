#include "sixteen_segment_display.hpp"

PackedVector3Array SixteenSegmentDisplay3D::get_lines_from_string(
	const JPH::string_view& str,
	PackedVector3Array& p_points
) {
	for (int i = 0; i < str.size(); i++) {
		const char& c = str[static_cast<unsigned int>(i)];
		const Vector3 letter_offset = CHARACTER_WIDTH * static_cast<real_t>(i);
		std::map<const char, uint32_t>::const_iterator it = CHARACTER_TO_HEX.find(c);
		if (it != CHARACTER_TO_HEX.end()) {
			uint32_t binary_code = it->second;

			if (binary_code & 1) { // Segment A1
				p_points.push_back(POINTS::TOP_LEFT + letter_offset);
				p_points.push_back(POINTS::TOP_CENTER + letter_offset);
			}
			if (binary_code & 1 << 1) { // Segment A2
				p_points.push_back(POINTS::TOP_CENTER + letter_offset);
				p_points.push_back(POINTS::TOP_RIGHT + letter_offset);
			}
			if (binary_code & 1 << 2) { // Segment B
				p_points.push_back(POINTS::TOP_RIGHT + letter_offset);
				p_points.push_back(POINTS::MIDDLE_RIGHT + letter_offset);
			}
			if (binary_code & 1 << 3) { // Segment C
				p_points.push_back(POINTS::MIDDLE_RIGHT + letter_offset);
				p_points.push_back(POINTS::LOWER_RIGHT + letter_offset);
			}
			if (binary_code & 1 << 4) { // Segment D1
				p_points.push_back(POINTS::LOWER_LEFT + letter_offset);
				p_points.push_back(POINTS::LOWER_CENTER + letter_offset);
			}
			if (binary_code & 1 << 5) { // Segment D2
				p_points.push_back(POINTS::LOWER_CENTER + letter_offset);
				p_points.push_back(POINTS::LOWER_RIGHT + letter_offset);
			}
			if (binary_code & 1 << 6) { // Segment E
				p_points.push_back(POINTS::LOWER_LEFT + letter_offset);
				p_points.push_back(POINTS::MIDDLE_LEFT + letter_offset);
			}
			if (binary_code & 1 << 7) { // Segment F
				p_points.push_back(POINTS::MIDDLE_LEFT + letter_offset);
				p_points.push_back(POINTS::TOP_LEFT + letter_offset);
			}
			if (binary_code & 1 << 8) { // Segment G1
				p_points.push_back(POINTS::MIDDLE_LEFT + letter_offset);
				p_points.push_back(POINTS::MIDDLE_CENTER + letter_offset);
			}
			if (binary_code & 1 << 9) { // Segment G2
				p_points.push_back(POINTS::MIDDLE_CENTER + letter_offset);
				p_points.push_back(POINTS::MIDDLE_RIGHT + letter_offset);
			}
			if (binary_code & 1 << 10) { // Segment H
				p_points.push_back(POINTS::TOP_LEFT + letter_offset);
				p_points.push_back(POINTS::MIDDLE_CENTER + letter_offset);
			}
			if (binary_code & 1 << 11) { // Segment I
				p_points.push_back(POINTS::TOP_CENTER + letter_offset);
				p_points.push_back(POINTS::MIDDLE_CENTER + letter_offset);
			}
			if (binary_code & 1 << 12) { // Segment J
				p_points.push_back(POINTS::TOP_RIGHT + letter_offset);
				p_points.push_back(POINTS::MIDDLE_CENTER + letter_offset);
			}
			if (binary_code & 1 << 13) { // Segment K
				p_points.push_back(POINTS::LOWER_LEFT + letter_offset);
				p_points.push_back(POINTS::MIDDLE_CENTER + letter_offset);
			}
			if (binary_code & 1 << 14) { // Segment L
				p_points.push_back(POINTS::LOWER_CENTER + letter_offset);
				p_points.push_back(POINTS::MIDDLE_CENTER + letter_offset);
			}
			if (binary_code & 1 << 15) { // Segment M
				p_points.push_back(POINTS::LOWER_RIGHT + letter_offset);
				p_points.push_back(POINTS::MIDDLE_CENTER + letter_offset);
			}
			if (binary_code & 1 << 16) { // Segment DP
				p_points.push_back(POINTS::DP1 + letter_offset);
				p_points.push_back(POINTS::DP2 + letter_offset);
			}
		}
	}
	return p_points;
}
