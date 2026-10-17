#include "archimedes.h"

#include "util/path_util.h"

#pragma warning(push, 0)
#include <QUrl>
#pragma warning(pop)

namespace archimedes::path {

std::filesystem::path from_qurl(const QUrl &url)
{
	return path::from_qstring(url.toLocalFile());
}

}
