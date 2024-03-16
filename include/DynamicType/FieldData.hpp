#pragma once
#include <DynamicType/Types.hpp>

namespace DynamicType
{
    struct FieldData
    {
        size_t Offset;
        size_t OffsetAdjuster;
        size_t Size;

        template <typename T, typename TSelf>
        T* GetValue(TSelf* Self)
        {
            return reinterpret_cast<T*>(reinterpret_cast<uint8*>(Self) + Offset - OffsetAdjuster);
        }

        template <typename T, typename TSelf>
        const T* GetValue(const TSelf* Self)
        {
            return reinterpret_cast<const T*>(reinterpret_cast<const uint8*>(Self) + Offset - OffsetAdjuster);
        }
    };
} // namespace DynamicType
