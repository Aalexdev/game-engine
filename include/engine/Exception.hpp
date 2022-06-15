#pragma once

#include "pch.hpp"
#include "core.hpp"

namespace engine::exceptions{
	class ENGINE_API Exception{
		public:
			virtual ~Exception() = default;

			std::string what() const {return error;}

		protected:
			Exception(const std::string &what) : error{what}{}
			std::string error;
	};

	class ENGINE_API OutOfRange : public Exception{
		public:
			OutOfRange(const std::string &what) : Exception(what){}
	};

	class ENGINE_API Lenght : public Exception{
		public:
			Lenght(const std::string &what) : Exception(what){}
	};

	class ENGINE_API Overflow : public Exception{
		public:
			Overflow(const std::string &what) : Exception(what){}
	};

	class ENGINE_API Underflow : public Exception{
		public:
			Underflow(const std::string &what) : Exception(what){}
	};

	class ENGINE_API BadPath : public Exception{
		public:
			BadPath(const std::string &what) : Exception(what){}
	};

	class ENGINE_API API : public Exception{
		public:
			API(const std::string &what) : Exception(what){}
	};

	class ENGINE_API BadParameter : public Exception{
		public:
			BadParameter(const std::string &what) : Exception(what){}
	};
}