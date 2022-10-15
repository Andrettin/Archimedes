#pragma once

namespace archimedes {

class log_output_handler final
{
public:
	explicit log_output_handler(const std::filesystem::path &error_log_path)
		: error_log_path(error_log_path)
	{
		if (std::filesystem::exists(this->error_log_path) && std::filesystem::file_size(this->error_log_path) > 1000000) {
			std::filesystem::remove(this->error_log_path);
		}

		this->error_stream = std::ofstream(this->error_log_path);
		this->cerr_buf = std::cerr.rdbuf();
		std::cerr.rdbuf(this->error_stream.rdbuf());
	}

	~log_output_handler()
	{
		std::cerr.clear();
		std::cerr.rdbuf(this->cerr_buf);
		this->error_stream.close();

		if (std::filesystem::exists(this->error_log_path) && std::filesystem::file_size(this->error_log_path) == 0) {
			std::filesystem::remove(this->error_log_path);
		}
	}

private:
	const std::filesystem::path error_log_path;
	std::ofstream error_stream;
	std::streambuf *cerr_buf = nullptr;
};

}
