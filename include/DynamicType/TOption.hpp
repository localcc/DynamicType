#pragma once
#include <DynamicType/FieldData.hpp>
#include <DynamicType/FieldWrapper.hpp>
#include <DynamicType/SafetyCookie.hpp>

#include <optional>

namespace DynamicType
{
    /**
     * @brief An optionally present value
     *
     * This wrapper is used for values that might or might not be present as part of the structure layout at runtime
     *
     * @tparam T wrapped type
     * @tparam ReflectionData associated reflection data
     * @tparam IsPresent condition for the type being present
     */
    template <typename T, FieldData* ReflectionData, bool* IsPresent>
    class TOption
    {
      public:
        /**
         * @brief Set the value inside of the container
         *
         * If the value is not present, does nothing
         */
        void Set(T Data)
        {
            if (*IsPresent)
            {
                *ReflectionData->GetValue<T, TOption>(this) = std::move(Data);
            }
        }

      private:
        template <typename Ret, typename Self>
        Ret* GetValue(Self* self)
        {
            if (*IsPresent)
            {
                return ReflectionData->GetValue<T, TOption>(self);
            }
            else
            {
                if constexpr (std::is_default_constructible_v<T>)
                {
                    static T DefaultInstance = {};
                    return &DefaultInstance;
                }
                else
                {
                    throw std::bad_optional_access{};
                }
            }
        }

      public:
        /**
         * @brief Get the value inside of the container
         *
         * If the value is not present:
         *
         * * If the value supports default construction, returns a default value instance
         *
         * * If the value doesn't support default construction, throws bad_optional_access
         */
        T& operator*()
        {
            return *GetValue<T>(this);
        }

        /**
         * @brief Get the value inside of the container
         *
         * If the value is not present:
         *
         * * If the value supports default construction, returns a default value instance
         *
         * * If the value doesn't support default construction, throws bad_optional_access
         */
        T* operator->()
        {
            return GetValue(this);
        }

        /**
         * @brief Get the value inside of the container
         *
         * If the value is not present:
         *
         * * If the value supports default construction, returns a default value instance
         *
         * * If the value doesn't support default construction, throws bad_optional_access
         */
        const T& operator*() const
        {
            return *GetValue<const T>(this);
        }

        /**
         * @brief Get the value inside of the container
         *
         * If the value is not present:
         *
         * * If the value supports default construction, returns a default value instance
         *
         * * If the value doesn't support default construction, throws bad_optional_access
         */
        const T* operator->() const
        {
            return GetValue(this);
        }

      public:
        explicit TOption(SafetyCookie)
            requires std::is_default_constructible_v<T>
        {
            if (*IsPresent)
            {
                *ReflectionData->GetValue<T, TOption>(this) = T();
            }
        }

        explicit TOption(SafetyCookie, const T& Value)
            requires std::is_copy_assignable_v<T>
        {
            if (*IsPresent)
            {
                *ReflectionData->GetValue<T, TOption>(this) = Value;
            }
        }

        explicit TOption(SafetyCookie, T&& Value)
            requires std::is_move_assignable_v<T>
        {
            if (*IsPresent)
            {
                *ReflectionData->GetValue<T, TOption>(this) = std::move(Value);
            }
        }

        TOption(const TOption& Other)
            requires std::is_copy_assignable_v<T>
        {
            if (*IsPresent)
            {
                *ReflectionData->GetValue<T, TOption>(this) = *ReflectionData->GetValue<T, TOption>(&Other);
            }
        }
        TOption& operator=(const TOption& Other)
            requires std::is_copy_assignable_v<T>
        {
            if (*IsPresent)
            {
                *ReflectionData->GetValue<T, TOption>(this) = *ReflectionData->GetValue<T, TOption>(&Other);
            }
            return *this;
        }
        TOption& operator=(const T& Value)
            requires std::is_copy_assignable_v<T>
        {
            if (*IsPresent)
            {
                *ReflectionData->GetValue<T, TOption>(this) = Value;
            }
            return *this;
        }

        TOption(TOption&& Other) noexcept
            requires std::is_move_assignable_v<T>
        {
            if (*IsPresent)
            {
                *ReflectionData->GetValue<T, TOption>(this) = std::move(*ReflectionData->GetValue<T, TOption>(&Other));
            }
        }
        TOption& operator=(TOption&& Other) noexcept
            requires std::is_move_assignable_v<T>
        {
            if (*IsPresent)
            {
                *ReflectionData->GetValue<T, TOption>(this) = std::move(*ReflectionData->GetValue<T, TOption>(&Other));
            }
            return *this;
        }
        TOption& operator=(T&& Value)
            requires std::is_move_assignable_v<T>
        {
            if (*IsPresent)
            {
                *ReflectionData->GetValue<T, TOption>(this) = Value;
            }
            return *this;
        }

      public:
        ~TOption()
            requires std::is_destructible_v<T>
        {
            if (*IsPresent)
            {
                ReflectionData->GetValue<T, TOption>(this)->~T();
            }
        }

      public:
        static FieldData& InitializeOffsets(size_t Offset, size_t OffsetAdjuster)
        {
            ReflectionData->Offset = Offset;
            ReflectionData->OffsetAdjuster = OffsetAdjuster;

            if (*IsPresent)
            {
                ReflectionData->Size = GetSize<T>();
            }
            else
            {
                ReflectionData->Size = 0;
            }
            return *ReflectionData;
        }

        static size_t DynamicSize()
        {
            return ReflectionData->Size;
        }
    };
} // namespace DynamicType
