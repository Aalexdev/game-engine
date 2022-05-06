#pragma once

#include "core.hpp"

namespace engine{
	class ENGINE_API UUID{
		public:
			UUID() : uuid{generateUUID()} {}
			UUID(uint64_t id) : uuid{id} {}

			uint64_t get() const {return uuid;}

			operator uint64_t() const {return uuid;}

		private:
			uint64_t uuid;

			static uint64_t generateUUID();
	};
}

namespace std{
	template<>
	struct hash<engine::UUID>{
		size_t operator()(const engine::UUID &id) const{
			return static_cast<size_t>(static_cast<uint64_t>(id));
		}
	};
}
