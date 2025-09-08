#pragma once

namespace archimedes::path {

inline std::string to_string(const std::filesystem::path &path)
{
#ifdef __clang__
	return path.string();
#else
	//convert a path to a UTF-8 encoded string
	const std::u8string u8str = path.u8string();
	return std::string(u8str.begin(), u8str.end());
#endif
}

inline std::filesystem::path from_string(const std::string &path_str)
{
	try {
#ifdef __clang__
		return std::filesystem::path(path_str);
#else
		//convert a UTF-8 encoded string to a path
		const std::u8string u8str(path_str.begin(), path_str.end());
		return std::filesystem::path(u8str);
#endif
	} catch (...) {
		std::throw_with_nested(std::runtime_error("Failed to convert string \"" + path_str + "\" to a path."));
	}
}

inline QString to_qstring(const std::filesystem::path &path)
{
#ifdef __clang__
	return QString::fromStdString(path.string());
#else
	const std::u8string u8str = path.u8string();
	return QString::fromUtf8(reinterpret_cast<const char *>(u8str.c_str()));
#endif
}

inline std::filesystem::path from_qstring(const QString &path_str)
{
#ifdef USE_WIN32
	return std::filesystem::path(path_str.toStdU16String());
#else
	return path::from_string(path_str.toStdString());
#endif
}

inline std::filesystem::path from_qurl(const QUrl &url)
{
	return path::from_qstring(url.toLocalFile());
}

}

Q_DECLARE_METATYPE(std::filesystem::path)
