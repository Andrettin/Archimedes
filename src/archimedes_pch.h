#pragma once

#pragma warning(push, 0)
#include <algorithm>
#include <array>
#include <cassert>
#include <cctype>
#include <cerrno>
#include <cinttypes>
#include <climits>
#include <cmath>
#include <cstdarg>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <deque>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iostream>
#include <list>
#include <map>
#include <memory>
#include <mutex>
#include <optional>
#include <queue>
#include <random>
#include <set>
#include <shared_mutex>
#if __has_include(<source_location>)
#include <source_location>
#endif
#include <sstream>
#include <stack>
#include <stdexcept>
#include <string>
#include <thread>
#include <tuple>
#include <type_traits>
#include <typeinfo>
#include <unordered_map>
#include <unordered_set>
#include <variant>
#include <vector>

#ifdef _WIN32
#include <SDKDDKVer.h>
#endif

#include <QColor>
#include <QCoreApplication>
#include <QDate>
#include <QDateTime>
#include <QDebug>
#include <QEvent>
#include <QGeoCircle>
#include <QGeoCoordinate>
#include <QGeoPath>
#include <QGeoPolygon>
#include <QGeoRectangle>
#include <QImage>
#include <QMetaProperty>
#include <QObject>
#include <QPoint>
#include <QString>
#include <QtConcurrent/QtConcurrentRun>
#include <QThread>
#include <QTime>
#include <QTimer>
#include <QVariant>
#include <QVariantList>

#include <QCoroFuture>
#include <QCoroTimer>
#include <QCoroTask>
#include <QCoroQmlTask>

#define MAGIC_ENUM_RANGE_MIN -1
#define MAGIC_ENUM_RANGE_MAX 64
#include <magic_enum/magic_enum.hpp>
#pragma warning(pop)

#include "util/point_operators.h" //to ensure the / operator with an int as RHS is used instead of the Qt one with qreal (which uses rounding)
#include "util/size_operators.h" //as above

//widely-used, seldomly-changed own headers
#include "database/data_type.h"
#include "util/decimillesimal_int.h"
#include "util/dice.h"

//metatype definitions
Q_DECLARE_METATYPE(std::filesystem::path)
Q_DECLARE_METATYPE(std::string)
