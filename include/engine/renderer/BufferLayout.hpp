#pragma once

#include "../pch.hpp"
#include "../core.hpp"
#include "../debug/Log.hpp"

namespace engine{
	enum class DataType : uint8_t{
		None    = 0,
		Float   = 1,
		Float2  = 2,
		Float3  = 3,
		Float4  = 4,
		Int     = 5,
		Int2    = 6,
		Int3    = 7,
		Int4    = 8,
		Mat2    = 9,
		Mat3    = 10,
		Mat4    = 11,
		Bool    = 12,
		UInt    = 13,
		UInt2   = 14,
		UInt3   = 15,
		UInt4   = 16,
	};

	static constexpr uint32_t dataTypeSize(DataType type){
		switch (type){
			case DataType::None    : return 0;
			case DataType::Float   : return 4;
			case DataType::Float2  : return 4 * 2;
			case DataType::Float3  : return 4 * 3;
			case DataType::Float4  : return 4 * 4;
			case DataType::Mat2    : return 4 * 4;
			case DataType::Mat3    : return 4 * 9;
			case DataType::Mat4    : return 4 * 16;
			case DataType::Int     : return 4;
			case DataType::Int2    : return 4 * 2;
			case DataType::Int3    : return 4 * 3;
			case DataType::Int4    : return 4 * 4;
			case DataType::Bool    : return 4;
			case DataType::UInt    : return 4;
			case DataType::UInt2   : return 4 * 2;
			case DataType::UInt3   : return 4 * 3;
			case DataType::UInt4   : return 4 * 4;
		}
		ENGINE_CORE_ASSERT(false, "unknown DataType");
		return 0;
	}

	inline static constexpr uint8_t dataTypeElemementCount(DataType type){
		return dataTypeSize(type) / 4;
	}

	struct ENGINE_API BufferElement{
        DataType type;
        std::string name;
        uint32_t offset = 0;
        uint32_t size = 0;
        bool normalized = false;

        BufferElement(const DataType &type, const std::string &name, const bool &normalized = false) : name{name}, type{type}, size{dataTypeSize(type)}, normalized{normalized} {}
        inline uint8_t getComponentCount() const {return dataTypeElemementCount(type);}
    };

    class ENGINE_API BufferLayout{
        public:
            BufferLayout(const std::initializer_list<BufferElement>& layout) : elements{layout} {
                uint32_t offset = 0;
                for (auto &element : elements){
                    element.offset = offset;
                    offset += element.size;
                }
                stride = offset;
            }

            BufferLayout() {}

            inline const std::vector<BufferElement>& getElements() const {return elements;}
            inline operator const std::vector<BufferElement>&() const {return elements;}
            inline const uint32_t& getStride() const {return stride;}

            inline std::vector<BufferElement>::iterator begin() {return elements.begin();}
            inline std::vector<BufferElement>::iterator end() {return elements.end();}
            inline std::vector<BufferElement>::const_iterator begin() const {return elements.begin();}
            inline std::vector<BufferElement>::const_iterator end() const {return elements.end();}

        private:
            std::vector<BufferElement> elements;
            uint32_t stride = 0;
    };
}