#include "backend/looped_command.hpp"
#include "pointers.hpp"

namespace big
{
	class aimbot : looped_command
	{
		using looped_command::looped_command;

		memory::byte_patch* m_aimbot_patch_1;
		memory::byte_patch* m_aimbot_patch_2;

		virtual void on_enable() override
		{
			if (!m_aimbot_patch_1)
				m_aimbot_patch_1 = memory::byte_patch::make(g_pointers->m_gta.m_should_not_target_entity, std::to_array({0xB0, 0x00, 0xC3})).get();
			if (!m_aimbot_patch_2)
				m_aimbot_patch_2 = memory::byte_patch::make(g_pointers->m_gta.m_get_assisted_aim_type, std::to_array({0xB0, 0x01, 0xC3})).get();

			m_aimbot_patch_1->apply();
			m_aimbot_patch_2->apply();
		}

		virtual void on_tick() override
		{
			
		}

		virtual void on_disable() override
		{
			m_aimbot_patch_1->restore();
			m_aimbot_patch_2->restore();		
		}
	};

	class aimbot_target_drivers : looped_command
	{
		using looped_command::looped_command;

		memory::byte_patch* m_driver_patch;

		virtual void on_enable() override
		{
			if (!m_driver_patch)
				m_driver_patch = memory::byte_patch::make(g_pointers->m_gta.m_should_allow_driver_lock_on, std::to_array({0xB0, 0x01, 0xC3}))
				                     .get();

			m_driver_patch->apply();
		}

		virtual void on_tick() override
		{
		}

		virtual void on_disable() override
		{
			m_driver_patch->restore();
		}
	};

	aimbot g_aimbot("aimbot", "VIEW_OVERLAY_AIMBOT", "BACKEND_LOOPED_WEAPONS_AIMBOT_DESC", g.weapons.aimbot.enable);
	aimbot_target_drivers
	    g_aimbot_target_drivers("aimbottargetdrivers", "AIMBOT_TARGET_DRIVERS", "AIMBOT_TARGET_DRIVERS_DESC", g.weapons.aimbot.target_drivers);
}
