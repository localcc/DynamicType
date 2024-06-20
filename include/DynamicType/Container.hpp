#pragma once
#include <DynamicType/FieldWrapper.hpp>
#include <DynamicType/SafetyCookie.hpp>
#include <DynamicType/DynamicallySizedWrapper.hpp>

#include <type_traits>
#include <algorithm>

/**
 * @brief Allocate an instance of a dynamically sized type on the stack
 *
 * An instance allocated like this will get destroyed upon exiting from the stack frame
 */
#define DT_STACKALLOC(ty, ...) DynamicType::TDynamicTypeInstance<ty>::PlacementNew(reinterpret_cast<ty*>(alloca(ty::DynamicSize())) __VA_OPT__(, ) __VA_ARGS__)

namespace DynamicType
{
    template <typename T>
    struct MemberPointerType;

    template <typename Class, typename Value>
    struct MemberPointerType<Value Class::*>
    {
        typedef Value type;
    };

    template <typename T>
    using MemberPointerTypeT = typename MemberPointerType<T>::type;

    /**
     * @brief Initialize offsets of a dynamically sized type
     *
     * @return Final size of the container
     */
    template <typename T, typename... TFields>
        requires DynamicallySized<T>
    size_t InitializeOffsets(TFields... Fields)
    {
        static_assert(sizeof...(Fields) == sizeof(T),
                      "DynamicSize type must only have type selectors as fields. Type selectors must be listed in order. All type selectors must be listed.");

        size_t RunningOffset = 0;
        size_t FieldIndex = 0;

        auto InitializeField = [&RunningOffset, &FieldIndex]<typename Field>(const Field F) {
            FieldData& Data = MemberPointerTypeT<Field>::InitializeOffsets(RunningOffset, FieldIndex);

            FieldIndex += 1;
            RunningOffset += Data.Size;
        };

        (InitializeField(Fields), ...);

        return RunningOffset;
    }

    /**
     * @brief An instance of a dynamically sized type
     *
     * A dynamically sized type instance.
     *
     * Creating this without a pointer allocates an instance of the type on the heap with the dynamic size reported by the type.
     *
     * @tparam T Underlying type
     */
    template <DynamicallySizedWrapper T>
    class TDynamicTypeInstance
    {
      private:
        using Value = T;

      public:
        /**
         * @brief Create a new instance of a dynamically sized type on the heap
         */
        template <typename... CArgs>
        explicit TDynamicTypeInstance(CArgs... Args)
            requires std::is_constructible_v<Value, SafetyCookie, CArgs...>
            : m_Owns(true)
        {
            void* Instance = ::operator new(Value::DynamicSize());
            if (!Instance)
            {
                throw std::bad_alloc{};
            }

            memset(Instance, 0, Value::DynamicSize());
            new (Instance) Value(SafetyCookie(), std::forward<CArgs>(Args)...);

            m_Instance = reinterpret_cast<Value*>(Instance);
        }

        /**
         * @brief Use a dynamically sized type from pointer
         *
         * After constructing an instance using this constructor, the pointed object is **NOT** treated as owned,
         * meaning it won't be destroyed on destruction of this container
         */
        explicit TDynamicTypeInstance(Value* Instance) : m_Instance(Instance), m_Owns(false)
        {
        }

        TDynamicTypeInstance(const TDynamicTypeInstance& Other) = delete;
        TDynamicTypeInstance& operator=(const TDynamicTypeInstance& Other) = delete;

        TDynamicTypeInstance(TDynamicTypeInstance&& Other) noexcept
            : m_Instance{std::exchange(Other.m_Instance, nullptr)}, m_Owns{std::exchange(Other.m_Owns, false)}
        {
        }

        TDynamicTypeInstance& operator=(TDynamicTypeInstance&& Other) noexcept
        {
            std::swap(m_Instance, Other.m_Instance);
            std::swap(m_Owns, Other.m_Owns);
            return *this;
        }

        ~TDynamicTypeInstance()
        {
            if (m_Instance && m_Owns)
            {
                ::operator delete(m_Instance);
            }
        }

      public:
        /**
         * @brief Use a placement new constructor for a dynamic type
         *
         * After constructing an instance using this constructor, the pointed object is **NOT** treated as owned,
         * meaning it won't be destroyed on destruction of this container
         */
        template <typename... CArgs>
        static TDynamicTypeInstance PlacementNew(void* Data, CArgs... Args)
            requires std::is_constructible_v<typename Value::Inner, SafetyCookie, CArgs...>
        {
            new (Data) Value::Inner(SafetyCookie(), std::forward<CArgs>(Args)...);
            return TDynamicTypeInstance(reinterpret_cast<Value*>(Data));
        }

      public:
        const Value& operator*() const
        {
            return *m_Instance;
        }

        const Value* operator->() const
        {
            return m_Instance;
        }

        Value& operator*()
        {
            return *m_Instance;
        }

        Value* operator->()
        {
            return m_Instance;
        }

      private:
        Value* m_Instance = nullptr;
        bool m_Owns = false;
    };
} // namespace DynamicType
