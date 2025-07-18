#pragma once

#include "util/color_container.h"
#include "util/fractional_int.h"

namespace archimedes {
	enum class colorization_type;
}

namespace archimedes::image {

enum class frame_order {
	left_to_right,
	top_to_bottom
};

[[nodiscard]]
extern QCoro::Task<QImage> load(const std::filesystem::path &filepath);

inline QImage get_frame(const QImage &image, const int frame_x, const int frame_y, const QSize &frame_size)
{
	const int pixel_x = frame_x * frame_size.width();
	const int pixel_y = frame_y * frame_size.height();
	return image.copy(pixel_x, pixel_y, frame_size.width(), frame_size.height());
}

extern void copy_frame(const QImage &frame_image, const QPoint &frame_offset, QImage &dst_image, const QSize &dst_frame_size, const int dst_frame_index);

inline void copy_frame_data(const uint32_t *src_frame_data, uint32_t *dst_data, const QSize &frame_size, const int frame_x, const int frame_y, const int dst_width)
{
	//BPP is assumed to be 4, hence why uint32_t buffers are used

	const int frame_width = frame_size.width();
	const int frame_height = frame_size.height();

	for (int x = 0; x < frame_width; ++x) {
		for (int y = 0; y < frame_height; ++y) {
			const int frame_pixel_index = y * frame_width + x;
			const int pixel_x = frame_x * frame_width + x;
			const int pixel_y = frame_y * frame_height + y;
			const int pixel_index = pixel_y * dst_width + pixel_x;
			dst_data[pixel_index] = src_frame_data[frame_pixel_index];
		}
	}
}

template <QImage::Format image_format, typename function_type>
[[nodiscard]]
inline QImage scale(const QImage &src_image, const centesimal_int &scale_factor, const function_type &scaling_function)
{
	if (src_image.format() != image_format) {
		const QImage reformatted_src_image = src_image.convertToFormat(image_format);
		return image::scale<image_format>(reformatted_src_image, scale_factor, scaling_function);
	}

	//scale in steps if the scale factor is too large, since some scaling algorithms might have a maximum scale factor
	if (scale_factor > 5 && scale_factor.get_fractional_value() == 0 && (scale_factor.to_int() % 2) == 0) {
		const QImage scaled_src_image = image::scale<image_format>(src_image, centesimal_int(2), scaling_function);
		return image::scale<image_format>(scaled_src_image, scale_factor / 2, scaling_function);
	}

	int scale_multiplier = scale_factor.to_int();
	if (scale_factor.get_fractional_value() != 0) {
		if (scale_factor.get_fractional_value() == 50) {
			scale_multiplier = (scale_factor * 2).to_int();
		} else {
			scale_multiplier += 1;
		}
	}

	QImage result_image;

	if (scale_multiplier > 1) {
		result_image = QImage(src_image.size() * scale_multiplier, image_format);
		const unsigned char *src_data = src_image.constBits();
		unsigned char *dst_data = result_image.bits();

		scaling_function(scale_multiplier, reinterpret_cast<const uint32_t *>(src_data), reinterpret_cast<uint32_t *>(dst_data), src_image.width(), src_image.height());
	} else {
		result_image = src_image;
	}

	if (scale_factor.get_fractional_value() != 0) {
		const QSize scaled_size = src_image.size() * scale_factor;
		result_image = result_image.scaled(scaled_size, Qt::IgnoreAspectRatio, Qt::SmoothTransformation).convertToFormat(image_format);
	}

	return result_image;
}

template <QImage::Format image_format, typename function_type>
[[nodiscard]]
inline QImage scale_frame(const QImage &image, const int frame_x, const int frame_y, const centesimal_int &scale_factor, const QSize &old_frame_size, const function_type &scaling_function)
{
	const QImage frame_image = image::get_frame(image, frame_x, frame_y, old_frame_size);

	return image::scale<image_format>(frame_image, scale_factor, scaling_function);
}

template <QImage::Format image_format, typename function_type>
[[nodiscard]]
inline QImage scale(const QImage &src_image, const centesimal_int &scale_factor, const QSize &old_frame_size, const function_type &scaling_function)
{
	if (src_image.format() != image_format) {
		const QImage reformatted_src_image = src_image.convertToFormat(image_format);

		return image::scale<image_format>(reformatted_src_image, scale_factor, old_frame_size, scaling_function);
	}

	if (src_image.size() == old_frame_size) {
		//image has only one frame
		return image::scale<image_format>(src_image, scale_factor, scaling_function);
	}

	//scale an image with xBRZ
	const QSize new_frame_size = old_frame_size * scale_factor;

	const int bpp = src_image.depth() / 8;

	if (bpp != 4) {
		throw std::runtime_error("BPP must be 4 when scaling an image.");
	}

	const QSize result_size = src_image.size() * scale_factor;
	QImage result_image(result_size, image_format);

	if (result_image.isNull()) {
		throw std::runtime_error("Failed to allocate image to be scaled.");
	}

	uint32_t *dst_data = reinterpret_cast<uint32_t *>(result_image.bits());

	//if a simple scale factor is being used for the resizing, then use xBRZ for the rescaling
	const int horizontal_frame_count = src_image.width() / old_frame_size.width();
	const int vertical_frame_count = src_image.height() / old_frame_size.height();

	//scale each frame individually
	std::vector<QFuture<void>> futures;
	const int result_width = result_size.width();

	for (int frame_x = 0; frame_x < horizontal_frame_count; ++frame_x) {
		for (int frame_y = 0; frame_y < vertical_frame_count; ++frame_y) {
			QFuture<void> future = QtConcurrent::run([frame_x, frame_y, &src_image, &scale_factor, &old_frame_size, &new_frame_size, dst_data, result_width, scaling_function]() {
				const QImage result_frame_image = image::scale_frame<image_format>(src_image, frame_x, frame_y, scale_factor, old_frame_size, scaling_function);

				const uint32_t *frame_data = reinterpret_cast<const uint32_t *>(result_frame_image.constBits());
				image::copy_frame_data(frame_data, dst_data, new_frame_size, frame_x, frame_y, result_width);
			});

			futures.push_back(std::move(future));
		}
	}

	for (QFuture<void> &future : futures) {
		future.waitForFinished();
	}

	return result_image;
}

extern std::set<QRgb> get_rgbs(const QImage &image);
extern color_set get_colors(const QImage &image);

inline int get_frames_per_row(const QImage &image, const int frame_width)
{
	return image.width() / frame_width;
}

inline int get_frames_per_column(const QImage &image, const int frame_height)
{
	return image.height() / frame_height;
}

extern int get_frame_index(const QImage &image, const QSize &frame_size, const QPoint &frame_pos);
extern QPoint get_frame_pos(const QImage &image, const QSize &frame_size, const int frame_index, const frame_order frame_order = frame_order::left_to_right);

inline std::vector<QImage> to_frames(const QImage &image, const QSize &frame_size)
{
	if (image.size() == frame_size) {
		return { image };
	}

	std::vector<QImage> frames;

	const int horizontal_frame_count = image::get_frames_per_row(image, frame_size.width());
	const int vertical_frame_count = image::get_frames_per_column(image, frame_size.height());

	for (int frame_y = 0; frame_y < vertical_frame_count; ++frame_y) {
		for (int frame_x = 0; frame_x < horizontal_frame_count; ++frame_x) {
			frames.push_back(image::get_frame(image, frame_x, frame_y, frame_size));
		}
	}

	return frames;
}

extern void pack_folder(const std::filesystem::path &dir_path, const frame_order frame_order, const int frames_per_row);
extern QImage crop_frames(const QImage &src_image, const QSize &src_frame_size, const QSize &min_size);

extern void index_to_palette(QImage &image, const color_set &palette);

inline void index_to_image_palette(QImage &image, const QImage &other_image)
{
	image::index_to_palette(image, image::get_colors(other_image));
}

extern void rotate_hue(QImage &image, const int degrees, const color_set &ignored_colors);
extern void colorize(QImage &image, const colorization_type colorization, const color_set &ignored_colors);

template <typename function_type>
inline void for_each_pixel_pos(const QImage &image, const function_type &function)
{
	for (int y = 0; y < image.height(); ++y) {
		for (int x = 0; x < image.width(); ++x) {
			function(x, y);
		}
	}
}

extern std::pair<std::filesystem::path, centesimal_int> get_scale_suffixed_filepath(const std::filesystem::path &filepath, const centesimal_int &scale_factor);

extern void set_outline_color(QImage &image, const QColor &color);

inline void apply_grayscale(QImage &image)
{
	if (image.format() != QImage::Format_RGBA8888) {
		image = image.convertToFormat(QImage::Format_RGBA8888);
	}

	for (int x = 0; x < image.width(); ++x) {
		for (int y = 0; y < image.height(); ++y) {
			const QPoint pixel_pos = QPoint(x, y);
			const QColor pixel_color = image.pixelColor(pixel_pos);

			if (pixel_color.alpha() == 0) {
				continue;
			}

			const int gray = qGray(pixel_color.red(), pixel_color.green(), pixel_color.blue());
			image.setPixelColor(pixel_pos, QColor(gray, gray, gray, pixel_color.alpha()));
		}
	}
}

inline void apply_greenscale(QImage &image)
{
	if (image.format() != QImage::Format_RGBA8888) {
		image = image.convertToFormat(QImage::Format_RGBA8888);
	}

	for (int x = 0; x < image.width(); ++x) {
		for (int y = 0; y < image.height(); ++y) {
			const QPoint pixel_pos = QPoint(x, y);
			const QColor pixel_color = image.pixelColor(pixel_pos);

			if (pixel_color.alpha() == 0) {
				continue;
			}

			const int green = std::max(0, std::max(std::max(pixel_color.red(), pixel_color.green()), pixel_color.blue()) - 32);
			image.setPixelColor(pixel_pos, QColor(0, green, 0, pixel_color.alpha()));
		}
	}
}

inline void apply_redscale(QImage &image)
{
	if (image.format() != QImage::Format_RGBA8888) {
		image = image.convertToFormat(QImage::Format_RGBA8888);
	}

	for (int x = 0; x < image.width(); ++x) {
		for (int y = 0; y < image.height(); ++y) {
			const QPoint pixel_pos = QPoint(x, y);
			const QColor pixel_color = image.pixelColor(pixel_pos);

			if (pixel_color.alpha() == 0) {
				continue;
			}

			const int red = std::max(0, std::max(std::max(pixel_color.red(), pixel_color.green()), pixel_color.blue()) - 32);
			image.setPixelColor(pixel_pos, QColor(red, 0, 0, pixel_color.alpha()));
		}
	}
}

}
