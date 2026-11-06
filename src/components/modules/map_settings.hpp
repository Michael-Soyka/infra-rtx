#pragma once
#include "remix_vars.hpp"
#include "utils/vector.hpp"

namespace components
{
	class map_settings final : public common::loader::component_module
	{
	public:
		map_settings();

		static inline map_settings* p_this = nullptr;
		static map_settings* get() { return p_this; }

		static bool is_initialized()
		{
			if (p_this && p_this->m_initialized) {
				return true;
			}
			return false;
		}

		enum TRANSITION_MODE : uint8_t
		{
			ONCE_ON_ENTER = 0,
			ONCE_ON_LEAVE = 1,
			ALWAYS_ON_ENTER = 2,
			ALWAYS_ON_LEAVE = 3,
		};

		enum TRANSITION_TRIGGER_TYPE : uint8_t
		{
			CHOREO = 0,
			SOUND = 1,
			LEAF = 2,
		};

		struct remix_transition_s
		{
			TRANSITION_TRIGGER_TYPE trigger_type;

			// choreo trigger
			std::string choreo_name;
			std::string choreo_actor;
			std::string choreo_event;
			std::string choreo_param1;

			// sound trigger
			std::uint32_t sound_hash;
			std::string sound_name;

			// leaf trigger
			std::unordered_set<std::uint32_t> leafs;

			std::string config_name;
			TRANSITION_MODE mode;
			remix_vars::EASE_TYPE interpolate_type;
			float delay_in = 0.0f;
			float delay_out = 0.0f;
			float duration = 0.0f;
			std::uint64_t hash;
			bool _state_enter = false;
		};

		struct marker_settings_s
		{
			std::uint32_t index = 0;
			Vector origin = {};
			Vector rotation = { 0.0f, 0.0f, 0.0f };
			Vector scale = { 1.0f, 1.0f, 1.0f }; // no_cull only
			std::unordered_set<std::uint32_t> areas; // no_cull only
			std::unordered_set<std::uint32_t> when_not_in_leafs; // no_cull only
			std::string comment;

			void* handle = nullptr; // internal use
			bool is_hidden = false; // internal use
		};

		struct api_config_var
		{
			std::string variable;
			std::string value;
		};

		struct api_texture_category_tweak
		{
			std::unordered_set<std::string> add_hashes;
			std::unordered_set<std::string> remove_hashes;
		};

		struct remix_light_settings_s
		{
			struct point_s
			{
				Vector position;
				Vector radiance;
				float radiance_scalar = 1.0f;
				float radius = 1.0f;
				float timepoint = 0.0f;
				float smoothness = 0.5f;

				// shaping
				bool use_shaping = false;
				Vector direction = { 0.0f, 0.0f, 1.0f };
				Vector angle_offset_attached = { 0.0f, 0.0f, 0.0f }; // offset light direction when attached to an entity or bone (Euler)
				float degrees = 180.0; // cone angle
				float softness = 0.0f; // cone
				float exponent = 0.0f; // focus

				// volumetric
				float volumetric_scale = 1.0f;
			};

			std::vector<point_s> points;
			bool run_once = false;
			bool loop = false;
			bool loop_smoothing = false;
			bool trigger_always = false;

			std::string trigger_choreo_name;
			std::string trigger_choreo_actor;
			std::string trigger_choreo_event;
			std::string trigger_choreo_param1;
			std::uint32_t trigger_sound_hash;
			float trigger_delay = 0.0f;

			std::string kill_choreo_name;
			std::uint32_t kill_sound_hash;
			float kill_delay = 0.0f;

			float attach_prop_radius = 0.0f;
			std::string attach_prop_name;
			Vector attach_prop_mins; // min bounds
			Vector attach_prop_maxs; // max bounds
			int attach_bone_index = -1;
			std::string attach_bone_name;

			std::string comment;
		};

		// ---

		static constexpr float DEFAULT_NOCULL_DIST = 600.0f;

		static constexpr const char* AREA_CULL_MODE_STR[] =
		{
			"NoFrustum",
			"NoFrstmInAr",
			"Stock",
			"ForceAr",
			"AreaDist",
			"Distance"
		};

		enum AREA_CULL_MODE : uint8_t
		{
			AREA_CULL_MODE_NO_FRUSTUM = 0,					// no frustum culling (everywhere)
			AREA_CULL_MODE_NO_FRUSTUM_IN_CURRENT_AREA = 1,	// no frustum culling in current area
			AREA_CULL_MODE_STOCK = 2,						// OG: frustum culling
			AREA_CULL_MODE_FORCE_AREA = 3,					// frustum culling (outside current area) + force all leafs/nodes in current area
			AREA_CULL_MODE_FORCE_AREA_DISTANCE = 4,			// frustum culling (outside current area) + force all leafs/nodes in current area and outside of current area within certain dist to player
			AREA_CULL_MODE_DISTANCE = 5,					// force all leafs/nodes within certain dist to player
			// -------------------
			AREA_CULL_INFO_COUNT = 6,
			AREA_CULL_INFO_DEFAULT = AREA_CULL_MODE_DISTANCE,
			AREA_CULL_INFO_NOCULLDIST_START = AREA_CULL_MODE_FORCE_AREA_DISTANCE,
			AREA_CULL_INFO_NOCULLDIST_END = AREA_CULL_MODE_DISTANCE,
		};

		struct leaf_tweak_s
		{
			std::unordered_set<std::uint32_t> in_leafs;
			std::unordered_set<std::uint32_t> areas;
			std::unordered_set<std::uint32_t> leafs;
			float nocull_dist = 0.0f;
		};

		struct hide_area_s
		{
			std::unordered_set<std::uint32_t> areas;
			std::unordered_set<std::uint32_t> when_not_in_leafs;
		};

		struct area_overrides_s
		{
			std::unordered_set<std::uint32_t> leafs;
			std::unordered_set<std::uint32_t> areas;
			std::unordered_set<std::uint32_t> hide_leafs;
			std::vector<hide_area_s> hide_areas;
			std::vector<leaf_tweak_s> leaf_tweaks;
			AREA_CULL_MODE cull_mode;
			float nocull_distance = DEFAULT_NOCULL_DIST;
			bool nocull_distance_overrides_in_leaf_twk = false;
			std::uint32_t area_index;
		};

		struct hide_models_s
		{
			std::unordered_set<std::string> substrings;
			std::unordered_set<float> radii;
		};

		struct sun_settings_s
		{
			float intensity = 1.0f;
			float diameter = 1.0f;
			bool custom_direction = false;
			float pitch = 0.0f;
			float yaw = 0.0f;

			bool operator==( const sun_settings_s & ) const = default;
		};

		struct map_settings_s
		{
			std::string	mapname;
			float fog_dist = 0.0f;
			float fog_density = 0.0f;
			DWORD fog_color = 0xFFFFFFFF;
			float water_uv_scale = 1.0f;
			float water_uv_top_scale = 0.0f;
			float water_offset_top = 0.5f; // top layer
			float water_offset_bottom = 0.0f; // bottom layer
			sun_settings_s sun;
			std::unordered_map<std::uint32_t, area_overrides_s> area_settings;
			float default_nocull_dist = DEFAULT_NOCULL_DIST;
			hide_models_s hide_models;
			std::unordered_set<std::string> unbake_models;
			std::vector<remix_transition_s> remix_transitions;
			std::vector<marker_settings_s> map_markers;
			std::vector<std::string> api_var_configs;
			std::unordered_map<std::string, api_texture_category_tweak> api_texture_category_tweaks;
			std::vector<remix_light_settings_s> remix_lights;
			bool using_any_light_sound_hash = false;
			bool using_any_light_attached_to_prop = false;
			bool using_any_transition_sound_hash = false;
			bool using_any_transition_sound_name = false;
		};

		static map_settings_s& get_map_settings() { return m_map_settings; }
		static const std::string& get_map_name() { return m_map_settings.mapname; }

		void set_settings_for_map(const std::string& map_name);
		static void on_map_load(const std::string& map_name);
		static void on_map_unload();
		static void clear_map_settings();
		static void reload();

	private:
		bool m_initialized = false;

		static inline map_settings_s m_map_settings = {};
		static inline std::vector<std::string> m_args;
		static inline bool m_loaded = false;

		bool parse_toml();
		bool matches_map_name();
		void open_and_set_var_config(const std::string& config, bool no_error = false, bool ignore_hashes = false, const char* custom_path = nullptr);
	};
}
