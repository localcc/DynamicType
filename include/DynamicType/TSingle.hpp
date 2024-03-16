#pragma once
#include <DynamicType/FieldData.hpp>
#include <DynamicType/FieldWrapper.hpp>

namespace DynamicType
{
    /**
     * @brief A reflection wrapper for a value
     *
     * This wrapper is used for values that never change size at runtime
     *
     * @tparam T wrapped type
     * @tparam ReflectionData associated reflection data
     */
    template <typename T, FieldData* ReflectionData>
    class TSingle
    {
      public:
        /**
         * @brief Set the value inside of the container
         */
        void Set(T Data)
        {
            *ReflectionData->GetValue<T, TSingle>(this) = std::move(Data);
        }

      public:
        const T& operator*() const
        {
            return *ReflectionData->GetValue<T, TSingle>(this);
        }

        const T& operator->() const
        {
            return *ReflectionData->GetValue<T, TSingle>(this);
        }

      public:
        TSingle()
        {
        }

        TSingle(const TSingle& Other)
            requires std::is_copy_assignable_v<T>
        {
            *ReflectionData->GetValue<T, TSingle>(this) = *ReflectionData->GetValue<T, TSingle>(&Other);
        }
        TSingle& operator=(const TSingle& Other)
            requires std::is_copy_assignable_v<T>
        {
            *ReflectionData->GetValue<T, TSingle>(this) = *ReflectionData->GetValue<T, TSingle>(&Other);
            return *this;
        }

        TSingle(TSingle&& Other)
            requires std::is_move_assignable_v<T>
        {
            *ReflectionData->GetValue<T, TSingle>(this) = std::move(*ReflectionData->GetValue<T, TSingle>(&Other));
        }
        TSingle& operator=(TSingle&& Other)
            requires std::is_move_assignable_v<T>
        {
            *ReflectionData->GetValue<T, TSingle>(this) = std::move(*ReflectionData->GetValue<T, TSingle>(&Other));
            return *this;
        }

      public:
        ~TSingle()
            requires std::is_destructible_v<T>
        {
            ReflectionData->GetValue<T, TSingle>(this)->~T();
        }

      public:
        static FieldData& InitializeOffsets(size_t Offset, size_t OffsetAdjuster)
        {
            ReflectionData->Offset = Offset;
            ReflectionData->OffsetAdjuster = OffsetAdjuster;
            ReflectionData->Size = GetSize<T>();
            return *ReflectionData;
        }

        static size_t DynamicSize()
        {
            return ReflectionData->Size;
        }
    };
} // namespace DynamicType
