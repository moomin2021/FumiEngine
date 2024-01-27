#pragma once
#include <stdint.h>

struct Timer {
	float coolTime = 0.0f;
	int64_t last = 0;

public:
	Timer() { coolTime = 0.0f, last = 0; }
	bool GetOn();
};