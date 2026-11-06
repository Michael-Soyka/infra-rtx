#pragma once
#include "infra.hpp"
#include "sdk/inputsystem/c_input_stackstystem.hpp"

#define RENDERER_MOD			game::shaderapidx9_module
#define STUDIORENDER_MOD		game::studiorender_module
#define MATERIALSTYSTEM_MOD		game::materialsystem_module
#define ENGINE_MOD				game::engine_module
#define CLIENT_MOD				game::client_module
#define SERVER_MOD				game::server_module
#define VSTDLIB_MOD				game::vstdlib_module

#define RENDERER_BASE			game::shaderapidx9_module.handle
#define STUDIORENDER_BASE		game::studiorender_module.handle
#define MATERIALSTYSTEM_BASE	game::materialsystem_module.handle
#define ENGINE_BASE				game::engine_module.handle
#define CLIENT_BASE				game::client_module.handle
#define SERVER_BASE				game::server_module.handle
#define VSTDLIB_BASE			game::vstdlib_module.handle

using namespace components;

namespace glob
{
	extern bool spawned_external_console;
	extern bool has_debug_arg;
	extern HWND main_window;
	extern sdk::InputContext_t* input_context;
}

namespace game
{
	extern utils::mem::module_info shaderapidx9_module;
	extern utils::mem::module_info studiorender_module;
	extern utils::mem::module_info materialsystem_module;
	extern utils::mem::module_info engine_module;
	extern utils::mem::module_info client_module;
	extern utils::mem::module_info server_module;
	extern utils::mem::module_info vstdlib_module;

	extern const D3DXMATRIX IDENTITY;
	extern const D3DXMATRIX TC_TRANSLATE_TO_CENTER;
	extern const D3DXMATRIX TC_TRANSLATE_FROM_CENTER_TO_TOP_LEFT;

	extern ConVar* find_cvar(const char* name);
	extern const ConVar* find_cvar_const(const char* name);
	extern void con_add_command(ConCommand* cmd, const char* name, void(__cdecl* callback)(), const char* desc);
	extern void debug_add_text_overlay(const float* pos, float duration, const char* text);
	extern void debug_add_text_overlay(const float* pos, const char* text, int line_offset = 0, float r = 1.0f, float g = 1.0f, float b = 1.0f, float a = 1.0f);
	//extern void cbaseentity_remove(void* cbaseentity_ptr);

	extern int get_visframecount();
	extern const char* get_map_name();

	extern void r_flow_through_area(int area, const Vector* vec_vis_origin, const CPortalRect* clip_rect, const VisOverrideData_t* vis_data, float* reflection_water_height);
	inline int* get_visible_areas_num() { return p2::visible_areas_num; }
	inline std::uint16_t* get_visible_areas() { return p2::visible_areas; }
	inline bool* get_viewer_in_solid_space() { return p2::viewer_in_solid_space; }
	inline CPortalRect* get_area_rect() { return p2::area_rect; }
	inline Frustum_t* get_area_frustum() { return reinterpret_cast<Frustum_t*>(*(p2::area_frustum)); } // + 0x10 to pElements

	extern void frustum_set_planes(Frustum_t* frustum, const VPlane* planes);

	extern bool frustum_cull_box(Frustum_t* frustum, const Vector* mins, const Vector* maxs);
	inline Frustum_t* get_g_frustum() { return p2::g_frustum; }

	extern void cvar_uncheat(const char* name);
	extern void cvar_uncheat_and_set_int(const char* name, const int val);
	extern void cvar_uncheat_and_set_float(const char* name, const float val);

	extern void print_ingame(const char* msg, ...);

	// CRender gRender
	inline components::CRender* get_engine_renderer() { return reinterpret_cast<components::CRender*>(*p2::engine_renderer_ptr); }
	inline IDirect3DDevice9* get_d3d_device() { return reinterpret_cast<IDirect3DDevice9*>(*p2::d3d_device_ptr); }
	inline components::IShaderAPIDX8* get_shaderapi() { return reinterpret_cast<components::IShaderAPIDX8*>(*p2::shaderapi_ptr); }
	//inline components::CShaderAPIDx8* get_cshaderapi() { return reinterpret_cast<components::CShaderAPIDx8*>((RENDERER_BASE + USE_OFFSET(0xD7040, 0x1769A0))); } // 0125
	inline components::worldbrushdata_t* get_hoststate_worldbrush_data() { return reinterpret_cast<components::CCommonHostState*>(p2::hoststate_worldbrush_data_ptr)->worldbrush; }
	inline components::CGlobalVarsBase* get_global_vars() { return reinterpret_cast<components::CGlobalVarsBase*>(*p2::global_vars_ptr); }
	inline components::CStaticPropMgr* get_cstatic_prop_mgr() { return reinterpret_cast<components::CStaticPropMgr*>(p2::cstatic_prop_mgr_ptr); }

	inline Vector* get_current_view_origin() { return p2::current_view_origin; }
	inline Vector* get_current_view_forward() { return p2::current_view_forward; /*reinterpret_cast<Vector*>(ENGINE_BASE + USE_OFFSET(0x4351D0, 0x42FFE4));*/ }
	inline Vector* get_current_view_right() { return &p2::current_view_forward[3]; /*reinterpret_cast<Vector*>(ENGINE_BASE + USE_OFFSET(0x4351DC, 0x42FFF0));*/ }
	inline Vector* get_current_view_up() { return &p2::current_view_forward[6]; /*reinterpret_cast<Vector*>(ENGINE_BASE + USE_OFFSET(0x4351E8, 0x42FFFC));*/ }

	// note: this might be ILLEGAL when within 'CBaseWorldView::DrawSetup' -> use 'game::saved_view_id' instead
	inline view_id* get_current_view_id() { return p2::current_view_id; /*reinterpret_cast<view_id*>(CLIENT_BASE + USE_OFFSET(0x937F40, 0x92FB00));*/ }
	extern view_id saved_view_id;

	inline bool is_puzzlemaker_active()
	{
		struct puzz
		{
			int pad;
			bool m_bShowing;
			bool m_bActive;
			float m_flTransition;
			float m_flZoomScale;
			bool m_bInputEnabled;
			int m_LastUpdateFrame;
			bool m_bIsInLevel;
			bool m_bCanQuitGame;
			int m_nCachedSSSlot;
		};

		const auto p = reinterpret_cast<puzz*>(*p2::puzzlemaker_ptr);
		return p && p->m_bActive;
	}
	// 946438

	// CM_PointLeafnum
	inline int get_leaf_from_position(const Vector& pos) { return utils::hook::call<int(__cdecl)(const float*)>(p2::fn_addr__cm_point_leaf_num)(&pos.x); }

	inline bool is_paused() 
	{
		return p2::CClientState__IsPaused(p2::GetBaseLocalClient(), nullptr);
	}


	// returns C_BaseAnimating class pointer for a given IClientRenderable
	C_BaseAnimating* get_base_animating_for_client_renderable(IClientRenderable* pRenderable);

	namespace namespaces
	{
		namespace C_BaseAnimating
		{
			// returns bone matrix for given bone index
			/// @param this_ptr			C_BaseAnimating ptr
			/// @param bone				bone index
			/// @param boneToWorld		out bone matrix
			inline void GetBoneTransform(void* this_ptr, const int bone, matrix3x4_t* boneToWorld)
			{
				utils::hook::call<void(__fastcall)(void* this_ptr, void* null, int bone, matrix3x4_t* boneToWorld)>(p2::fn_addr__get_bone_transform)
					(this_ptr, nullptr, bone, boneToWorld);
			}

			// returns bone index for given bone name
			/// @param this_ptr			C_BaseAnimating ptr
			/// @param bone_name		bone name
			/// @return					bone index
			inline int LookupBone(void* this_ptr, const char* bone_name)
			{
				// xref "weapon_bone"
				return utils::hook::call<int(__fastcall)(void* this_ptr, void* null, const char* bone_name)>(p2::fn_addr__lookup_bone)
					(this_ptr, nullptr, bone_name);
			}

			// returns CStudioHdr pointer for given C_BaseAnimating pointer
			/// @param this_ptr			C_BaseAnimating ptr
			/// @return					CStudioHdr ptr
			inline CStudioHdr* GetModelPtr(void* this_ptr)
			{
				return utils::hook::call<CStudioHdr * (__fastcall)(void* this_ptr, void* null)>(p2::fn_addr__get_model_ptr)
					(this_ptr, nullptr);
			}
		}
	}
	

	extern void lock_cursor();
	extern void unlock_cursor();


	// ::
	// debug print console redirects

	static void(WINAPI* OriginalOutputDebugStringA)(LPCSTR lpOutputString) = nullptr;
	static void(WINAPI* OriginalOutputDebugStringW)(LPCWSTR lpOutputString) = nullptr;

	inline void WINAPI HookedOutputDebugStringA(LPCSTR lpOutputString)
	{
		// guard against messages that came through the detoured "Warning/Msg" funcs as these were already printed to the console
		if (lpOutputString && !globals::detoured_warning_fn_origin && !globals::detoured_msg_fn_origin) {
			common::log("Game:Dbg >", lpOutputString, common::LOG_TYPE::LOG_TYPE_DEFAULT, false, false, true);
		}

		// og func
		if (OriginalOutputDebugStringA) {
			OriginalOutputDebugStringA(lpOutputString);
		}
	}

	inline void WINAPI HookedOutputDebugStringW(LPCWSTR lpOutputString)
	{
		if (lpOutputString)
		{
			char buffer[1024];
			WideCharToMultiByte(CP_UTF8, 0, lpOutputString, -1, buffer, sizeof(buffer), NULL, NULL);

			// guard against messages that came through the detoured "Warning/Msg" funcs as these were already printed to the console
			if (!globals::detoured_warning_fn_origin && !globals::detoured_msg_fn_origin) {
				common::log("Game:Dbg >", buffer, common::LOG_TYPE::LOG_TYPE_DEFAULT, true, false, true);
			}
		}

		// og func
		if (OriginalOutputDebugStringW) {
			OriginalOutputDebugStringW(lpOutputString);
		}
	}

	inline void SetupDebugOutputHook()
	{
		if (MH_CreateHook(&OutputDebugStringA, &HookedOutputDebugStringA, reinterpret_cast<LPVOID*>(&OriginalOutputDebugStringA)) != MH_OK)
		{
			common::log("Functions", "Failed to create hook for OutputDebugStringA", common::LOG_TYPE::LOG_TYPE_ERROR);
			return;
		}

		if (MH_CreateHook(&OutputDebugStringW, &HookedOutputDebugStringW, reinterpret_cast<LPVOID*>(&OriginalOutputDebugStringW)) != MH_OK)
		{
			common::log("Functions", "Failed to create hook for OutputDebugStringW", common::LOG_TYPE::LOG_TYPE_ERROR);
			return;
		}

		if (MH_EnableHook(&OutputDebugStringA) != MH_OK || MH_EnableHook(&OutputDebugStringW) != MH_OK)
		{
			common::log("Functions", "Failed to enable hooks for OutputDebugStringA & OutputDebugStringW", common::LOG_TYPE::LOG_TYPE_ERROR);
			return;
		}
	}
}
