#include "std_include.hpp"

#include "components/common/flags.hpp"
#include "components/common/remix_api.hpp"
#include "components/modules/choreo_events.hpp"
#include "components/modules/game_settings.hpp"
#include "components/modules/imgui.hpp"
#include "components/modules/interfaces.hpp"
#include "components/modules/main_module.hpp"
#include "components/modules/model_render.hpp"
#include "components/modules/remix_lights.hpp"
#include "components/modules/remix_rayportal.hpp"
#include "components/modules/sound_events.hpp"

namespace p2
{
	// -------------------------------------------
	// game variables

	// - server


	// - engine
	int* r_visframecount = nullptr;
	int* visible_areas_num = nullptr;
	std::uint16_t* visible_areas = nullptr;
	bool* viewer_in_solid_space = nullptr;
	CPortalRect* area_rect = nullptr;
	DWORD* area_frustum = nullptr;
	Frustum_t* g_frustum = nullptr;
	DWORD* engine_renderer_ptr = nullptr;
	DWORD* hoststate_worldbrush_data_ptr = nullptr;
	DWORD* cstatic_prop_mgr_ptr = nullptr;
	Vector* current_view_origin = nullptr;
	Vector* current_view_forward = nullptr;
	DWORD* cclientstate_ptr = nullptr;

	// - client
	Vector4D* s_viewFadeColor = nullptr;
	DWORD* global_vars_ptr = nullptr;
	view_id* current_view_id = nullptr;
	DWORD* puzzlemaker_ptr = nullptr;

	// - shaderapidx9
	DWORD* d3d_device_ptr = nullptr;
	DWORD* shaderapi_ptr = nullptr;

	// -------------------------------------------
	// game functions

	// - server


	// - engine
	R_CullNode_t R_CullNode = nullptr;
	CM_LeafArea_t CM_LeafArea = nullptr;
	ComputeViewMatrices_t ComputeViewMatrices = nullptr;
	GetBaseLocalClient_t GetBaseLocalClient = nullptr;
	CClientState__IsPaused_t CClientState__IsPaused = nullptr;

	// - client
	GetSplitScreenViewPlayer_t GetSplitScreenViewPlayer = nullptr;
	ToPortalPlayer_t ToPortalPlayer = nullptr;
	CPortalRenderable__AddToVisAsExitPortal_t CPortalRenderable__AddToVisAsExitPortal = nullptr;
	CViewRender__ViewDrawScene_t CViewRender__ViewDrawScene = nullptr;

	// - shaderapidx9
	//


	// -------------------------------------------
	// game asm offsets

	// - server
	uint32_t hk_addr__scene_ent_on_start_event = 0u;
	uint32_t hk_addr__scene_ent_on_finish_event = 0u;
	uint32_t hk_addr__spawn_all_entities = 0u;
	uint32_t retn_addr__spawn_all_entities = 0u;

	// - engine
	uint32_t hk_addr__on_map_load = 0u;
	uint32_t hk_addr__on_host_disconnect = 0u;
	uint32_t hk_addr__on_host_change_level = 0u;
	uint32_t hk_addr__pre_recursive_world_node = 0u;
	uint32_t hk_addr__while_recursive_world_node = 0u;
	uint32_t hk_addr__while_recursive_world_node_og_retn = 0u;
	uint32_t hk_addr__while_recursive_world_node_force_retn = 0u;
	uint32_t mod_addr__while_recursive_world_node_01 = 0u;
	uint32_t nop_addr__while_recursive_world_node_02 = 0u;
	uint32_t mod_addr__while_recursive_world_node_03 = 0u;
	uint32_t nop_addr__draw_leaf = 0u;
	uint32_t nop_addr__cbrushbatchrender_draw_opaque_bmodel = 0u;
	uint32_t nop_addr__draw_displacements_in_leaf = 0u;
	uint32_t mod_addr__shader_draw_chains = 0u;
	uint32_t mod_addr__cbrushbatchrender_draw_opaque_bmodel = 0u;
	uint32_t nop_addr__cbrushbatchrender_compute_lmap_pages = 0u;
	uint32_t hk_addr__flow_through_area = 0u;
	uint32_t nop_addr__flow_through_area = 0u;
	uint32_t mod_addr__cstaticpropmgr_update_prop_visibility = 0u;
	uint32_t nop_addr__cstaticpropmgr_update_prop_visibility = 0u;
	uint32_t hk_addr__on_start_sound = 0u;
	uint32_t fn_addr__debug_overlay_add_text = 0u;
	uint32_t fn_addr__debug_overlay_add_text_colored = 0u;
	uint32_t fn_addr__r_flow_through_area = 0u;
	uint32_t fn_addr__frustum_set_planes = 0u;
	uint32_t fn_addr__frustum_cull_box = 0u;
	uint32_t fn_addr__cm_point_leaf_num = 0u;
	uint32_t fn_addr__override_view_frustum = 0u;
	uint32_t fn_addr__cull_node_simd = 0u;

	// - client
	uint32_t hk_addr__cviewrenderer_renderview = 0u;
	uint32_t hk_addr__cviewrenderer_drawonemonitor = 0u;
	uint32_t mod_addr__clientleafsystem_extract_culled_renderables = 0u;
	uint32_t nop_addr__vguiscreen_draw_model = 0u;
	uint32_t nop_addr__csimpleworldview_setup = 0u;
	uint32_t mod_addr__csimpleworldview_setup = 0u;
	uint32_t hk_addr__draw_our_3rd_person_body_mesh = 0u;
	uint32_t hk_addr__draw_our_3rd_person_weapon_mesh = 0u;
	uint32_t fn_addr__draw_our_3rd_person_weapon_mesh = 0u;
	uint32_t hk_addr__viewdrawscene_push_args = 0u;
	uint32_t hk_addr__cbaseworldview_draw_setup = 0u;
	uint32_t hk_addr__cbaseentity_update_visibility = 0u;
	uint32_t hk_addr__cbaseentity_update_visibility_retn = 0u;
	uint32_t nop_addr__write_savegame_screenshot_of_size = 0u;
	uint32_t mod_addr__envprojectedtexture_should_update = 0u;
	uint32_t hk_addr__cviewrenderer_init_fade_data = 0u;
	uint32_t mod_addr__cbaseentity_should_draw = 0u;
	uint32_t fn_addr__add_console_cmd = 0u;
	uint32_t fn_addr__get_map_name = 0u;
	uint32_t fn_addr__get_bone_transform = 0u;
	uint32_t fn_addr__lookup_bone = 0u;
	uint32_t fn_addr__get_model_ptr = 0u;
	uint32_t mod_addr__areaportalwindow_draw_model = 0u;
	uint32_t hk_addr__render_sprite_card_rope = 0u;
	uint32_t hk_addr__render_sprite_card_rope_normal = 0u;
	uint32_t hk_addr__render_sprite_card_rope_normal_cache = 0u;
	uint32_t hk_addr__render_sprites_trail = 0u;
	uint32_t hk_addr__render_sprite_card_new = 0u;
	uint32_t hk_addr__rope_draw_render_cache_flush = 0u;
	uint32_t hk_addr__rope_draw_render_cache_end = 0u;

	// - shaderapidx9
	uint32_t nop_addr__cshadermgr_set_pixel_shader = 0u;
	uint32_t hk_addr__cmeshdx8_renderpass_pre_draw = 0u;
	uint32_t hk_addr__cmeshdx8_renderpass_post_draw = 0u;
	uint32_t hk_addr__cmeshdx8_renderpass_for_instances = 0u;

	// - studiorender
	uint32_t hk_addr__studio_software_process_mesh = 0u;
	uint32_t hk_addr__studio_software_process_mesh_restore = 0u;
	uint32_t hk_addr__studio_render_final = 0u;
	uint32_t hk_addr__studio_draw_points = 0u;


	// -------------------------------------------

#define PATTERN_OFFSET_SIMPLE(mod, var, pattern, byte_offset, static_addr) \
		if (const auto offset = utils::mem::find_pattern(mod, ##pattern, byte_offset, #var, use_pattern, static_addr); offset) { \
			(var) = offset; found_pattern_count++; \
		} total_pattern_count++;

#define PATTERN_OFFSET_SIMPLE_CAST(mod, var, type, pattern, byte_offset, static_addr) \
		if (const auto offset = utils::mem::find_pattern(mod, ##pattern, byte_offset, #var, use_pattern, static_addr); offset) { \
			(var) = (type)offset; found_pattern_count++; \
		} total_pattern_count++;

#define PATTERN_OFFSET_DWORD_PTR_CAST_TYPE(mod, var, type, pattern, byte_offset, static_addr) \
		if (const auto offset = utils::mem::find_pattern(mod, ##pattern, byte_offset, #var, use_pattern, static_addr); offset) { \
			(var) = (type)*(DWORD*)offset; found_pattern_count++; \
		} total_pattern_count++;

	// init any adresses here :: returns false if a pattern is missing
	bool init_game_addresses()
	{
		const bool use_pattern = !common::flags::has_flag("no_pattern") && IS_LATEST_BUILD;
		const bool validate_patterns = common::flags::has_flag("validate_patterns");

		if (use_pattern) {
			common::log("P2", "Getting offsets ...", common::LOG_TYPE::LOG_TYPE_DEFAULT, false);
		}

		std::uint32_t total_pattern_count = 0u;
		std::uint32_t found_pattern_count = 0u;

#pragma region GAME_VARIABLES

		// --------------------------
		// - server - variables


		// - server - functions


		// - server - asm
		PATTERN_OFFSET_SIMPLE(SERVER_MOD, hk_addr__scene_ent_on_start_event, "8B 5D ? 57 8B F9 68 ? ? ? ? 8B CB E8 ? ? ? ? 50 E8 ? ? ? ? 83 C4 ? 85 C0 0F 84", 0, USE_OFFSET(0x1F6954, 0x22D428));
		PATTERN_OFFSET_SIMPLE(SERVER_MOD, hk_addr__scene_ent_on_finish_event, "8B 86 ? ? ? ? 85 C0 75 ? B8 ? ? ? ? F3 0F 10 86 ? ? ? ? 53", 0, USE_OFFSET(0x1FB653, 0x232273));
		PATTERN_OFFSET_SIMPLE(SERVER_MOD, hk_addr__spawn_all_entities, "85 C0 74 ? 6A ? 50 E8 ? ? ? ? 83 C4 ? 85 C0", 0, USE_OFFSET(0x16EFC0, 0x19A870));
		if (hk_addr__spawn_all_entities) {
			retn_addr__spawn_all_entities = USE_OFFSET(utils::mem::resolve_relative_jump_address(hk_addr__spawn_all_entities + 2u, 2u, 1u), SERVER_BASE + 0x19A8B4); found_pattern_count++;
		} total_pattern_count++;

		// --------------------------
		// - engine - variables
		PATTERN_OFFSET_DWORD_PTR_CAST_TYPE(ENGINE_MOD, r_visframecount, int*, "3B 05 ? ? ? ? 0F 85 ? ? ? ? 56", 2, USE_OFFSET(0xD40D3, 0xDFB33));
		PATTERN_OFFSET_DWORD_PTR_CAST_TYPE(ENGINE_MOD, visible_areas_num, int*, "8B 0D ? ? ? ? 66 89 04 4D ? ? ? ?", 2, USE_OFFSET(0x1031DB, 0x10E13E)); // ENGINE_BASE + USE_OFFSET(0x5FE410, 0x615468)
		PATTERN_OFFSET_DWORD_PTR_CAST_TYPE(ENGINE_MOD, visible_areas, std::uint16_t*, "8B 0D ? ? ? ? 66 89 04 4D ? ? ? ?", 10, USE_OFFSET(0x1031DB, 0x10E188)); // ENGINE_BASE + USE_OFFSET(0x5FE418, 0x615470)
		PATTERN_OFFSET_DWORD_PTR_CAST_TYPE(ENGINE_MOD, viewer_in_solid_space, bool*, "80 3D ? ? ? ? ? 8B 4D", 2, USE_OFFSET(0x102F93, 0x10E7E3)); // ENGINE_BASE + USE_OFFSET(0x5FFA58, 0x616AB0)
		PATTERN_OFFSET_DWORD_PTR_CAST_TYPE(ENGINE_MOD, area_rect, CPortalRect*, "81 C1 ? ? ? ? 66 0F D6 01", 2, USE_OFFSET(0x103203, 0x10E1B4)); // ENGINE_BASE + USE_OFFSET(0x5FE618, 0x615670)
		PATTERN_OFFSET_DWORD_PTR_CAST_TYPE(ENGINE_MOD, area_frustum, DWORD*, "A3 ? ? ? ? 89 35 ? ? ? ? 39 35", 1, USE_OFFSET(0x10316D, 0x10E9AD)); // ENGINE_BASE + USE_OFFSET(0x5FFB8C, 0x616BE4) + 0x10 to pElements
		PATTERN_OFFSET_DWORD_PTR_CAST_TYPE(ENGINE_MOD, g_frustum, Frustum_t*, "B9 ? ? ? ? E8 ? ? ? ? 84 C0 75 ? ? ? ? 57", 1, USE_OFFSET(0xDAE08, 0xE6378)); // ENGINE_BASE + USE_OFFSET(0x5F8C00, 0x60FC20)
		PATTERN_OFFSET_DWORD_PTR_CAST_TYPE(ENGINE_MOD, engine_renderer_ptr, DWORD*, "8B 0D ? ? ? ? ? ? 8B 50 ? FF D2 89 45 ? 8B 45", 2, USE_OFFSET(0x68596, 0x70D26)); // ENGINE_BASE + USE_OFFSET(0x40F220, 0x60F880)
		PATTERN_OFFSET_DWORD_PTR_CAST_TYPE(ENGINE_MOD, hoststate_worldbrush_data_ptr, DWORD*, "A1 ? ? ? ? 8D 50 ? 52 50 E8 ? ? ? ? A1", 1, USE_OFFSET(0x7048B, 0x78C89)); // ENGINE_BASE + USE_OFFSET(0x418BC4, 0x439C1C)
		
		if constexpr (IS_LATEST_BUILD) // (ENGINE_BASE + USE_OFFSET(0x442998, 0x43D490)
		{
			if (auto offset = utils::mem::find_pattern(ENGINE_MOD, "E8 ? ? ? ? ? ? 8B C8 8B 42 ? 56 FF D0 84 C0", 0, "cstatic_prop_mgr_ptr", use_pattern, 0xB4F2); offset) 
			{
				// pattern to fn call -> fn access of cstatic_prop_mgr_ptr at fn+1u
				offset = utils::mem::resolve_relative_call_address(offset) + 1u;
				cstatic_prop_mgr_ptr = (DWORD*)*(DWORD*)(offset); found_pattern_count++;
			} total_pattern_count++;
		} else {
			cstatic_prop_mgr_ptr = (DWORD*)(ENGINE_BASE + 0x43D490);
		}

		PATTERN_OFFSET_DWORD_PTR_CAST_TYPE(ENGINE_MOD, current_view_origin, Vector*, "F3 0F 10 0D ? ? ? ? F3 0F 10 15 ? ? ? ? F3 0F 10 1D", 12, USE_OFFSET(0x68E07, 0xD3E29)); // ENGINE_BASE + USE_OFFSET(0x4F6A90, 0x50DB50)
		PATTERN_OFFSET_DWORD_PTR_CAST_TYPE(ENGINE_MOD, current_view_forward, Vector*, "68 ? ? ? ? 68 ? ? ? ? E8 ? ? ? ? 83 C4 ? 56 B9", 1, USE_OFFSET(0xD22C1, 0xDDE2F)); // ENGINE_BASE + USE_OFFSET(0x40EFE4, 0x42FFE4)
		

		// - engine - functions
		PATTERN_OFFSET_SIMPLE_CAST(ENGINE_MOD, R_CullNode, R_CullNode_t, "55 8B EC 80 3D ? ? ? ? ? 8B 4D", 0, USE_OFFSET(0x102F90, 0x10E7E0));
		PATTERN_OFFSET_SIMPLE_CAST(ENGINE_MOD, CM_LeafArea, CM_LeafArea_t, "55 8B EC 8B 45 ? 8B 0D ? ? ? ? 03 C0 66 8B 54 C1", 0, USE_OFFSET(0x14BCE0, 0x159470));
		PATTERN_OFFSET_SIMPLE_CAST(ENGINE_MOD, ComputeViewMatrices, ComputeViewMatrices_t, "55 8B EC 0F 57 D2 F3 0F 10 1D", 0, USE_OFFSET(0xD1860, 0xDD4A0));
		PATTERN_OFFSET_SIMPLE_CAST(ENGINE_MOD, GetBaseLocalClient, GetBaseLocalClient_t, "A1 ? ? ? ? 83 C0 ? C3", 0, USE_OFFSET(0x96020, 0x9E7E0));
		PATTERN_OFFSET_SIMPLE_CAST(ENGINE_MOD, CClientState__IsPaused, CClientState__IsPaused_t, "80 B9 ? ? ? ? ? 75 ? 80 3D", 0, USE_OFFSET(0xA0910, 0xAB140));

		// - engine - asm
		PATTERN_OFFSET_SIMPLE(ENGINE_MOD, hk_addr__on_map_load, "8D 4F ? 33 D2", 0, USE_OFFSET(0xF2C0C, 0xFCD5C));
		PATTERN_OFFSET_SIMPLE(ENGINE_MOD, hk_addr__on_host_disconnect, "8B EC 83 EC ? 53 8B 5D ? 57 BF", 0, USE_OFFSET(0x1891E1, 0x197DF1));
		PATTERN_OFFSET_SIMPLE(ENGINE_MOD, hk_addr__on_host_change_level, "68 ? ? ? ? 8D 8D ? ? ? ? 56 51 E8 ? ? ? ? 83 C4 ? 83 3D", 0, USE_OFFSET(0x184A6F, 0x193C6D));
		PATTERN_OFFSET_SIMPLE(ENGINE_MOD, hk_addr__pre_recursive_world_node, "8B 52 ? 8B CB E8", 0, USE_OFFSET(0xDB7ED, 0xE6D6D));
		PATTERN_OFFSET_SIMPLE(ENGINE_MOD, hk_addr__while_recursive_world_node, "8B 4B ? 3B 0D", 0, USE_OFFSET(0xDB376, 0xE68E6));
		
		if (hk_addr__while_recursive_world_node) {
			hk_addr__while_recursive_world_node_og_retn = USE_OFFSET(utils::mem::resolve_relative_jump_address(hk_addr__while_recursive_world_node + 9u, 6u, 2u), ENGINE_BASE + 0xE6A42); found_pattern_count++;
		} total_pattern_count++;

		if (hk_addr__while_recursive_world_node) {
			hk_addr__while_recursive_world_node_force_retn = USE_OFFSET(utils::mem::resolve_relative_jump_address(hk_addr__while_recursive_world_node + 18u, 2u, 1u), ENGINE_BASE + 0xE690B); found_pattern_count++;
		} total_pattern_count++;

		PATTERN_OFFSET_SIMPLE(ENGINE_MOD, mod_addr__while_recursive_world_node_01, "7C ? 53 E8 ? ? ? ? 83 C4 ? 84 C0 0F 85", 0, USE_OFFSET(0xDB388, 0xE68F8));
		PATTERN_OFFSET_SIMPLE(ENGINE_MOD, nop_addr__while_recursive_world_node_02, "74 ? ? ? F7 C1 ? ? ? ? 75", 0, USE_OFFSET(0xDB453, 0xE69C3));
		PATTERN_OFFSET_SIMPLE(ENGINE_MOD, mod_addr__while_recursive_world_node_03, "75 ? 8B D1 C1 EA", 0, USE_OFFSET(0xDB45D, 0xE69CD));
		PATTERN_OFFSET_SIMPLE(ENGINE_MOD, nop_addr__draw_leaf, "0F 87 ? ? ? ? 8B D1", 0, USE_OFFSET(0xDB053, 0xE65C3));
		PATTERN_OFFSET_SIMPLE(ENGINE_MOD, nop_addr__cbrushbatchrender_draw_opaque_bmodel, "74 ? 0F BF 4B ? 85 C9", 0, USE_OFFSET(0x68DDE, 0x7156E));
		PATTERN_OFFSET_SIMPLE(ENGINE_MOD, nop_addr__draw_displacements_in_leaf, "75 ? ? ? ? 57 74", 0, USE_OFFSET(0xDAE14, 0xE6384));
		PATTERN_OFFSET_SIMPLE(ENGINE_MOD, mod_addr__shader_draw_chains, "75 ? ? ? ? 8B 42 ? 8B CE", 0, USE_OFFSET(0xDD3ED, 0xE8C4D));
		PATTERN_OFFSET_SIMPLE(ENGINE_MOD, mod_addr__cbrushbatchrender_draw_opaque_bmodel, "74 ? 80 7D ? ? 74 ? ? ? 8B 42 ? 6A", 0, USE_OFFSET(0x68DAA, 0x7153A));
		PATTERN_OFFSET_SIMPLE(ENGINE_MOD, nop_addr__cbrushbatchrender_compute_lmap_pages, "74 ? 85 C9 7E ? 8B 45", 0, USE_OFFSET(0x66250, 0x6E710));
		PATTERN_OFFSET_SIMPLE(ENGINE_MOD, hk_addr__flow_through_area, "E8 ? ? ? ? 83 C4 ? EB ? 8B 0D ? ? ? ? C6 05", 0, USE_OFFSET(0x103A0E, 0x10F0EC));
		PATTERN_OFFSET_SIMPLE(ENGINE_MOD, nop_addr__flow_through_area, "80 3D ? ? ? ? ? 75 ? E8 ? ? ? ? 5F", 9, USE_OFFSET(0x103A54, 0x10F11D));
		PATTERN_OFFSET_SIMPLE(ENGINE_MOD, mod_addr__cstaticpropmgr_update_prop_visibility, "78 ? 8A 47 ? 84 C0", 0, USE_OFFSET(0x1D7490, 0x1ED3F0));
		PATTERN_OFFSET_SIMPLE(ENGINE_MOD, nop_addr__cstaticpropmgr_update_prop_visibility, "75 ? 66 83 7F ? ? 74", 0, USE_OFFSET(0x1D751F, 0x1ED47F));
		PATTERN_OFFSET_SIMPLE(ENGINE_MOD, hk_addr__on_start_sound, "53 89 43 ? E8 ? ? ? ? 83 C4 ? 5E 5F 5B 8B E5 5D C3", 12, USE_OFFSET(0x190C1, 0x1BD20));

		if constexpr (IS_LATEST_BUILD)
		{
			if (const auto offset = utils::mem::find_pattern(ENGINE_MOD, "E8 ? ? ? ? 83 C4 ? 8B 0D ? ? ? ? ? ? 8B 52 ? 8D 85 ? ? ? ? 50 ? ? 57", 0, "fn_addr__debug_overlay_add_text", use_pattern, 0x189C1); offset) 
			{
				fn_addr__debug_overlay_add_text = utils::mem::resolve_relative_call_address(offset); found_pattern_count++;
				if (validate_patterns)
				{
					if (ENGINE_BASE + 0xB9250 == fn_addr__debug_overlay_add_text) {
						log("Hook", std::format("> Pattern offset validated!", ENGINE_BASE + 0xB9250), common::LOG_TYPE::LOG_TYPE_GREEN, true);
					} else {
						log("Hook", std::format("> Pattern offset invalid!", ENGINE_BASE + 0xB9250), common::LOG_TYPE::LOG_TYPE_ERROR, true);
					}
				}
			} total_pattern_count++;

			// overload is next to AddTextOverlay, too similar to its siblings for a pattern
			fn_addr__debug_overlay_add_text_colored = fn_addr__debug_overlay_add_text + 0x4A0u;
		}
		else
		{
			fn_addr__debug_overlay_add_text = ENGINE_BASE + 0xC3FE0;
			fn_addr__debug_overlay_add_text_colored = ENGINE_BASE + 0xC4460;
		}

		PATTERN_OFFSET_SIMPLE(ENGINE_MOD, fn_addr__r_flow_through_area, "55 8B EC 8B 45 ? 8B 15 ? ? ? ? 81 EC", 0, USE_OFFSET(0x1031C0, 0x10EA00));
		PATTERN_OFFSET_SIMPLE(ENGINE_MOD, fn_addr__frustum_set_planes, "55 8B EC 8B 55 ? 52", 0, USE_OFFSET(0x252770, 0x26CED0));
		PATTERN_OFFSET_SIMPLE(ENGINE_MOD, fn_addr__frustum_cull_box, "53 8B DC 83 EC ? 83 E4 ? 83 C4 ? 55 8B 6B ? 89 6C 24 ? 8B EC 83 EC ? 8B 43 ? ? ? ? 8B 53", 0, USE_OFFSET(0x252820, 0x26CF80));
		PATTERN_OFFSET_SIMPLE(ENGINE_MOD, fn_addr__override_view_frustum, "55 8B EC B9 ? ? ? ? 5D E9 ? ? ? ? CC CC 8B 0D ? ? ? ? 8B 01 8B 90", 0, USE_OFFSET(0xD0C90, 0xDC8F0));

		if constexpr (IS_LATEST_BUILD)
		{
			if ( const auto offset = utils::mem::find_pattern( ENGINE_MOD, "51 50 E8 ? ? ? ? 83 C4 08 5D C3", 2, "fn_addr__cull_node_simd", use_pattern, 0x102FD4 ); offset )
			{
				fn_addr__cull_node_simd = utils::mem::resolve_relative_call_address( offset ); found_pattern_count++;
			} total_pattern_count++;
		}
		else {
			fn_addr__cull_node_simd = ENGINE_BASE + 0xC0260;
		}

		PATTERN_OFFSET_SIMPLE(ENGINE_MOD, fn_addr__cm_point_leaf_num, "55 8B EC 83 3D ? ? ? ? ? 75 ? 33 C0 5D C3 8B 45", 0, USE_OFFSET(0x14ADB0, 0x158540)); // ENGINE_BASE + USE_OFFSET(0x159C80, 0x158540)

		// --------------------------
		// - client - variables
		PATTERN_OFFSET_DWORD_PTR_CAST_TYPE(CLIENT_MOD, s_viewFadeColor, Vector4D*, "81 C1 ? ? ? ? ? ? ? ? 0F 57 C9", 2, USE_OFFSET(0x186833, 0x1DB213));
		PATTERN_OFFSET_DWORD_PTR_CAST_TYPE(CLIENT_MOD, global_vars_ptr, DWORD*, "A1 ? ? ? ? F3 0F 10 48 ? F3 0F 5C 4D", 1, USE_OFFSET(0x6695, 0x3E335)); // CLIENT_BASE + USE_OFFSET(0x5E5704, 0x9220BC)
		PATTERN_OFFSET_DWORD_PTR_CAST_TYPE(CLIENT_MOD, current_view_id, view_id*, "89 3D ? ? ? ? FF D0 8B 0D", 2, USE_OFFSET(0x18E827, 0x1E22F7)); // CLIENT_BASE + USE_OFFSET(0x5F2F40, 0x92FB00)
		PATTERN_OFFSET_DWORD_PTR_CAST_TYPE(CLIENT_MOD, puzzlemaker_ptr, DWORD*, "8B 0D ? ? ? ? ? ? 8B 90 ? ? ? ? FF D2 33 C9 84 C0 0F 94 C0", 2, USE_OFFSET(0xE4667, 0xCE040)); // CLIENT_BASE + USE_OFFSET(0x65371C, 0x946438)

		// - client - functions
		PATTERN_OFFSET_SIMPLE_CAST(CLIENT_MOD, GetSplitScreenViewPlayer, GetSplitScreenViewPlayer_t, "8B 0D ? ? ? ? ? ? 8B 90 ? ? ? ? FF D2 50 E8 ? ? ? ? 83 C4 ? C3", 0, USE_OFFSET(0x128EF0, 0x176460));
		PATTERN_OFFSET_SIMPLE_CAST(CLIENT_MOD, ToPortalPlayer, ToPortalPlayer_t, "55 8B EC 56 8B 75 ? 85 F6 74 ? ? ? 8B 90 ? ? ? ? 8B CE FF D2 84 C0 74 ? 8B C6", 0, USE_OFFSET(0x104610, 0x14BF30));
		PATTERN_OFFSET_SIMPLE_CAST(CLIENT_MOD, CPortalRenderable__AddToVisAsExitPortal, CPortalRenderable__AddToVisAsExitPortal_t, "55 8B EC 51 57 8B 7D ? 89 4D ? 85 FF 0F 84 ? ? ? ? 53", 0, USE_OFFSET(0x22F290, 0x2BBDA0));
		PATTERN_OFFSET_SIMPLE_CAST(CLIENT_MOD, CViewRender__ViewDrawScene, CViewRender__ViewDrawScene_t, "55 8B EC 51 A1 ? ? ? ? 53 56 57 8B F9 8B 48", 0, USE_OFFSET(0x1945D0, 0x1E84E0));

		// - client - asm
		PATTERN_OFFSET_SIMPLE(CLIENT_MOD, hk_addr__cviewrenderer_renderview, "C7 45 ? ? ? ? ? 0F 85 ? ? ? ? 68", 0, USE_OFFSET(0x1990FD, 0x1ECDC5));
		PATTERN_OFFSET_SIMPLE(CLIENT_MOD, hk_addr__cviewrenderer_drawonemonitor, "8B 5D ? 6A ? 6A ? 6A ? 6A", 0, USE_OFFSET(0x1953F1, 0x1E92F4));
		PATTERN_OFFSET_SIMPLE(CLIENT_MOD, mod_addr__clientleafsystem_extract_culled_renderables, "74 ? FF 4B ? EB ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? 66 8B 4F ? 66 89 4E ? 8A 57 ? 88 56 ? 0F B6 47 ? 32 46 ? 8B 55 ? 24 ? 30 46 ? 8A 46 ? 8A 4F ? 32 C8 80 E1 ? 32 C8 8B 45 ? 88 4E ? ? ? ? 40 89 45 ? 83 C6 ? 8B 45 ? 40 83 C7 ? 89 45 ? 3B 45 ? 0F 8C ? ? ? ? 8B 45", 0, USE_OFFSET(0x9EF75, 0xDE4D5));
		PATTERN_OFFSET_SIMPLE(CLIENT_MOD, nop_addr__vguiscreen_draw_model, "74 ? 83 8E ? ? ? ? ? EB ? 80 7B", 0, USE_OFFSET(0x191EA3, 0x1E5693));
		PATTERN_OFFSET_SIMPLE(CLIENT_MOD, mod_addr__csimpleworldview_setup, "83 8E ? ? ? ? ? EB ? 80 7B", 0, USE_OFFSET(0x191EA5, 0x1E5695));
		PATTERN_OFFSET_SIMPLE(CLIENT_MOD, hk_addr__draw_our_3rd_person_body_mesh, "E8 ? ? ? ? 84 C0 75 ? 8B 45 ? A8", 0, USE_OFFSET(0x1E61CC, 0x274FF4));
		PATTERN_OFFSET_SIMPLE(CLIENT_MOD, hk_addr__draw_our_3rd_person_weapon_mesh, "83 C4 ? A1 ? ? ? ? ? ? 8B 45", 20, USE_OFFSET(0x5C4C0, 0x950A0));
		if (hk_addr__draw_our_3rd_person_weapon_mesh) {
			fn_addr__draw_our_3rd_person_weapon_mesh = USE_OFFSET(utils::mem::resolve_relative_call_address(hk_addr__draw_our_3rd_person_weapon_mesh), CLIENT_BASE + 0x58710); found_pattern_count++;
		} total_pattern_count++;

		PATTERN_OFFSET_SIMPLE(CLIENT_MOD, hk_addr__viewdrawscene_push_args, "E8 ? ? ? ? ? ? 8B 42 ? 8B CF FF D0 C6 05", 0, USE_OFFSET(0x19923C, 0x1ECF04));
		PATTERN_OFFSET_SIMPLE(CLIENT_MOD, hk_addr__cbaseworldview_draw_setup, "57 51 89 5D ? C7 05", 0, USE_OFFSET(0x19777A, 0x1EB59A));
		PATTERN_OFFSET_SIMPLE(CLIENT_MOD, hk_addr__cbaseentity_update_visibility, "84 DB 0F 84 ? ? ? ? 8B 0D ? ? ? ? 8B 01 8B 90 ? ? ? ? FF D2 8A D8 84 DB 75 ? 6A 01 E8 ? ? ? ? 6A 01 E8 ? ? ? ? 83 C4 08 56", 0, USE_OFFSET(0x3BB9A, 0x73076));
		if (hk_addr__cbaseentity_update_visibility) {
			hk_addr__cbaseentity_update_visibility_retn = USE_OFFSET(utils::mem::resolve_relative_jump_address(hk_addr__cbaseentity_update_visibility + 2u, 6u, 2u), CLIENT_BASE + 0x73105); found_pattern_count++;
		} total_pattern_count++;

		PATTERN_OFFSET_SIMPLE(CLIENT_MOD, nop_addr__write_savegame_screenshot_of_size, "6A 00 6A 22 8D 85 ? ? ? ? 50 50 FF D2 E8", 0, USE_OFFSET(0x17BFCA, 0x1D0FC9));
		PATTERN_OFFSET_SIMPLE(CLIENT_MOD, mod_addr__envprojectedtexture_should_update, "32 C0 5E C3 E8 ? ? ? ? 8A 8E ? ? ? ? 84 C9 74 ? 0F B6 C9 49 3B C8 7F ? 8A 8E ? ? ? ? 84 C9 74 ? 0F B6 D1 4A 3B D0 7C ? B0", 0, USE_OFFSET(0x6217C, 0x9AF1C));
		PATTERN_OFFSET_SIMPLE(CLIENT_MOD, hk_addr__cviewrenderer_init_fade_data, "E8 ? ? ? ? C1 E0 ? 05", 0, USE_OFFSET(0x18C812, 0x1DFC33));
		PATTERN_OFFSET_SIMPLE(CLIENT_MOD, mod_addr__cbaseentity_should_draw, "74 ? 0F B6 C9 49 3B C8 7F ? 8A 8E ? ? ? ? 84 C9 74 ? 0F B6 D1 4A 3B D0 7C ? E8 ? ? ? ? 8A 8E ? ? ? ? 84 C9 74 ? 0F B6 C9 49 3B C8 7F ? 8A 8E ? ? ? ? 84 C9 74 ? 0F B6 D1 4A 3B D0 7C ? 83 7E", 0, USE_OFFSET(0x3457D, 0x6BC2D));
		PATTERN_OFFSET_SIMPLE(CLIENT_MOD, fn_addr__add_console_cmd, "55 8B EC 8B 45 ? 53 33 DB 56 8B F1 8B 4D ? 80 66", 0, USE_OFFSET(0x3A2660, 0x6298D0));
		PATTERN_OFFSET_SIMPLE(CLIENT_MOD, fn_addr__get_map_name, "8B 0D ? ? ? ? 8B 01 8B 90 D0 00 00 00 FF E2 8B 0D", 0, USE_OFFSET(0x19B3A0, 0x1EEEE0));

		PATTERN_OFFSET_SIMPLE(CLIENT_MOD, mod_addr__areaportalwindow_draw_model, "55 8B EC 51 57 8B F9 80 BF ? ? ? ? 00 75 ? 33 C0 5F 8B E5 5D C2 08 00", 0, USE_OFFSET(0x64D60, 0x9DE9E));

		// the three RenderSpriteCard_Internal<> instantiations are byte-identical here, siblings are at fixed distances
		PATTERN_OFFSET_SIMPLE(CLIENT_MOD, hk_addr__render_sprite_card_rope, "C2 06 3B 8F ? ? ? ? 7E 06 89 8F ? ? ? ? 8B 8F F4 00 00 00", 16, USE_OFFSET(0x390474, 0x6165E4));
		if ( hk_addr__render_sprite_card_rope )
		{
			hk_addr__render_sprite_card_rope_normal = hk_addr__render_sprite_card_rope + 0x1625u; found_pattern_count++;
			hk_addr__render_sprite_card_rope_normal_cache = hk_addr__render_sprite_card_rope + 0x2C45u; found_pattern_count++;
		} total_pattern_count += 2u;

		PATTERN_OFFSET_SIMPLE(CLIENT_MOD, hk_addr__render_sprites_trail, "8B 8D D8 FE FF FF 0F", 0, USE_OFFSET(0x393FFE, 0x61A0EE));
		// CRopeManager::DrawRenderCache_NonQueued :: AdvanceVertices before End, buffer full and end of batch
		PATTERN_OFFSET_SIMPLE(CLIENT_MOD, hk_addr__rope_draw_render_cache_flush, "8B 95 FC FD FF FF 0F AF D1 01 95 B4 FE FF FF", 0, USE_OFFSET(0x7FDFA, 0x0));
		PATTERN_OFFSET_SIMPLE(CLIENT_MOD, hk_addr__rope_draw_render_cache_end, "8B 85 FC FD FF FF 0F AF C1 01 85 B4 FE FF FF", 0, USE_OFFSET(0x80A33, 0xB9613));

		PATTERN_OFFSET_SIMPLE(CLIENT_MOD, hk_addr__render_sprite_card_new, "8D 8D ? FE FF FF E8 D0 4F C8 FF 8B 8D 58 FE FF FF", 11, USE_OFFSET(0x393AA5, 0x619BA0));

		PATTERN_OFFSET_SIMPLE(CLIENT_MOD, fn_addr__get_bone_transform, "55 8B EC 56 8B F1 83 BE ? ? ? ? ? 57 75 ? 8B 46 ? 8B 50 ? 8D 4E ? FF D2 85 C0 74 ? 8B CE E8 ? ? ? ? 8B 86", 0, USE_OFFSET(0x246B0, 0x5C380));
		
		if constexpr (IS_LATEST_BUILD)
		{
			if (const auto offset = utils::mem::find_pattern(CLIENT_MOD, "E8 ? ? ? ? 89 86 ? ? ? ? 83 BE ? ? ? ? ? 75 ? 8B 56", 0, "fn_addr__lookup_bone", use_pattern, 0x5C331); offset) {
				fn_addr__lookup_bone = utils::mem::resolve_relative_call_address(offset); found_pattern_count++;
			} total_pattern_count++;
		} else {
			fn_addr__lookup_bone = CLIENT_BASE + 0x59D30;
		}

		PATTERN_OFFSET_SIMPLE(CLIENT_MOD, fn_addr__get_model_ptr, "56 8B F1 83 BE ? ? ? ? ? 75 ? 8B 46 ? 8B 50 ? 8D 4E ? FF D2 85 C0 74 ? 8B CE E8 ? ? ? ? 8B 86 ? ? ? ? 5E 85 C0 74 ? ? ? ? 75 ? 33 C0 C3", 0, USE_OFFSET(0x63F0, 0x3D040));


		// --------------------------
		// - shaderapidx9 - variables
		if constexpr (IS_LATEST_BUILD)
		{
			// any Dx9Device() call -> 'mov eax, offset g_D3DDeviceWrapper'; device is its first member
			if ( const auto offset = utils::mem::find_pattern( RENDERER_MOD, "E8 ? ? ? ? 8B F0 83 7E 10 00 74", 0, "d3d_device_ptr", use_pattern, 0x6082 ); offset )
			{
				d3d_device_ptr = (DWORD*)*(DWORD*)( utils::mem::resolve_relative_call_address( offset ) + 1u ); found_pattern_count++;
			} total_pattern_count++;
		}
		else {
			d3d_device_ptr = (DWORD*)(RENDERER_BASE + 0x179F38);
		}

		PATTERN_OFFSET_DWORD_PTR_CAST_TYPE(RENDERER_MOD, shaderapi_ptr, DWORD*, "8B 0D ? ? ? ? ? ? 8B 90 ? ? ? ? FF D2 ? ? 8B C8 8B 82 ? ? ? ? FF E0", 2, USE_OFFSET(0x6EC0, 0x60E0)); // RENDERER_BASE + USE_OFFSET(0x16ACE0, 0x164C48)


		// - shaderapidx9 - functions
		

		// - shaderapidx9 - asm
		PATTERN_OFFSET_SIMPLE(RENDERER_MOD, hk_addr__cmeshdx8_renderpass_pre_draw, "8B 4D FC 8B 41 4C 85 C0 0F 84", 0, USE_OFFSET(0xBC85, 0xAD23));
		PATTERN_OFFSET_SIMPLE(RENDERER_MOD, hk_addr__cmeshdx8_renderpass_post_draw, "50 51 56 FF D2 E9", 0, USE_OFFSET(0xBDD1, 0xADF5));
		PATTERN_OFFSET_SIMPLE(RENDERER_MOD, hk_addr__cmeshdx8_renderpass_for_instances, "8B 77 20 8B 16 8B 42 04", 0, USE_OFFSET(0xB0FA, 0xA56A));
		PATTERN_OFFSET_SIMPLE(RENDERER_MOD, nop_addr__cshadermgr_set_pixel_shader, "FF 15 ? ? ? ? 83 C4 ? 5F 5E 5B 8B E5 5D C2 ? ? 8B 4E", 0, USE_OFFSET(0x2F47F, 0x2AAB4));

		// --------------------------
		// - studiorender - variables
		//

		// - studiorender - functions
		//

		// - studiorender - asm
		PATTERN_OFFSET_SIMPLE(STUDIORENDER_MOD, hk_addr__studio_software_process_mesh, "0F 57 C9 56 8B 31", 0, USE_OFFSET(0xA4B7, 0xA587));
		// first match is CProcessMeshWrapper<0,0,0>, same instantiation as the hook above
		PATTERN_OFFSET_SIMPLE(STUDIORENDER_MOD, hk_addr__studio_software_process_mesh_restore, "89 88 A4 01 00 00 5E 8B E5 5D 8B E3 5B C3", 10, USE_OFFSET(0xA70F, 0xA7E9));
		PATTERN_OFFSET_SIMPLE(STUDIORENDER_MOD, hk_addr__studio_render_final, "5F 5E 8B E5 5D C2 28 00 CC", 0, USE_OFFSET(0x118D7, 0x10C57));
		PATTERN_OFFSET_SIMPLE(STUDIORENDER_MOD, hk_addr__studio_draw_points, "8B 87 D4 00 00 00 83", 0, USE_OFFSET(0x1175C, 0x10ADC));


#pragma endregion

		if (use_pattern)
		{
			if (found_pattern_count == total_pattern_count) {
				common::log("P2", std::format("Found all '{:d}' Patterns.", total_pattern_count), common::LOG_TYPE::LOG_TYPE_GREEN, true);
			}
			else
			{
				common::log("P2", std::format("Only found '{:d}' out of '{:d}' Patterns.", found_pattern_count, total_pattern_count), common::LOG_TYPE::LOG_TYPE_ERROR, true);
				common::log("P2", ">> Please create an issue on GitHub and attach this console log and information about your game (version, platform etc.)", common::LOG_TYPE::LOG_TYPE_STATUS, true);
				return false;
			}
		}

		return true;
	}

#undef PATTERN_OFFSET_SIMPLE

	void main()
	{
		if ( !init_game_addresses() )
		{
			Beep(300, 100); Sleep(100); Beep(200, 100);
			common::log("P2", "Not loading RTX Compatibility Mod.", common::LOG_TYPE::LOG_TYPE_ERROR, true);
			return;
		}

		// init remix api
		common::remix_api::initialize(nullptr, nullptr, []
		{
			main_module::hud_draw_area_info();
		}, false);

		common::loader::module_loader::register_module(std::make_unique<interfaces>());
		common::loader::module_loader::register_module(std::make_unique<imgui>());
		common::loader::module_loader::register_module(std::make_unique<game_settings>());
		common::loader::module_loader::register_module(std::make_unique<choreo_events>());
		common::loader::module_loader::register_module(std::make_unique<sound_events>());
		common::loader::module_loader::register_module(std::make_unique<remix_vars>());
		common::loader::module_loader::register_module(std::make_unique<remix_rayportal>());
		common::loader::module_loader::register_module(std::make_unique<remix_lights>());
		common::loader::module_loader::register_module(std::make_unique<map_settings>());
		common::loader::module_loader::register_module(std::make_unique<model_render>());
		common::loader::module_loader::register_module(std::make_unique<main_module>());

		MH_EnableHook(MH_ALL_HOOKS);
	}
}
