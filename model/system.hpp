#pragma once

#include "model/body.hpp"

namespace minima {

#define MAX_BODIES 16

struct System {
	Body bodies[MAX_BODIES];
	int bodies_number;
};

}


