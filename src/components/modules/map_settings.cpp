#include "std_include.hpp"

#include "game_settings.hpp"
#include "imgui.hpp"
#include "interfaces.hpp"
#include "main_module.hpp"
#include "remix_lights.hpp"
#include "remix_rayportal.hpp"
#include "components/common/flags.hpp"
#include "components/common/remix_api.hpp"

#include "components/common/toml_ext.hpp"
#include "toml11/parser.hpp"

namespace components
{
	void map_settings::set_settings_for_map(const std::string& map_name)
	{
		// infra changelevels use mixed case map names
		m_map_settings.mapname = utils::str_to_lower( !map_name.empty() ? map_name : game::get_map_name() );
		utils::replace_all(m_map_settings.mapname, std::string("maps/"), "");		// if sp map
		utils::replace_all(m_map_settings.mapname, std::string(".bsp"), "");

		parse_toml();

		static bool disable_map_configs = common::flags::has_flag("xo_disable_map_conf");
		if (common::remix_api::is_initialized())
		{
			if (!disable_map_configs)
			{
				// resets all modified variables back to rtx.conf level
				remix_vars::reset_all_modified();

				// auto apply {map_name}.conf (if it exists)
				//open_and_set_var_config(m_map_settings.mapname + ".conf", true);
				if (remix_vars::parse_and_apply_conf_with_lerp(m_map_settings.mapname + ".conf", 0xDEADBEEF, remix_vars::EASE_TYPE_LINEAR, 0.1f, true)) {
					common::log("MapSettings", std::format("Applying config: {}", m_map_settings.mapname + ".conf"), common::LOG_TYPE::LOG_TYPE_DEFAULT, false);
				}
				

				// apply other manually defined configs
				uint32_t count = 0u;
				for (const auto& f : m_map_settings.api_var_configs) 
				{
					common::log("MapSettings", std::format("Applying config: {}", f), common::LOG_TYPE::LOG_TYPE_DEFAULT, false);
					//open_and_set_var_config(f);
					remix_vars::parse_and_apply_conf_with_lerp(f, ++count, remix_vars::EASE_TYPE_LINEAR, 0.1f);
				}
			}

			main_module::cross_handle_map_and_game_settings();

			// lights are spawned manually in edit mode
			if (!imgui::get()->m_light_edit_mode) {
				remix_lights::get()->add_all_map_setting_lights_without_creation_trigger();
			}
		}

		m_map_settings.default_nocull_dist = game_settings::get()->default_nocull_distance.get_as<float>();

		// are we using any sound hashes or names to trigger configvar transitions?
		{
			if (!m_map_settings.remix_transitions.empty())
			{
				for (const auto& t : m_map_settings.remix_transitions)
				{
					if (t.trigger_type == TRANSITION_TRIGGER_TYPE::SOUND && t.sound_hash) {
						m_map_settings.using_any_transition_sound_hash = true;
					}

					if (t.trigger_type == TRANSITION_TRIGGER_TYPE::SOUND && !t.sound_name.empty()) {
						m_map_settings.using_any_transition_sound_name = true;
					}
				}
			}
		}

		// are we using any sound hashes to trigger light spawning?
		{
			if (!m_map_settings.remix_lights.empty())
			{
				for (const auto& l : m_map_settings.remix_lights)
				{
					if (l.trigger_sound_hash || l.kill_sound_hash)
					{
						m_map_settings.using_any_light_sound_hash = true;
						break;
					}
				}
			}
		}

		m_loaded = true;
	}

	
	toml::value parse_toml_wrapper(const std::string& file_path)
	{
		return toml::parse(file_path, toml::spec::v(1, 1, 0));
	}

	bool map_settings::parse_toml()
	{
		try
		{
			const std::string file_path = globals::root_path + COMPMOD_ASSET_DIR "map_settings.toml";
			auto config = parse_toml_wrapper(file_path);

			// ####################
			// parse 'FOG' table
			if (config.contains("FOG"))
			{
				auto& fog_table = config["FOG"];

				// try to find the loaded map
				if (fog_table.contains(m_map_settings.mapname))
				{
					if (const auto map = fog_table[m_map_settings.mapname];
						!map.is_empty())
					{
						const bool has_distance = map.contains("distance");
						const bool has_density = map.contains("density");

						if ((has_distance || has_density) && map.contains("color"))
						{
							if (has_distance) {
								m_map_settings.fog_dist = common::toml_ext::to_float(map.at("distance"));
							}
							else if (has_density) {
								m_map_settings.fog_density = common::toml_ext::to_float(map.at("density"));
							}

							if (const auto& color = map.at("color").as_array();
								color.size() == 3)
							{
								const auto r = static_cast<std::uint8_t>(common::toml_ext::to_int(color[0]));
								const auto g = static_cast<std::uint8_t>(common::toml_ext::to_int(color[1]));
								const auto b = static_cast<std::uint8_t>(common::toml_ext::to_int(color[2]));
								m_map_settings.fog_color = D3DCOLOR_XRGB(r, g, b);
							}
						}
					}
				}
			} // end 'FOG'


			// ####################
			// parse 'WATER' table
			if (config.contains("WATER"))
			{
				auto& water_table = config["WATER"];

				// try to find the loaded map
				if (water_table.contains(m_map_settings.mapname))
				{
					if (const auto map = water_table[m_map_settings.mapname];
						!map.is_empty())
					{
						if (map.contains("scale")) {
							m_map_settings.water_uv_scale = common::toml_ext::to_float(map.at("scale"), 1.0f);
						}

						if (map.contains("scale_top")) {
							m_map_settings.water_uv_top_scale = common::toml_ext::to_float(map.at("scale_top"), 0.0f);
						}

						if (map.contains("top_layer_offset")) {
							m_map_settings.water_offset_top = common::toml_ext::to_float(map.at("top_layer_offset"), 0.5f);
						}

						if (map.contains("bottom_layer_offset")) {
							m_map_settings.water_offset_bottom = common::toml_ext::to_float(map.at("bottom_layer_offset"), 0.0f);
						}
					}
				}
			} // end 'WATER'


			// ####################
			// parse 'SUN' table
			if (config.contains("SUN"))
			{
				auto& sun_table = config["SUN"];

				// try to find the loaded map
				if (sun_table.contains(m_map_settings.mapname))
				{
					if (const auto map = sun_table[m_map_settings.mapname];
						!map.is_empty())
					{
						auto& sun = m_map_settings.sun;

						if (map.contains("intensity")) {
							sun.intensity = std::max( common::toml_ext::to_float(map.at("intensity"), 1.0f), 0.0f );
						}

						if (map.contains("diameter")) {
							sun.diameter = std::max( common::toml_ext::to_float(map.at("diameter"), 1.0f), 0.01f );
						}

						if (map.contains("pitch") && map.contains("yaw"))
						{
							sun.custom_direction = true;
							sun.pitch = common::toml_ext::to_float(map.at("pitch"), 0.0f);
							sun.yaw = common::toml_ext::to_float(map.at("yaw"), 0.0f);
						}
					}
				}
			} // end 'SUN'


			// ####################
			// parse 'CULL' table
			if (config.contains("CULL"))
			{
				auto& cull_table = config["CULL"];

				// #
				auto process_cull_entry = [](const toml::value& entry)
					{
						const auto contains_leafs = entry.contains("leafs");
						const auto contains_areas = entry.contains("areas");
						const auto contains_leaf_tweak = entry.contains("leaf_tweak");
						const auto contains_hidden_leafs = entry.contains("hide_leafs");
						const auto contains_hidden_areas = entry.contains("hide_areas");
						const auto contains_cull = entry.contains("cull");

						if (entry.contains("in_area"))
						{
							const auto area = common::toml_ext::to_uint(entry.at("in_area"));

							// forced leafs
							std::unordered_set<std::uint32_t> leaf_set;
							if (contains_leafs)
							{
								auto& leafs = entry.at("leafs").as_array();

								for (const auto& leaf : leafs) {
									leaf_set.insert(common::toml_ext::to_uint(leaf));
								}
							}

							// forced areas
							std::unordered_set<std::uint32_t> area_set;
							if (contains_areas)
							{
								auto& areas = entry.at("areas").as_array();

								for (const auto& a : areas) {
									area_set.insert(common::toml_ext::to_uint(a));
								}
							}

							// culling mode
							AREA_CULL_MODE cmode = imgui::get()->m_disable_cullnode ? map_settings::AREA_CULL_MODE_NO_FRUSTUM : map_settings::AREA_CULL_INFO_DEFAULT;
							if (contains_cull)
							{
								auto m = common::toml_ext::to_uint(entry.at("cull"));
								if (m >= AREA_CULL_INFO_COUNT)
								{
									common::log("MapSettings", std::format("param 'cull' was out-of-range {:d}", m), common::LOG_TYPE::LOG_TYPE_ERROR, false);
									m = 0u;
								}
								cmode = (AREA_CULL_MODE)(std::uint8_t)m;
							}

							// nocull dist for certain cull modes
							float temp_nocull_dist = game_settings::get()->default_nocull_distance.get_as<float>();
							if (entry.contains("nocull_dist")) {
								temp_nocull_dist = common::toml_ext::to_float(entry.at("nocull_dist"));
							}

							// hidden leafs
							std::unordered_set<std::uint32_t> hidden_leaf_set;
							if (contains_hidden_leafs)
							{
								auto& leafs = entry.at("hide_leafs").as_array();

								for (const auto& leaf : leafs) {
									hidden_leaf_set.insert(common::toml_ext::to_uint(leaf));
								}
							}

							// hidden areas
							std::vector<hide_area_s> temp_hidden_areas_set;
							if (contains_hidden_areas)
							{
								auto& hide_areas = entry.at("hide_areas").as_array();
								for (const auto& elem : hide_areas)
								{
									if (elem.contains("areas"))
									{
										std::unordered_set<std::uint32_t> temp_area_set;
										const auto& areas = elem.at("areas").as_array();

										for (const auto& a : areas) {
											temp_area_set.insert(common::toml_ext::to_uint(a));
										}

										std::unordered_set<std::uint32_t> temp_not_in_leaf_set;
										if (elem.contains("N_leafs"))
										{
											const auto& nleafs = elem.at("N_leafs").as_array();
											for (const auto& nl : nleafs) {
												temp_not_in_leaf_set.insert(common::toml_ext::to_uint(nl));
											}
										}

										temp_hidden_areas_set.emplace_back(std::move(temp_area_set), std::move(temp_not_in_leaf_set));
									}
								}
							}

							// leaf tweaks
							std::vector<leaf_tweak_s> temp_leaf_tweak_set;
							bool any_nocull_dist_overrides_in_leaf_tweaks = false;

							if (contains_leaf_tweak)
							{
								auto& leaf_tweak = entry.at("leaf_tweak").as_array();
								for (const auto& elem : leaf_tweak)
								{
									if (elem.contains("in_leafs"))
									{
										std::unordered_set<std::uint32_t> temp_in_leafs_set;
										const auto& in_leafs = elem.at("in_leafs").as_array();

										for (const auto& l : in_leafs) {
											temp_in_leafs_set.insert(common::toml_ext::to_uint(l));
										}

										std::unordered_set<std::uint32_t> temp_areas;
										if (elem.contains("areas"))
										{
											const auto& areas = elem.at("areas").as_array();
											for (const auto& a : areas) {
												temp_areas.insert(common::toml_ext::to_uint(a));
											}
										}

										std::unordered_set<std::uint32_t> temp_leafs;
										if (elem.contains("leafs"))
										{
											const auto& leafs = elem.at("leafs").as_array();
											for (const auto& l : leafs) {
												temp_leafs.insert(common::toml_ext::to_uint(l));
											}
										}

										// nocull dist for certain cull modes
										float temp_leaf_tweak_nocull_dist = 0.0f; // 0 = no override
										if (elem.contains("nocull_dist"))
										{
											temp_leaf_tweak_nocull_dist = common::toml_ext::to_float(elem.at("nocull_dist"));
											any_nocull_dist_overrides_in_leaf_tweaks = true;
										}

										temp_leaf_tweak_set.emplace_back(
											std::move(temp_in_leafs_set),
											std::move(temp_areas),
											std::move(temp_leafs),
											temp_leaf_tweak_nocull_dist);
									}
								}
							}

							m_map_settings.area_settings.emplace(area,
								area_overrides_s
								{
									std::move(leaf_set),
									std::move(area_set),
									std::move(hidden_leaf_set),
									std::move(temp_hidden_areas_set),
									std::move(temp_leaf_tweak_set),
									cmode,
									temp_nocull_dist,
									any_nocull_dist_overrides_in_leaf_tweaks,
									area
								});
						}
					};

				// try to find the loaded map
				if (cull_table.contains(m_map_settings.mapname))
				{
					if (const auto map = cull_table[m_map_settings.mapname];
						!map.is_empty() && !map.as_array().empty())
					{
						for (const auto& entry : map.as_array()) {
							process_cull_entry(entry);
						}
					}
				}
			} // end 'CULL'


			// ####################
			// parse 'HIDEMODEL' table
			if (config.contains("HIDEMODEL"))
			{
				// try to find the loaded map
				if (auto& hidemdl_table = config["HIDEMODEL"];
					hidemdl_table.contains(m_map_settings.mapname))
				{
					if (const auto map = hidemdl_table[m_map_settings.mapname];
						!map.is_empty())
					{
						if (map.contains("name"))
						{
							if (auto& names = map.at("name");
								!names.is_empty())
							{
								if (const auto& narray = map.at("name").as_array();
									!narray.empty())
								{
									for (auto& str : narray) {
										m_map_settings.hide_models.substrings.insert(str.as_string());
									}
								}
							}
						}

						if (map.contains("radius"))
						{
							if (auto& radii = map.at("radius");
								!radii.is_empty())
							{
								if (const auto& rarray = map.at("radius").as_array();
									!rarray.empty())
								{
									for (auto& r : rarray) {
										m_map_settings.hide_models.radii.insert(common::toml_ext::to_float(r, -1.0f));
									}
								}
							}
						}

					}
				}
			} // end 'HIDEMODEL'


			// ####################
			// parse 'UNBAKE' table
			if (config.contains("UNBAKE"))
			{
				auto& unbake_table = config["UNBAKE"];

				// try to find the loaded map
				if (unbake_table.contains(m_map_settings.mapname))
				{
					if (const auto map = unbake_table[m_map_settings.mapname];
						!map.is_empty())
					{
						if (map.contains("name"))
						{
							if (auto& names = map.at("name");
								!names.is_empty())
							{
								if (const auto& arr = map.at("name").as_array();
									!arr.empty())
								{
									for (auto& str : arr) {
										m_map_settings.unbake_models.insert(str.as_string());
									}
								}
							}
						}
					}
				}

				if (unbake_table.contains("ALL"))
				{
					if (auto& names = unbake_table.at("ALL");
						!names.is_empty())
					{
						if (const auto& arr = unbake_table.at("ALL").as_array();
							!arr.empty())
						{
							for (auto& str : arr) {
								m_map_settings.unbake_models.insert(str.as_string());
							}
						}
					}
				}
			} // end 'UNBAKE'


			// ####################
			// parse 'MARKER' table
			if (config.contains("MARKER"))
			{
				auto& marker_table = config["MARKER"];

				// #
				auto process_marker_entry = [](const toml::value& entry)
					{
						std::uint32_t temp_marker_index = 0u;

						if (entry.contains("marker")) 
						{
							temp_marker_index = static_cast<std::uint32_t>(common::toml_ext::to_int(entry.at("marker"), 0u));
							common::log("MapSettings", "Using deprecated marker system (index: " + std::to_string(temp_marker_index) + "). Transition to nocull!", common::LOG_TYPE::LOG_TYPE_WARN);
						}
						else if (entry.contains("nocull")) {
							temp_marker_index = static_cast<std::uint32_t>(common::toml_ext::to_int(entry.at("nocull"), 0u));
						}
						else
						{
							TOML_ERROR("[MARKER] #index", entry, "Marker did not define an index via 'marker' or 'nocull' -> skipping");
							return;
						}

						std::string temp_comment;
						if (!entry.comments().empty())
						{
							temp_comment = entry.comments().at(0);
							temp_comment.erase(0, 2); // rem '# '
						}

						if (entry.contains("position"))
						{
							if (const auto& pos = entry.at("position").as_array();
								pos.size() == 3)
							{
								Vector temp_rotation;
								Vector temp_scale = { 1.0, 1.0f, 1.0f };

								// optional
								if (entry.contains("rotation"))
								{
									if (const auto& rot = entry.at("rotation").as_array(); rot.size() == 3) {
										temp_rotation = { DEG2RAD(common::toml_ext::to_float(rot[0])), DEG2RAD(common::toml_ext::to_float(rot[1])), DEG2RAD(common::toml_ext::to_float(rot[2])) };
									}
									else { TOML_ERROR("[MARKER] #rotation", entry.at("rotation"), "expected a 3D vector but got => %d ", entry.at("rotation").as_array().size()); }
								}

								// optional
								if (entry.contains("scale"))
								{
									if (const auto& scale = entry.at("scale").as_array(); scale.size() == 3) {
										temp_scale = { common::toml_ext::to_float(scale[0]), common::toml_ext::to_float(scale[1]), common::toml_ext::to_float(scale[2]) };
									}
									else { TOML_ERROR("[MARKER] #scale", entry.at("scale"), "expected a 3D vector but got => %d ", entry.at("scale").as_array().size()); }
								}

								// optional
								std::unordered_set<std::uint32_t> temp_area_set;
								if (entry.contains("areas"))
								{
									if (const auto& areas = entry.at("areas").as_array(); !areas.empty())
									{
										for (const auto& a : areas) {
											temp_area_set.insert(common::toml_ext::to_int(a));
										}
									}
								}

								// optional
								std::unordered_set<std::uint32_t> temp_not_in_leaf_set;
								if (entry.contains("N_leafs"))
								{
									if (const auto& nleafs = entry.at("N_leafs").as_array(); !nleafs.empty())
									{
										for (const auto& nl : nleafs) {
											temp_not_in_leaf_set.insert(common::toml_ext::to_int(nl));
										}
									}
								}

								m_map_settings.map_markers.emplace_back(
									marker_settings_s
									{
										.index = temp_marker_index,
										.origin = {common::toml_ext::to_float(pos[0]), common::toml_ext::to_float(pos[1]), common::toml_ext::to_float(pos[2]) },
										.rotation = temp_rotation,
										.scale = temp_scale,
										.areas = std::move(temp_area_set),
										.when_not_in_leafs = std::move(temp_not_in_leaf_set),
										.comment = std::move(temp_comment)
									});
							}
							else { TOML_ERROR("[MARKER] #position", entry.at("position"), "expected a 3D vector but got => %d ", entry.at("position").as_array().size()); }
						}
					};

				// try to find the loaded map
				if (marker_table.contains(m_map_settings.mapname))
				{
					if (const auto map = marker_table[m_map_settings.mapname];
						!map.is_empty() && !map.as_array().empty())
					{
						for (const auto& entry : map.as_array()) {
							process_marker_entry(entry);
						}
					}
				}
			} // end 'MARKER'


			// ####################
			// parse 'CONFIGVARS' table
			{
				auto& configvar_table = config["CONFIGVARS"];

				auto process_transition_entry = [](const toml::value& entry)
					{
						// we NEED conf, leafs and duration or speed
						if (entry.contains("conf") && entry.contains("trigger") && (entry.contains("duration") || entry.contains("speed")))
						{
							std::string config_name;

							try { config_name = entry.at("conf").as_string(); }
							catch (toml::type_error& err)
							{
								common::log("MapSettings", std::format("{}", err.what()), common::LOG_TYPE::LOG_TYPE_ERROR, false);
								return;
							}

							if (!config_name.empty())
							{
								std::uint8_t mode = 0u;
								remix_vars::EASE_TYPE ease = remix_vars::EASE_TYPE_LINEAR;
								float delay_in = 0.0f, delay_out = 0.0f, duration = 0.0f;

								if (entry.contains("mode")) {
									mode = (std::uint8_t)common::toml_ext::to_int(entry.at("mode"));
								}

								if (entry.contains("ease")) {
									ease = (remix_vars::EASE_TYPE)common::toml_ext::to_int(entry.at("ease"));
								}

								if (entry.contains("delay_in")) {
									delay_in = common::toml_ext::to_float(entry.at("delay_in"));
								}

								if (entry.contains("delay_out")) {
									delay_out = common::toml_ext::to_float(entry.at("delay_out"));
								}

								if (entry.contains("duration")) {
									duration = common::toml_ext::to_float(entry.at("duration"));
								}

								const auto& trigger = entry.at("trigger");

								// choreo trigger
								if (trigger.contains("choreo"))
								{
									std::string choreo_name;
									std::string choreo_actor;
									std::string choreo_event;
									std::string choreo_param1;

									try { choreo_name = trigger.at("choreo").as_string(); }
									TOML_CATCH_TYPE_ERROR;

									if (trigger.contains("actor"))
									{
										try { choreo_actor = trigger.at("actor").as_string(); }
										TOML_CATCH_TYPE_ERROR;
									}

									if (trigger.contains("event"))
									{
										try { choreo_event = trigger.at("event").as_string(); }
										TOML_CATCH_TYPE_ERROR;
									}

									if (trigger.contains("param1"))
									{
										try { choreo_param1 = trigger.at("param1").as_string(); }
										TOML_CATCH_TYPE_ERROR;
									}

									if (!choreo_name.empty())
									{
										const auto hash = utils::string_hash64(utils::va("%s%s%.2f", choreo_name.c_str(), config_name.c_str(), duration));
										m_map_settings.remix_transitions.emplace_back(
											TRANSITION_TRIGGER_TYPE::CHOREO,
											std::move(choreo_name),
											std::move(choreo_actor),
											std::move(choreo_event),
											std::move(choreo_param1),
											0u,
											"",
											std::unordered_set<std::uint32_t>(),
											config_name,
											(TRANSITION_MODE)mode,
											ease,
											delay_in,
											delay_out,
											duration,
											hash);
									}
								}

								// sound trigger
								else if (trigger.contains("sound"))
								{
									std::uint32_t temp_sound_hash = 0u;
									std::string temp_sound_name;

									if (trigger.at("sound").type() == toml::value_t::integer) {
										temp_sound_hash = common::toml_ext::to_uint(trigger.at("sound"), 0u);
									}
									else
									{
										try { temp_sound_name = trigger.at("sound").as_string(); }
										TOML_CATCH_TYPE_ERROR;
									}

									const auto hash = utils::string_hash64(utils::va("%d%s%s%.2f", temp_sound_hash, temp_sound_name.c_str(), config_name.c_str(), duration));
									m_map_settings.remix_transitions.emplace_back(
										TRANSITION_TRIGGER_TYPE::SOUND,
										"",
										"",
										"",
										"",
										temp_sound_hash,
										std::move(temp_sound_name),
										std::unordered_set<std::uint32_t>(),
										config_name,
										(TRANSITION_MODE)mode,
										ease,
										delay_in,
										delay_out,
										duration,
										hash);
								}

								// leaf trigger
								else if (trigger.contains("leafs") && trigger.at("leafs").is_array())
								{
									std::unordered_set<std::uint32_t> leaf_set;
									const auto& leafs = trigger.at("leafs").as_array();
									if (!leafs.empty())
									{
										for (const auto& leaf : leafs) {
											leaf_set.insert(common::toml_ext::to_int(leaf));
										}

										// create a unique hash for this transition
										std::uint32_t leaf_sum = 0;
										for (const auto& leaf : leaf_set) {
											leaf_sum += leaf;
										}

										const auto hash = utils::string_hash64(utils::va("%d%s%.2f", leaf_sum, config_name.c_str(), duration));
										m_map_settings.remix_transitions.emplace_back(
											TRANSITION_TRIGGER_TYPE::LEAF,
											"",
											"",
											"",
											"",
											0u,
											"",
											std::move(leaf_set),
											config_name,
											(TRANSITION_MODE)mode,
											ease,
											delay_in,
											delay_out,
											duration,
											hash);
									}
								}
							}
						}
					};

				// try to find the loaded map
				if (configvar_table.contains(m_map_settings.mapname))
				{
					if (const auto map = configvar_table[m_map_settings.mapname];
						!map.is_empty())
					{
						if (map.contains("startup"))
						{
							if (auto& startup = map.at("startup").as_array();
								!startup.empty())
							{
								for (const auto& conf : startup)
								{
									try {
										m_map_settings.api_var_configs.emplace_back(conf.as_string());
									}
									catch (toml::type_error& err) {
										common::log("MapSettings", std::format("{}", err.what()), common::LOG_TYPE::LOG_TYPE_ERROR, false);
									}
								}
							}
						}

						if (map.contains("transitions"))
						{
							if (auto& transitions = map.at("transitions").as_array();
								!transitions.empty())
							{
								for (const auto& entry : transitions) {
									process_transition_entry(entry);
								}
							}
						}
					}
				}
			} // end 'CONFIGVARS'


			// ####################
			// parse 'PORTALS' table
			if (config.contains("PORTALS"))
			{
				auto& portal_table = config["PORTALS"];

				// #
				auto process_portal_pair_entry = [](const toml::value& entry)
					{
						if (entry.contains("pair") && entry.contains("portals"))
						{
							if (auto& portals = entry.at("portals");
								!portals.is_empty())
							{
								if (const auto& parray = entry.at("portals").as_array();
									!parray.empty() && parray.size() == 2)
								{
									if (parray[0].contains("position") && parray[0].contains("rotation") && parray[0].contains("scale") && parray[0].contains("square_mask")
										&& parray[1].contains("position") && parray[1].contains("rotation") && parray[1].contains("scale") && parray[1].contains("square_mask"))
									{
										const auto& p0_pos = parray[0].at("position").as_array();
										const auto& p0_rot = parray[0].at("rotation").as_array();
										const auto& p0_scale = parray[0].at("scale").as_array();
										const auto& p0_mask = common::toml_ext::to_int(parray[0].at("square_mask"));

										const auto& p1_pos = parray[1].at("position").as_array();
										const auto& p1_rot = parray[1].at("rotation").as_array();
										const auto& p1_scale = parray[1].at("scale").as_array();
										const auto& p1_mask = common::toml_ext::to_int(parray[1].at("square_mask"));

										if (p0_pos.size() == 3 && p0_rot.size() == 3 && p0_scale.size() == 2
											&& p1_pos.size() == 3 && p1_rot.size() == 3 && p1_scale.size() == 2)
										{
											remix_rayportal::get()->add_pair(
												(remix_rayportal::PORTAL_PAIR)static_cast<std::uint32_t>(common::toml_ext::to_int(entry.at("pair"))),
												{ common::toml_ext::to_float(p0_pos[0]),   common::toml_ext::to_float(p0_pos[1]), common::toml_ext::to_float(p0_pos[2]) },
												{ common::toml_ext::to_float(p0_rot[0]),   common::toml_ext::to_float(p0_rot[1]), common::toml_ext::to_float(p0_rot[2]) },
												{ common::toml_ext::to_float(p0_scale[0]), common::toml_ext::to_float(p0_scale[1]) },
												p0_mask,
												{ common::toml_ext::to_float(p1_pos[0]),   common::toml_ext::to_float(p1_pos[1]), common::toml_ext::to_float(p1_pos[2]) },
												{ common::toml_ext::to_float(p1_rot[0]),   common::toml_ext::to_float(p1_rot[1]), common::toml_ext::to_float(p1_rot[2]) },
												{ common::toml_ext::to_float(p1_scale[0]), common::toml_ext::to_float(p1_scale[1]) },
												p1_mask);
										}
									}
								}
							}
						}
					};


				if (common::remix_api::is_initialized())
				{
					// try to find the loaded map
					if (portal_table.contains(m_map_settings.mapname))
					{
						if (const auto& map = portal_table[m_map_settings.mapname];
							!map.is_empty() && !map.as_array().empty())
						{
							for (const auto& entry : map.as_array()) {
								process_portal_pair_entry(entry);
							}
						}
					}
				}
			} // end 'PORTALS'


			// ####################
			// parse 'LIGHTS' table
			if (config.contains("LIGHTS"))
			{
				auto& light_table = config["LIGHTS"];

				// #
				auto process_light_entry = [](const toml::value& entry)
					{
						if (entry.contains("points") && !entry.at("points").as_array().empty())
						{
							// - parse trigger

							std::string temp_trigger_choreo_name;
							std::string temp_trigger_choreo_actor;
							std::string temp_trigger_choreo_event;
							std::string temp_trigger_choreo_param1;

							std::uint32_t temp_trigger_sound = 0u;
							float temp_trigger_delay = 0.0f;
							bool temp_trigger_always = false;

							std::string temp_comment;
							if (!entry.comments().empty())
							{
								temp_comment = entry.comments().at(0);
								temp_comment.erase(0, 2); // rem '# '
							}

							if (entry.contains("trigger"))
							{
								bool has_valid_trigger = false;
								const auto& trigger = entry.at("trigger");

								// choreo trigger
								if (trigger.contains("choreo"))
								{
									try { temp_trigger_choreo_name = trigger.at("choreo").as_string(); }
									TOML_CATCH_TYPE_ERROR;

									if (trigger.contains("actor"))
									{
										try { temp_trigger_choreo_actor = trigger.at("actor").as_string(); }
										TOML_CATCH_TYPE_ERROR;
									}

									if (trigger.contains("event"))
									{
										try { temp_trigger_choreo_event = trigger.at("event").as_string(); }
										TOML_CATCH_TYPE_ERROR;
									}

									if (trigger.contains("param1"))
									{
										try { temp_trigger_choreo_param1 = trigger.at("param1").as_string(); }
										TOML_CATCH_TYPE_ERROR;
									}

									has_valid_trigger = true;
								}
								// sound trigger
								else if (trigger.contains("sound"))
								{
									temp_trigger_sound = common::toml_ext::to_uint(trigger.at("sound"), 0u);
									has_valid_trigger = true;
								}

								if (has_valid_trigger)
								{
									if (trigger.contains("delay")) {
										temp_trigger_delay = common::toml_ext::to_float(trigger.at("delay"), 0.0f);
									}

									if (trigger.contains("always")) {
										temp_trigger_always = common::toml_ext::to_bool(trigger.at("always"), false);
									}
								}
								else { TOML_ERROR("[LIGHTS] #trigger", trigger, "defined trigger with no choreo / sound hash"); }
							}

							// - parse kill

							std::string temp_kill_choreo_name;
							std::uint32_t temp_kill_sound = 0u;
							float temp_kill_delay = 0.0f;

							if (entry.contains("kill"))
							{
								bool has_valid_kill_trigger = false;
								const auto& kill = entry.at("kill");

								// choreo
								if (kill.contains("choreo"))
								{
									try { temp_kill_choreo_name = kill.at("choreo").as_string(); }
									TOML_CATCH_TYPE_ERROR;

									has_valid_kill_trigger = true;
								}
								// sound
								else if (kill.contains("sound"))
								{
									temp_kill_sound = common::toml_ext::to_uint(kill.at("sound"), 0u);
									has_valid_kill_trigger = true;
								}

								if (has_valid_kill_trigger)
								{
									if (kill.contains("delay")) {
										temp_kill_delay = common::toml_ext::to_float(kill.at("delay"), 0.0f);
									}
								}
								else { TOML_ERROR("[LIGHTS] #trigger", kill, "defined kill trigger with no choreo / sound hash"); }
							}

							// - parse points

							const auto& parray = entry.at("points").as_array();
							std::vector<remix_light_settings_s::point_s> temp_points;

							// for each point
							for (auto i = 0u; i < parray.size(); i++)
							{
								bool point_has_valid_position = false;

								const auto& p = parray[i];
								if (p.contains("position"))
								{
									if (const auto& positions = p.at("position").as_array(); positions.size() == 3) {
										point_has_valid_position = true;
									}
									else { TOML_ERROR("[LIGHTS] #position", p.at("position"), "expected a 3D vector but got => %d ", p.at("position").as_array().size()); }
								}

								if (!i && !point_has_valid_position) // first point needs to define a position
								{
									TOML_ERROR("[LIGHTS] #position", p, "first point needs to define a position! Ignoring light");
									break;
								}

								Vector temp_radiance = { 10.0f, 10.0f, 10.0f };
								if (p.contains("radiance"))
								{
									if (const auto& radiance = p.at("radiance").as_array(); radiance.size() == 3)
									{
										temp_radiance = Vector(common::toml_ext::to_float(radiance[0], 10.0f), common::toml_ext::to_float(radiance[1], 10.0f), common::toml_ext::to_float(radiance[2], 10.0f));
									}
									else { TOML_ERROR("[LIGHTS] #radiance", p.at("radiance"), "expected a 3D vector but got => %d ", p.at("radiance").as_array().size()); }
								}

								float temp_radiance_scalar = 1.0f;
								if (p.contains("scalar")) {
									temp_radiance_scalar = common::toml_ext::to_float(p.at("scalar"), 1.0f);
								}

								float temp_radius = 1.0f;
								if (p.contains("radius")) {
									temp_radius = common::toml_ext::to_float(p.at("radius"), 1.0f);
								}

								float temp_timepoint = 0.0f;
								if (i && p.contains("timepoint")) { // do not set timepoint for first point
									temp_timepoint = common::toml_ext::to_float(p.at("timepoint"), 0.0f);
								}

								float temp_smoothness = 0.5f;
								if (p.contains("smoothness"))
								{
									temp_smoothness = common::toml_ext::to_float(p.at("smoothness"), 0.5f);
									temp_smoothness = std::clamp<float>(temp_smoothness, 0.0f, 10.0f);
								}


								// shaping

								Vector temp_direction = { 0.0f, 0.0f, 1.0f };
								if (p.contains("direction"))
								{
									if (const auto& direction = p.at("direction").as_array(); direction.size() == 3)
									{
										temp_direction = Vector(common::toml_ext::to_float(direction[0], 0.0f), common::toml_ext::to_float(direction[1], 0.0f), common::toml_ext::to_float(direction[2], 1.0f));
										temp_direction.Normalize();
									}
									else { TOML_ERROR("[LIGHTS] #direction", p.at("direction"), "expected a 3D vector but got => %d ", p.at("direction").as_array().size()); }
								}

								Vector temp_angle_offset_attached = { 0.0f, 0.0f, 0.0f };
								if (p.contains("angle_offset_attached"))
								{
									if (const auto& angle_offset_attached = p.at("angle_offset_attached").as_array(); angle_offset_attached.size() == 3)
									{
										temp_angle_offset_attached = Vector(common::toml_ext::to_float(angle_offset_attached[0], 0.0f), common::toml_ext::to_float(angle_offset_attached[1], 0.0f), common::toml_ext::to_float(angle_offset_attached[2], 0.0f));
										utils::vector::angle_normalize(temp_angle_offset_attached);
									}
									else { TOML_ERROR("[LIGHTS] #angle_offset_attached", p.at("angle_offset_attached"), "expected a 3D vector but got => %d ", p.at("angle_offset_attached").as_array().size()); }
								}

								bool temp_shaping_enabled = false;
								float temp_degrees = 180.0f;
								if (p.contains("degrees"))
								{
									temp_degrees = common::toml_ext::to_float(p.at("degrees"), 180.0f);
									temp_degrees = std::clamp<float>(temp_degrees, 0.0f, 180.0f);
									temp_shaping_enabled = temp_degrees != 180.0f;
								}

								float temp_softness = 0.0f;
								if (p.contains("softness"))
								{
									temp_softness = common::toml_ext::to_float(p.at("softness"), 0.0f);
									temp_softness = std::clamp<float>(temp_softness, 0.0f, M_PI);
								}

								float temp_exponent = 0.0f;
								if (p.contains("exponent")) {
									temp_exponent = common::toml_ext::to_float(p.at("exponent"), 0.0f);
								}

								// volumetrics
								float temp_volumetric = 1.0f;
								if (p.contains("volumetric_scale")) { // volumetricRadianceScale
									temp_volumetric = common::toml_ext::to_float(p.at("volumetric_scale"), 1.0f);
								}

								// to avoid code duplication
								Vector pt;

								// using either position defined in current point or previous position
								if (point_has_valid_position)
								{
									const auto& positions = p.at("position").as_array();
									pt = Vector(common::toml_ext::to_float(positions[0]), common::toml_ext::to_float(positions[1]), common::toml_ext::to_float(positions[2]));
								}
								else {
									pt = temp_points.back().position; // pos of previous point
								}

								temp_points.emplace_back(
									remix_light_settings_s::point_s {
										.position = pt,
										.radiance = temp_radiance,
										.radiance_scalar = temp_radiance_scalar,
										.radius = temp_radius,
										.timepoint = temp_timepoint,
										.smoothness = temp_smoothness,
										.use_shaping = temp_shaping_enabled,
										.direction = temp_direction,
										.angle_offset_attached = temp_angle_offset_attached,
										.degrees = temp_degrees,
										.softness = temp_softness,
										.exponent = temp_exponent,
										.volumetric_scale = temp_volumetric }
									);
							}

							// attach settings

							float temp_attach_prop_radius = 0.0f;
							std::string temp_attach_prop_str;
							Vector temp_attach_prop_bounds_min;
							Vector temp_attach_prop_bounds_max;

							int temp_attach_bone_index = -1;
							std::string temp_attach_bone_str;

							if (entry.contains("attach"))
							{
								bool has_valid_attach = false;
								const auto& attach = entry.at("attach");

								if (attach.contains("radius"))
								{
									temp_attach_prop_radius = common::toml_ext::to_float(attach.at("radius"), 0.0f);
									has_valid_attach = true;
								}
								else if (attach.contains("name"))
								{
									try { temp_attach_prop_str = attach.at("name").as_string(); }
									TOML_CATCH_TYPE_ERROR;

									has_valid_attach = true;
								}

								if (has_valid_attach)
								{
									m_map_settings.using_any_light_attached_to_prop = true;

									if (attach.contains("bounds"))
									{
										if (const auto& bounds = attach.at("bounds").as_array();
											bounds.size() == 6u)
										{
											temp_attach_prop_bounds_min = Vector(common::toml_ext::to_float(bounds[0]), common::toml_ext::to_float(bounds[1]), common::toml_ext::to_float(bounds[2]));
											temp_attach_prop_bounds_max = Vector(common::toml_ext::to_float(bounds[3]), common::toml_ext::to_float(bounds[4]), common::toml_ext::to_float(bounds[5]));
										}
									}

									if (attach.contains("bone_index")) {
										temp_attach_bone_index = common::toml_ext::to_int(attach.at("bone_index"), -1);
									}

									if (attach.contains("bone_name"))
									{
										try { temp_attach_bone_str = attach.at("bone_name").as_string(); }
										TOML_CATCH_TYPE_ERROR;
									}
								}
							}

							// - parse general settings

							if (!temp_points.empty())
							{
								bool temp_run_once = false;
								if (entry.contains("run_once")) {
									temp_run_once = common::toml_ext::to_bool(entry.at("run_once"), false);
								}

								bool temp_loop = false;
								if (entry.contains("loop")) {
									temp_loop = common::toml_ext::to_bool(entry.at("loop"), false);
								}

								bool temp_loop_smoothing = false;
								if (entry.contains("loop_smoothing")) {
									temp_loop_smoothing = common::toml_ext::to_bool(entry.at("loop_smoothing"), false);
								}

								m_map_settings.remix_lights.push_back(
									remix_light_settings_s{
										.points = std::move(temp_points),
										.run_once = temp_run_once,
										.loop = temp_loop,
										.loop_smoothing = temp_loop_smoothing,
										.trigger_always = temp_trigger_always,

										.trigger_choreo_name = std::move(temp_trigger_choreo_name),
										.trigger_choreo_actor = std::move(temp_trigger_choreo_actor),
										.trigger_choreo_event = std::move(temp_trigger_choreo_event),
										.trigger_choreo_param1 = std::move(temp_trigger_choreo_param1),
										.trigger_sound_hash = temp_trigger_sound,
										.trigger_delay = temp_trigger_delay,

										.kill_choreo_name = std::move(temp_kill_choreo_name),
										.kill_sound_hash = temp_kill_sound,
										.kill_delay = temp_kill_delay,

										.attach_prop_radius = temp_attach_prop_radius,
										.attach_prop_name = std::move(temp_attach_prop_str),
										.attach_prop_mins = temp_attach_prop_bounds_min,
										.attach_prop_maxs = temp_attach_prop_bounds_max,
										.attach_bone_index = temp_attach_bone_index,
										.attach_bone_name = temp_attach_bone_str,

										.comment = std::move(temp_comment)
									});
							}
						}
						else { TOML_ERROR("[LIGHTS] #points", entry, "needs at least one point to define a light"); }
					};

				if (common::remix_api::is_initialized())
				{
					// try to find the loaded map
					if (light_table.contains(m_map_settings.mapname))
					{
						if (const auto& map = light_table[m_map_settings.mapname];
							!map.is_empty() && !map.as_array().empty())
						{
							for (const auto& entry : map.as_array()) {
								process_light_entry(entry);
							}
						}
					}
				}
			} // end 'LIGHTS'

			// ####################
			// parse 'CVARS' table
			if (config.contains("CVARS"))
			{
				auto& cvar_table = config["CVARS"];

				// try to find the loaded map
				if (cvar_table.contains(m_map_settings.mapname))
				{
					if (const auto map = cvar_table[m_map_settings.mapname];
						!map.is_empty() && map.is_array())
					{
						const auto& vars = map.as_array();
						for (auto& str : vars) {
							interfaces::get()->m_engine->execute_client_cmd_unrestricted(str.as_string().c_str());
						}
					}
				}
			} // end 'CVARS'
		} TOML_CATCH_SYNTAX_ERROR;
		
		return true;
	}

	bool map_settings::matches_map_name()
	{
		return utils::str_to_lower(m_args[0]) == m_map_settings.mapname;
	}

	void map_settings::open_and_set_var_config(const std::string& config, const bool no_error, const bool ignore_hashes, const char* custom_path)
	{
		std::string path = COMPMOD_ASSET_DIR "map_configs";
		if (custom_path) {
			path = custom_path;
		}

		std::ifstream file;
		if (utils::open_file_homepath(path, config, file))
		{
			common::log("MapSettings", std::format("Applying {}", config), common::LOG_TYPE::LOG_TYPE_DEFAULT, false);

			std::string input;
			while (std::getline(file, input))
			{
				if (utils::starts_with(input, "#")) {
					continue;
				}

				if (auto pair = utils::split(input, '=');
					pair.size() == 2u)
				{
					utils::trim(pair[0]);
					utils::trim(pair[1]);

					if (ignore_hashes && pair[1].starts_with("0x")) {
						continue;
					}

					if (pair[1].empty()) {
						continue;
					}

					if (const auto o = remix_vars::get_option(pair[0].c_str()); o)
					{
						const auto& v = remix_vars::string_to_option_value(o->second.type, pair[1]);
						remix_vars::set_option(o, v, true);
					}
				}
			}

			file.close();
		}
		else if (!no_error) {
			common::log("MapSettings", std::format("Failed to find/open config: '{}' in '{}'", config, custom_path ? custom_path : "'" COMPMOD_ASSET_DIR "map_configs'"), common::LOG_TYPE::LOG_TYPE_ERROR, false);
		}
	}

	void map_settings::on_map_load(const std::string& map_name)
	{
		if (m_loaded) {
			get()->clear_map_settings();
		}

		get()->set_settings_for_map(map_name);

	}

	void map_settings::on_map_unload()
	{
		if (const auto& imgui = imgui::get();  imgui->m_was_mapsettings_tab_open)
		{
			std::filesystem::create_directories(globals::root_path + COMPMOD_ASSET_DIR "logs\\");

			std::ofstream file;
			file.open((globals::root_path + COMPMOD_ASSET_DIR "logs\\autosave_mapsettings.toml").c_str());

			file << "# This file is autogenerated. It contains the the latest imgui map-setting changes.\n\n";

			file << "[CULL]\n";
			auto& areas = map_settings::get_map_settings().area_settings;
			file << "    " << common::toml_ext::build_culling_overrides_string_for_current_map(areas) << "\n\n";

			file << "[MARKER]\n";
			auto& markers = map_settings::get_map_settings().map_markers;
			file << "    " << common::toml_ext::build_map_marker_string_for_current_map(markers) << "\n\n";

			file << "[LIGHTS]\n";
			const auto lights = remix_lights::get();

			if (const auto edit_light = lights->get_first_active_light(); edit_light)
			{
				auto temp_def = edit_light->m_def;
				if (edit_light->m_mover.is_initialized()) {
					temp_def.points = edit_light->m_mover.get_points_vec();
				}

				file << "    " << common::toml_ext::build_light_string_for_single_light(temp_def) << "\n\n";
			}

			file.close();
		}

		get()->clear_map_settings();
	}

	void map_settings::clear_map_settings()
	{
		remix_rayportal::get()->destroy_all_pairs();

		m_map_settings.area_settings.clear();
		m_map_settings.hide_models.substrings.clear();
		m_map_settings.hide_models.radii.clear();
		m_map_settings.unbake_models.clear();
		m_map_settings.remix_transitions.clear();
		m_map_settings.map_markers.clear();
		m_map_settings.api_var_configs.clear();

		remix_lights::get()->destroy_and_clear_all_active_lights();
		m_map_settings.remix_lights.clear();
		m_map_settings.using_any_light_sound_hash = false;
		m_map_settings.using_any_transition_sound_hash = false;
		m_map_settings.using_any_transition_sound_name = false;

		m_map_settings = {};
		m_loaded = false;

		main_module::trigger_vis_logic();
	}

	ConCommand xo_mapsettings_update {};
	void map_settings::reload()
	{
		clear_map_settings();
		map_settings::get()->set_settings_for_map("");
		imgui::get()->m_light_edit_mode = false;
	}

	map_settings::map_settings()
	{
		p_this = this;
		game::con_add_command(&xo_mapsettings_update, "xo_mapsettings_update", map_settings::reload, "Reloads the map_settings.toml file + map.conf");

		// -----
		m_initialized = true;
		common::log("MapSettings", "Module initialized.", common::LOG_TYPE::LOG_TYPE_DEFAULT, false);
	}

#undef CATCH_ERR
}
