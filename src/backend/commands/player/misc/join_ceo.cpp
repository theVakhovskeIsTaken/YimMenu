#include "backend/player_command.hpp"
#include "core/scr_globals.hpp"
#include "natives.hpp"
#include "pointers.hpp"
#include "script_function.hpp"
#include "script.hpp"

#include <script/globals/GPBD_FM_3.hpp>

namespace
{
	void try_add_to_goon_list(int target, int goon)
	{
		auto& boss_goon = big::scr_globals::gpbd_fm_3.as<GPBD_FM_3*>()->Entries[target].BossGoon;

		for (int i = 0; i < 8; i++)
		{
			if (boss_goon.Goons[i] == -1)
			{
				boss_goon.Goons[i] = goon;
				break;
			}
		}
	}
}

namespace big
{
	class join_ceo : player_command
	{
		using player_command::player_command;

		virtual void execute(player_ptr player, const command_arguments& _args, const std::shared_ptr<command_context> ctx) override
		{
			// this is required for the associate status to stick
			scr_globals::gpbd_fm_3.as<GPBD_FM_3*>()->Entries[self::id].BossGoon.JoiningBoss = player->id();
			scr_globals::gpbd_fm_3.as<GPBD_FM_3*>()->Entries[self::id].BossGoon.JoinedInviteType = eGoonInviteType::INDIVIDUAL;

			const size_t arg_count = 3;
			int64_t args[arg_count] = {(int64_t)eRemoteEvent::RequestJoinCEO, (int64_t)self::id, 1 << player->id()};

			g_pointers->m_gta.m_trigger_script_event(1, args, arg_count, 1 << player->id(), (int)eRemoteEvent::RequestJoinCEO);

			script::get_current()->yield(5s);

			// try to force in
			if (scr_globals::gpbd_fm_3.as<GPBD_FM_3*>()->Entries[self::id].BossGoon.Boss != player->id())
			{
				scr_functions::join_ceo({player->id(), 0, false, false});
				try_add_to_goon_list(player->id(), self::id);
			}
		}
	};

	join_ceo g_join_ceo("joinceo", "JOIN_CEO", "JOIN_CEO_DESC", 0, false);
}