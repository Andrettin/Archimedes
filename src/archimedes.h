#pragma once

#include "pch.h"

//forward declarations

namespace archimedes {
	class calendar;
	class language;
	class language_family;
	class map_projection;
	class timeline;
	class word;
}

Q_DECLARE_OPAQUE_POINTER(archimedes::calendar*)
Q_DECLARE_OPAQUE_POINTER(archimedes::language*)
Q_DECLARE_OPAQUE_POINTER(archimedes::language_family*)
Q_DECLARE_OPAQUE_POINTER(archimedes::map_projection*)
Q_DECLARE_OPAQUE_POINTER(archimedes::timeline*)
Q_DECLARE_OPAQUE_POINTER(archimedes::word*)
