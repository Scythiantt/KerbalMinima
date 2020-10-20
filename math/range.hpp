#pragma once

#include <cmath>

struct Range {
	float start;
	float end;
	float interval;
	int size;

	Range(float start, float end, int size) : start(start), end(end), size(size), interval(size / (abs(start) + abs(end))) {}

	bool in_range(float value) {
		return !(value < start || value > end);
	}

	int operator[](float value) {
		return round((value - start) * interval);
	}
};