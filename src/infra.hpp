#pragma once

namespace p2
{
	// -------------------------------------------
	// game variables

	// - server


	// - engine
	extern int* r_visframecount;
	extern int* visible_areas_num;
	extern std::uint16_t* visible_areas;
	extern bool* viewer_in_solid_space;
	extern components::CPortalRect* area_rect;
	extern DWORD* area_frustum;
	extern components::Frustum_t* g_frustum;
	extern DWORD* engine_renderer_ptr;
	extern DWORD* hoststate_worldbrush_data_ptr;
	extern DWORD* cstatic_prop_mgr_ptr;
	extern Vector* current_view_origin;
	extern Vector* current_view_forward;

	// - client
	extern Vector4D* s_viewFadeColor;
	extern DWORD* global_vars_ptr;
	extern components::view_id* current_view_id;
	extern DWORD* puzzlemaker_ptr;

	// - shaderapidx9
	extern DWORD* d3d_device_ptr;
	extern DWORD* shaderapi_ptr;
	

	// -------------------------------------------
	// game functions

	// - server


	// - engine
	typedef	bool(__cdecl* R_CullNode_t)(components::mnode_t*);
	extern R_CullNode_t R_CullNode;

	typedef	bool(__cdecl* CM_LeafArea_t)(int leaf_num);
	extern CM_LeafArea_t CM_LeafArea;

	typedef	void(__cdecl* ComputeViewMatrices_t)(components::VMatrix* pWorldToView, components::VMatrix* pViewToProjection, components::VMatrix* pWorldToProjection, const components::CViewSetup* viewSetup);
	extern ComputeViewMatrices_t ComputeViewMatrices;

	typedef	void* (__cdecl* GetBaseLocalClient_t)();
	extern GetBaseLocalClient_t GetBaseLocalClient;

	typedef	BOOL(__fastcall* CClientState__IsPaused_t)(void* this_ptr, void* null);
	extern CClientState__IsPaused_t CClientState__IsPaused;

	// - client
	typedef components::C_BaseEntity* (__cdecl* GetSplitScreenViewPlayer_t)();
	extern GetSplitScreenViewPlayer_t GetSplitScreenViewPlayer;

	typedef	void* (__cdecl* ToPortalPlayer_t)(components::C_BaseEntity*);
	extern ToPortalPlayer_t ToPortalPlayer;

	typedef	void (__fastcall* CPortalRenderable__AddToVisAsExitPortal_t)(void* this_ptr, void* null, components::ViewCustomVisibility_t*);
	extern CPortalRenderable__AddToVisAsExitPortal_t CPortalRenderable__AddToVisAsExitPortal;

	typedef	void (__fastcall* CViewRender__ViewDrawScene_t)(void* this_ptr, void* null, bool, int, const components::CViewSetup*, int, int, bool, int, components::ViewCustomVisibility_t*);
	extern CViewRender__ViewDrawScene_t CViewRender__ViewDrawScene;

	// - shaderapidx9
	//


	// -------------------------------------------
	// game asm offsets

	// - server
	extern uint32_t hk_addr__scene_ent_on_start_event;
	extern uint32_t hk_addr__scene_ent_on_finish_event;
	extern uint32_t hk_addr__spawn_all_entities;
	extern uint32_t retn_addr__spawn_all_entities;

	// - engine
	extern uint32_t hk_addr__on_map_load;
	extern uint32_t hk_addr__on_host_disconnect;
	extern uint32_t hk_addr__on_host_change_level;
	extern uint32_t hk_addr__pre_recursive_world_node;
	extern uint32_t hk_addr__while_recursive_world_node;
	extern uint32_t hk_addr__while_recursive_world_node_og_retn;
	extern uint32_t hk_addr__while_recursive_world_node_force_retn;
	extern uint32_t mod_addr__while_recursive_world_node_01;
	extern uint32_t nop_addr__while_recursive_world_node_02;
	extern uint32_t mod_addr__while_recursive_world_node_03;
	extern uint32_t nop_addr__draw_leaf;
	extern uint32_t nop_addr__cbrushbatchrender_draw_opaque_bmodel;
	extern uint32_t nop_addr__draw_displacements_in_leaf;
	extern uint32_t mod_addr__shader_draw_chains;
	extern uint32_t mod_addr__cbrushbatchrender_draw_opaque_bmodel;
	extern uint32_t nop_addr__cbrushbatchrender_compute_lmap_pages;
	extern uint32_t hk_addr__flow_through_area;
	extern uint32_t nop_addr__flow_through_area;
	extern uint32_t mod_addr__cstaticpropmgr_update_prop_visibility;
	extern uint32_t nop_addr__cstaticpropmgr_update_prop_visibility;
	extern uint32_t hk_addr__on_start_sound;
	extern uint32_t fn_addr__debug_overlay_add_text;
	extern uint32_t fn_addr__debug_overlay_add_text_colored;
	extern uint32_t fn_addr__r_flow_through_area;
	extern uint32_t fn_addr__frustum_set_planes;
	extern uint32_t fn_addr__frustum_cull_box;
	extern uint32_t fn_addr__cm_point_leaf_num;
	extern uint32_t fn_addr__override_view_frustum;
	extern uint32_t fn_addr__cull_node_simd;

	// - client
	extern uint32_t hk_addr__cviewrenderer_renderview;
	extern uint32_t hk_addr__cviewrenderer_drawonemonitor;
	extern uint32_t mod_addr__clientleafsystem_extract_culled_renderables;
	extern uint32_t nop_addr__vguiscreen_draw_model;
	extern uint32_t nop_addr__csimpleworldview_setup;
	extern uint32_t mod_addr__csimpleworldview_setup;
	extern uint32_t hk_addr__draw_our_3rd_person_body_mesh;
	extern uint32_t hk_addr__draw_our_3rd_person_weapon_mesh;
	extern uint32_t fn_addr__draw_our_3rd_person_weapon_mesh;
	extern uint32_t hk_addr__viewdrawscene_push_args;
	extern uint32_t hk_addr__cbaseworldview_draw_setup;
	extern uint32_t hk_addr__cbaseentity_update_visibility;
	extern uint32_t hk_addr__cbaseentity_update_visibility_retn;
	extern uint32_t nop_addr__write_savegame_screenshot_of_size;
	extern uint32_t mod_addr__envprojectedtexture_should_update;
	extern uint32_t hk_addr__cviewrenderer_init_fade_data;
	extern uint32_t mod_addr__cbaseentity_should_draw;
	extern uint32_t fn_addr__add_console_cmd;
	extern uint32_t fn_addr__get_map_name;
	extern uint32_t fn_addr__get_bone_transform;
	extern uint32_t fn_addr__lookup_bone;
	extern uint32_t fn_addr__get_model_ptr;
	extern uint32_t mod_addr__areaportalwindow_draw_model;
	extern uint32_t hk_addr__render_sprite_card_rope;
	extern uint32_t hk_addr__render_sprite_card_rope_normal;
	extern uint32_t hk_addr__render_sprite_card_rope_normal_cache;
	extern uint32_t hk_addr__render_sprites_trail;
	extern uint32_t hk_addr__render_sprite_card_new;
	extern uint32_t hk_addr__rope_draw_render_cache_flush;
	extern uint32_t hk_addr__rope_draw_render_cache_end;

	// - shaderapidx9
	extern uint32_t nop_addr__cshadermgr_set_pixel_shader;
	extern uint32_t hk_addr__cmeshdx8_renderpass_pre_draw;
	extern uint32_t hk_addr__cmeshdx8_renderpass_post_draw;
	extern uint32_t hk_addr__cmeshdx8_renderpass_for_instances;

	// - studiorender
	extern uint32_t hk_addr__studio_software_process_mesh;
	extern uint32_t hk_addr__studio_software_process_mesh_restore;
	extern uint32_t hk_addr__studio_render_final;
	extern uint32_t hk_addr__studio_draw_points;


	// -------------------------------------------

	void main();

	extern bool g_installed_signature_patches;
	extern bool g_install_signature_patches_async;
}
