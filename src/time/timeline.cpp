#include "archimedes.h"

#include "time/timeline.h"

#include "util/assert_util.h"

namespace archimedes {

void timeline::check() const
{
	assert_throw(this->get_point_of_divergence().isValid());
}

}
