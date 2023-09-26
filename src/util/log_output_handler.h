#pragma once

namespace archimedes {

class log_output_handler final
{
private:
	static void init_log_file(const std::filesystem::path &filepath, std::ostream &ostream, std::ofstream &filestream, std::streambuf *buf)
	{
		if (std::filesystem::exists(filepath) && std::filesystem::file_size(filepath) > 1000000) {
			std::filesystem::remove(filepath);
		}

		filestream = std::ofstream(filepath);
		buf = ostream.rdbuf();
		ostream.rdbuf(filestream.rdbuf());
	}

	static void close_log_file(const std::filesystem::path &filepath, std::ostream &ostream, std::ofstream &filestream, std::streambuf *buf)
	{
		ostream.clear();
		ostream.rdbuf(buf);
		filestream.close();

		if (std::filesystem::exists(filepath) && std::filesystem::file_size(filepath) == 0) {
			std::filesystem::remove(filepath);
		}
	}

public:
	explicit log_output_handler(const std::filesystem::path &output_log_path, const std::filesystem::path &error_log_path)
		: output_log_path(output_log_path), error_log_path(error_log_path)
	{
		this->init_log_file(output_log_path, std::cout, this->output_stream, this->cout_buf);
		this->init_log_file(error_log_path, std::cerr, this->error_stream, this->cerr_buf);
	}

	~log_output_handler()
	{
		this->close_log_file(output_log_path, std::cout, this->output_stream, this->cout_buf);
		this->close_log_file(error_log_path, std::cerr, this->error_stream, this->cerr_buf);
	}

private:
	const std::filesystem::path output_log_path;
	std::ofstream output_stream;
	std::streambuf *cout_buf = nullptr;
	const std::filesystem::path error_log_path;
	std::ofstream error_stream;
	std::streambuf *cerr_buf = nullptr;
};

}
