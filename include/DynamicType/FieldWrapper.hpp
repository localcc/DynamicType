#pragma once
#include <DynamicType/FieldData.hpp>

#include <tuple>
#include <concepts>

namespace DynamicType
{
    /**
     * @brief A concept describing a reflection field wrapper
     *
     * The `Initialize` function should fill out the `Size`, `Offset` and `OffsetAdjuster` fields
     * of the underlying field and return the associated `FieldData`.
     */
    template <typename T>
    concept FieldWrapper = requires(T, size_t Offset, size_t OffsetAdjuster) {
        {
            T::InitializeOffsets(Offset, OffsetAdjuster)
        } -> std::same_as<FieldData&>;
    };

    /**
     * @brief A concept for a dynamically sized container
     *
     * The `Size` function should return the full size of the container
     */
    template <typename T>
    concept DynamicallySized = requires(T) {
        {
            T::DynamicSize()
        } -> std::same_as<size_t>;
    };

    /**
     * @brief A Function for getting the size
     *
     * If a type is dynamically sized, queries the dynamic size. Otherwise uses `sizeof`
     */
    template <typename T>
    size_t GetSize()
    {
        if constexpr (DynamicallySized<T>)
        {
            return T::DynamicSize();
        }
        else
        {
            return sizeof(T);
        }
    }
} // namespace DynamicType
