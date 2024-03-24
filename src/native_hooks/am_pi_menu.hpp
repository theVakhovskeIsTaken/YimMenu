#pragma once
#include "fiber_pool.hpp"
#include "native_hooks.hpp"
#include "natives.hpp"
#include "util/scripts.hpp"

namespace big
{
	namespace am_pi_menu
	{
		static void _SEND_TU_SCRIPT_EVENT_NEW(rage::scrNativeCallContext* src)
		{
			if (src->get_arg<Hash>(4) == (Hash)eRemoteEvent::SendCEOInvite)
			{
				g_player_service->iterate([src](const big::player_entry& e) {
					if ((src->get_arg<int>(3) & (1 << e.second->id())) != 0)
						e.second->invited_to_ceo = true;
				});
			}

			SCRIPT::_SEND_TU_SCRIPT_EVENT_NEW(src->get_arg<int>(0), src->get_arg<Any*>(1), src->get_arg<int>(2), src->get_arg<int>(3), src->get_arg<Hash>(4));
		}
	}
}
