#pragma once

#include "database/data_module_container.h"
#include "database/gsml_data.h"
#include "util/qunique_ptr.h"
#include "util/singleton.h"
#include "util/type_traits.h"

namespace archimedes {

class data_entry;
class data_module;
class data_type_metadata;
class defines_base;
class game_rules_base;
class gsml_data;
class gsml_property;
class timeline;

class database final : public singleton<database>
{
public:
	static constexpr const char campaign_folder[] = "campaign";
	static constexpr const char campaign_map_filename[] = "campaign.smp";
	static constexpr const char data_folder[] = "data";
	static constexpr const char graphics_folder[] = "graphics";
	static constexpr const char maps_folder[] = "maps";
	static constexpr const char music_folder[] = "music";
	static constexpr const char save_folder[] = "save";
	static constexpr const char shaders_folder[] = "shaders";
	static constexpr const char sounds_folder[] = "sounds";
	static constexpr const char translations_folder[] = "translations";

	void process_gsml_property_for_object(QObject *object, const gsml_property &property);
	QVariant process_gsml_property_value(const gsml_property &property, const QMetaProperty &meta_property, const QObject *object);
	void process_gsml_scope_for_object(QObject *object, const gsml_data &scope);
	static QVariant process_gsml_scope_value(const gsml_data &scope, const QMetaProperty &meta_property);
	void modify_list_property_for_object(QObject *object, const std::string &property_name, const gsml_operator gsml_operator, const std::string &value);
	static void modify_list_property_for_object(QObject *object, const std::string &property_name, const gsml_operator gsml_operator, const gsml_data &scope);

	static std::filesystem::path get_documents_modules_path()
	{
		return database::get_documents_path() / "modules";
	}

	static std::filesystem::path get_documents_path();
	static std::filesystem::path get_user_data_path();

	static std::filesystem::path get_user_maps_path()
	{
		std::filesystem::path path = database::get_user_data_path() / database::maps_folder;
		database::ensure_path_exists(path);
		return path;
	}

	static std::filesystem::path get_save_path()
	{
		std::filesystem::path path = database::get_user_data_path() / database::save_folder;
		database::ensure_path_exists(path);
		return path;
	}

	static void ensure_path_exists(const std::filesystem::path &path);

	[[nodiscard]]
	static QCoro::Task<void> parse_folder(const std::filesystem::path &path, std::vector<gsml_data> &gsml_data_list);

public:
	database();
	~database();

	[[nodiscard]]
	QCoro::Task<void> parse();

	[[nodiscard]]
	QCoro::Task<void> load(const bool initial_definition);

	void load_predefines();
	void load_defines();
	void load_history(const QDate &start_date, const timeline *timeline, const game_rules_base *game_rules = nullptr);

	bool is_initialized() const
	{
		return this->initialized;
	}

	void initialize();

	void clear();
	void register_metadata(std::unique_ptr<data_type_metadata> &&metadata);

	const data_type_metadata *get_metadata(const std::string &identifier) const
	{
		const auto find_iterator = this->metadata_by_identifier.find(identifier);
		if (find_iterator != this->metadata_by_identifier.end()) {
			return find_iterator->second;
		}

		throw std::runtime_error(std::format("No data type metadata found with identifier \"{}\".", identifier));
	}

	void process_modules();
	void process_modules_at_dir(const std::filesystem::path &path, data_module *parent_module = nullptr);
	std::vector<std::filesystem::path> get_module_paths() const;
	std::vector<std::pair<std::filesystem::path, const data_module *>> get_module_paths_with_module() const;

	const std::vector<qunique_ptr<data_module>> &get_modules() const
	{
		return this->modules;
	}

	data_module *get_module(const std::string &identifier) const
	{
		const auto find_iterator = this->modules_by_identifier.find(identifier);
		if (find_iterator != this->modules_by_identifier.end()) {
			return find_iterator->second;
		}

		throw std::runtime_error(std::format("No module found with identifier \"{}\".", identifier));
	}

	bool has_module(const std::string &identifier) const
	{
		return this->modules_by_identifier.contains(identifier);
	}

	const std::filesystem::path &get_root_path() const
	{
		return this->root_path;
	}

	void set_root_path(const std::filesystem::path &path)
	{
		this->root_path = path;
	}

	std::filesystem::path get_modules_path() const
	{
		return this->get_root_path() / "modules";
	}

	std::filesystem::path get_dlcs_path() const
	{
		return this->get_root_path() / "dlcs";
	}

	std::filesystem::path get_workshop_path() const
	{
		return this->get_root_path().parent_path().parent_path() / "workshop" / "content" / this->workshop_game_id;
	}

	void set_workshop_game_id(const std::string &id)
	{
		this->workshop_game_id = id;
	}

	const std::filesystem::path &get_base_path(const data_module *data_module) const;

	std::filesystem::path get_graphics_path(const data_module *data_module) const
	{
		return this->get_base_path(data_module) / database::graphics_folder;
	}

	std::filesystem::path get_maps_path(const data_module *data_module) const
	{
		return this->get_base_path(data_module) / database::maps_folder;
	}

	std::filesystem::path get_campaign_maps_path() const
	{
		return this->get_maps_path(nullptr) / database::campaign_folder;
	}

	std::filesystem::path get_campaign_map_filepath() const
	{
		std::filesystem::path filepath = this->get_campaign_maps_path() / campaign_map_filename;
		filepath.make_preferred();
		return filepath;
	}

	std::filesystem::path get_music_path(const data_module *data_module)
	{
		return this->get_base_path(data_module) / database::music_folder;
	}

	std::filesystem::path get_sounds_path(const data_module *data_module)
	{
		return this->get_base_path(data_module) / database::sounds_folder;
	}

	std::vector<std::filesystem::path> get_base_paths() const
	{
		std::vector<std::filesystem::path> base_paths;
		base_paths.push_back(this->get_root_path());

		std::vector<std::filesystem::path> module_paths = this->get_module_paths();
		base_paths.insert(base_paths.end(), module_paths.begin(), module_paths.end());

		return base_paths;
	}

	std::vector<std::pair<std::filesystem::path, const data_module *>> get_base_paths_with_module() const
	{
		std::vector<std::pair<std::filesystem::path, const data_module *>> base_paths;
		base_paths.emplace_back(this->get_root_path(), nullptr);

		std::vector<std::pair<std::filesystem::path, const data_module *>> module_paths = this->get_module_paths_with_module();
		base_paths.insert(base_paths.end(), module_paths.begin(), module_paths.end());

		return base_paths;
	}

	std::filesystem::path get_data_path() const
	{
		return this->get_root_path() / database::data_folder;
	}

	std::vector<std::filesystem::path> get_data_paths() const
	{
		std::vector<std::filesystem::path> paths = this->get_base_paths();

		for (std::filesystem::path &path : paths) {
			path /= database::data_folder;
		}

		return paths;
	}

	std::vector<std::pair<std::filesystem::path, const data_module *>> get_data_paths_with_module() const
	{
		std::vector<std::pair<std::filesystem::path, const data_module *>> paths = this->get_base_paths_with_module();

		for (auto &kv_pair : paths) {
			std::filesystem::path &path = kv_pair.first;
			path /= database::data_folder;
		}

		return paths;
	}

	std::vector<std::filesystem::path> get_graphics_paths() const
	{
		std::vector<std::filesystem::path> paths = this->get_base_paths();

		for (std::filesystem::path &path : paths) {
			path /= database::graphics_folder;
		}

		return paths;
	}

	std::vector<std::filesystem::path> get_maps_paths() const
	{
		std::vector<std::filesystem::path> paths = this->get_base_paths();

		for (std::filesystem::path &path : paths) {
			path /= database::maps_folder;
		}

		paths.push_back(database::get_user_maps_path());

		return paths;
	}

	std::vector<std::filesystem::path> get_translations_paths() const
	{
		std::vector<std::filesystem::path> paths = this->get_base_paths();

		for (std::filesystem::path &path : paths) {
			path /= database::translations_folder;
		}

		return paths;
	}

	std::filesystem::path get_graphics_filepath(const std::filesystem::path &relative_filepath) const
	{
		const data_module *preferred_module = this->get_current_module();

		const std::filesystem::path preferred_path = this->get_graphics_path(preferred_module) / relative_filepath;

		if (std::filesystem::exists(preferred_path)) {
			return preferred_path;
		}

		const std::vector<std::filesystem::path> graphics_paths = this->get_graphics_paths();

		//iterate the graphics paths in reverse other, so that modules loaded later can override graphics of those loaded earlier
		for (auto iterator = graphics_paths.rbegin(); iterator != graphics_paths.rend(); ++iterator) {
			std::filesystem::path filepath = *iterator / relative_filepath;

			if (std::filesystem::exists(filepath)) {
				return filepath;
			}
		}

		throw std::runtime_error("Graphics file \"" + relative_filepath.string() + "\" not found.");
	}

	std::filesystem::path get_map_filepath(const std::filesystem::path &relative_filepath) const
	{
		const data_module *preferred_module = this->get_current_module();

		const std::filesystem::path preferred_path = this->get_maps_path(preferred_module) / relative_filepath;

		if (std::filesystem::exists(preferred_path)) {
			return preferred_path;
		}

		const std::vector<std::filesystem::path> maps_paths = this->get_maps_paths();

		//iterate the maps paths in reverse other, so that modules loaded later can override maps of those loaded earlier
		for (auto iterator = maps_paths.rbegin(); iterator != maps_paths.rend(); ++iterator) {
			std::filesystem::path filepath = *iterator / relative_filepath;

			if (std::filesystem::exists(filepath)) {
				return filepath;
			}
		}

		throw std::runtime_error("Map file \"" + relative_filepath.string() + "\" not found.");
	}

	std::filesystem::path get_shaders_path() const
	{
		return this->get_root_path() / database::shaders_folder;
	}

	const data_module *get_current_module() const
	{
		return this->current_module;
	}

	void set_current_module(const data_module *data_module)
	{
		this->current_module = data_module;
	}

	void set_defines(defines_base *defines)
	{
		this->defines = defines;
	}

	void register_string_to_qvariant_conversion(const std::string &class_name, std::function<QVariant(const std::string &)> &&function);
	void register_list_property_function(const std::string &class_name, std::function<bool(QObject *object, const std::string &, const std::string &)> &&function);

	void register_on_initialization_function(std::function<void()> &&function)
	{
		this->on_initialization_functions.push_back(std::move(function));
	}

private:
	std::filesystem::path root_path = std::filesystem::current_path();
	std::vector<std::unique_ptr<data_type_metadata>> metadata;
	std::map<std::string, const data_type_metadata *> metadata_by_identifier;
	std::vector<qunique_ptr<data_module>> modules;
	std::map<std::string, data_module *> modules_by_identifier;
	const data_module *current_module = nullptr; //the module currently being processed
	bool initialized = false;
	std::string workshop_game_id;
	defines_base *defines = nullptr;
	std::map<std::string, std::function<QVariant(const std::string &)>> string_to_qvariant_conversion_map; //conversions functions from string to QVariant, mapped to the respective class names
	std::map<std::string, std::function<bool(QObject *object, const std::string &, const std::string &)>> list_property_function_map;
	std::vector<std::function<void()>> on_initialization_functions;
	std::map<const data_type_metadata *, data_module_map<std::vector<gsml_data>>> gsml_data_to_process_by_data_type;
};

}
