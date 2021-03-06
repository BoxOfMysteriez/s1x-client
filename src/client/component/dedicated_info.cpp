#include <std_include.hpp>
#include "console.hpp"
#include "loader/component_loader.hpp"
#include "game/game.hpp"
#include "scheduler.hpp"
#include <utils\string.hpp>

namespace dedicated_info
{
	class component final : public component_interface
	{
	public:
		void post_unpack() override
		{
			if (!game::environment::is_dedi())
			{
				return;
			}

			scheduler::loop([]()
			{
				auto* sv_running = game::Dvar_FindVar("sv_running");
				if (!sv_running || !sv_running->current.enabled)
				{
					console::set_title("S1x Dedicated Server");
					return;
				}

				auto* const sv_hostname = game::Dvar_FindVar("sv_hostname");
				auto* const sv_maxclients = game::Dvar_FindVar("sv_maxclients");
				auto* const mapname = game::Dvar_FindVar("mapname");

				auto clientCount = 0;

				for (auto i = 0; i < sv_maxclients->current.integer; i++)
				{
					auto* client = &game::mp::svs_clients[i];
					auto* self = &game::mp::g_entities[i];

					if (client->header.state >= 1 && self && self->client)
					{
						clientCount++;
					}
				}

				std::string cleaned_hostname;
				cleaned_hostname.resize(static_cast<int>(strlen(sv_hostname->current.string) + 1));

				utils::string::strip(sv_hostname->current.string, cleaned_hostname.data(),
				                     static_cast<int>(strlen(sv_hostname->current.string)) + 1);

				console::set_title(utils::string::va("%s on %s [%d/%d]", cleaned_hostname.data(),
				                                     mapname->current.string, clientCount,
				                                     sv_maxclients->current.integer));
			}, scheduler::pipeline::main, 1s);
		}
	};
}

REGISTER_COMPONENT(dedicated_info::component)
