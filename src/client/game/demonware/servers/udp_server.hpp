#pragma once

#include "base_server.hpp"
#include <utils/concurrency.hpp>

namespace demonware
{
	class udp_server : public base_server
	{
	public:
		using base_server::base_server;

		void handle_input(const char* buf, size_t size);
		size_t handle_output(char* buf, size_t size);
		bool pending_data() override;

	protected:
		virtual void handle(const std::string& data) = 0;

		void send(const std::string& data);

	private:
		utils::concurrency::container<data_queue> in_queue_;
		utils::concurrency::container<data_queue> out_queue_;

		void frame() override;
	};
}