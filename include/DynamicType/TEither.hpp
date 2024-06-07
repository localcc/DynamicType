#pragma once
#include <DynamicType/FieldData.hpp>
#include <DynamicType/FieldWrapper.hpp>
#include <DynamicType/SafetyCookie.hpp>

namespace DynamicType
{
    /**
     * @brief A reflection wrapper for a value that switches between two sizes at runtime
     *
     * @tparam T1 first wrapped type, selected when `SwitchCondition` is false
     * @tparam T2 second wrapped type, selected when `SwitchCondition` is true
     * @tparam ReflectionData associated reflection data
     * @tparam SwitchCondition condition for switching types
     */
    template <typename T1, typename T2, FieldData* ReflectionData, bool* SwitchCondition>
        requires std::is_convertible_v<T1, T2> && std::is_convertible_v<T2, T1>
    class TEither
    {
      public:
        /**
         * @brief Set the value inside of the container
         */
        template <typename T>
        void Set(T Data)
        {
            if (*SwitchCondition)
            {
                if constexpr (std::is_same_v<T, T2>)
                    *ReflectionData->GetValue<T2>(this) = std::move(Data);
                else
                    *ReflectionData->GetValue<T2>(this) = std::move(static_cast<T2>(Data));
            }
            else
            {
                if constexpr (std::is_same_v<T, T1>)
                    *ReflectionData->GetValue<T1>(this) = std::move(Data);
                else
                    *ReflectionData->GetValue<T1>(this) = std::move(static_cast<T1>(Data));
            }
        }

      public:
        template <typename T = T2>
        T operator*() const
        {
            if (*SwitchCondition)
            {
                if constexpr (std::is_same_v<T, T2>)
                    return *ReflectionData->GetValue<T2>(this);
                else
                    return static_cast<T1>(*ReflectionData->GetValue<T2>(this));
            }
            else
            {
                if constexpr (std::is_same_v<T, T1>)
                    return *ReflectionData->GetValue<T1>(this);
                else
                    return static_cast<T2>(*ReflectionData->GetValue<T1>(this));
            }
        }

      public:
        explicit TEither(SafetyCookie)
        {
        }

        TEither(const TEither& Other)
            requires std::is_copy_assignable_v<T1> && std::is_copy_assignable_v<T2>
        {
            if (*SwitchCondition)
            {
                *ReflectionData->GetValue<T2>(this) = *ReflectionData->GetValue<T2>(&Other);
            }
            else
            {
                *ReflectionData->GetValue<T1>(this) = *ReflectionData->GetValue<T1>(&Other);
            }
        }
        TEither& operator=(const TEither& Other)
            requires std::is_copy_assignable_v<T1> && std::is_copy_assignable_v<T2>
        {
            if (*SwitchCondition)
            {
                *ReflectionData->GetValue<T2>(this) = *ReflectionData->GetValue<T2>(&Other);
            }
            else
            {
                *ReflectionData->GetValue<T1>(this) = *ReflectionData->GetValue<T1>(&Other);
            }
            return *this;
        }

        TEither(TEither&& Other) noexcept
            requires std::is_move_assignable_v<T1> && std::is_move_assignable_v<T2>
        {
            if (*SwitchCondition)
            {
                *ReflectionData->GetValue<T2>(this) = std::move(*ReflectionData->GetValue<T2>(&Other));
            }
            else
            {
                *ReflectionData->GetValue<T1>(this) = std::move(*ReflectionData->GetValue<T1>(&Other));
            }
        }
        TEither& operator=(TEither&& Other) noexcept
            requires std::is_move_assignable_v<T1> && std::is_move_assignable_v<T2>
        {
            if (*SwitchCondition)
            {
                *ReflectionData->GetValue<T2>(this) = std::move(*ReflectionData->GetValue<T2>(&Other));
            }
            else
            {
                *ReflectionData->GetValue<T1>(this) = std::move(*ReflectionData->GetValue<T1>(&Other));
            }
            return *this;
        }

      public:
        static FieldData& InitializeOffsets(size_t Offset, size_t OffsetAdjuster)
        {
            ReflectionData->Offset = Offset;
            ReflectionData->OffsetAdjuster = OffsetAdjuster;
            if (*SwitchCondition)
            {
                ReflectionData->Size = GetSize<T2>();
            }
            else
            {
                ReflectionData->Size = GetSize<T1>();
            }

            return *ReflectionData;
        }

        static size_t DynamicSize()
        {
            return ReflectionData->Size;
        }
    };
} // namespace DynamicType
