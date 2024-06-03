#pragma once
#include <DynamicType/FieldWrapper.hpp>
#include <DynamicType/Internals.hpp>

#include <reflect>

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
    /**
     * @brief Validate a dynamically sized type
     */
    template <typename T>
        requires DynamicallySized<T>
    constexpr bool ValidateType()
    {
        size_t FieldCount = 0;

        DT_INTERNAL_PRE;
        reflect::for_each<T>([&FieldCount](auto I) {
            FieldCount += 1;
        });
        DT_INTERNAL_POST;

        return (FieldCount == sizeof(T)) ? true : throw "DynamicSize type must only have type selectors as fields.";
    }

    /**
     * @brief Initialize offsets of a dynamically sized type
     *
     * @return Final size of the container
     */
    template <typename T>
        requires DynamicallySized<T> && std::is_constructible_v<T>
    size_t InitializeOffsets()
    {
        constexpr bool Valid = ValidateType<T>();

        size_t RunningOffset = 0;
        size_t FinalSize = 0;

        DT_INTERNAL_PRE;
        reflect::for_each<T>([&RunningOffset, &FinalSize](auto I) {
            auto Value = reflect::get<I>(T{});
            FieldData& Data = decltype(Value)::InitializeOffsets(RunningOffset, reflect::offset_of<I, T>());

            RunningOffset += Data.Size;
            FinalSize = std::max(Data.Size + Data.Offset, FinalSize);
        });
        DT_INTERNAL_POST;

        return FinalSize;
    }

    /**
     * @brief A dynamically sized type
     *
     * This class is needed to wrap data storage of a type, to make it impossible to construct standalone.
     *
     * If the data storage class was constructed standalone it would lead to memory/logic errors
     * as members of the storage class all have a size of 1 instead of their member size.
     */
    template <typename T>
        requires DynamicallySized<T>
    class TDynamicallySized : public T
    {
      public:
        template <typename... CArgs>
        TDynamicallySized(CArgs... Args) = delete;

        using Inner = T;
    };

    namespace detail
    {
        template <typename T>
        concept DynamicallySizedWrapper = std::same_as<T, TDynamicallySized<typename T::Inner>>;
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
    template <typename T>
        requires detail::DynamicallySizedWrapper<T>
    class TDynamicTypeInstance
    {
      private:
        using Value = T;

      public:
        /**
         * @brief Create a new instance of a dynamically sized type on the heap
         */
        template <typename... CArgs>
        TDynamicTypeInstance(CArgs... Args)
            requires std::is_constructible_v<Value, CArgs...>
            : m_Owns(true)
        {
            void* Instance = ::operator new(Value::DynamicSize());
            if (!Instance)
            {
                throw std::bad_alloc{};
            }

            memset(Instance, 0, Value::DynamicSize());
            new (Instance) Value(std::forward<CArgs>(Args)...);

            m_Instance = reinterpret_cast<Value*>(Instance);
        }

        /**
         * @brief Use a dynamically sized type from pointer
         *
         * After constructing an instance using this constructor, the pointed object is **NOT** treated as owned,
         * meaning it won't be destroyed on destruction of this container
         */
        TDynamicTypeInstance(Value* Instance) : m_Instance(Instance), m_Owns(false)
        {
        }

        TDynamicTypeInstance(const TDynamicTypeInstance& Other) = delete;
        TDynamicTypeInstance& operator=(const TDynamicTypeInstance& Other) = delete;

        TDynamicTypeInstance(TDynamicTypeInstance&& Other) : m_Instance{std::exchange(Other.m_Instance, nullptr)}, m_Owns{std::exchange(Other.m_Owns, false)}
        {
        }

        TDynamicTypeInstance& operator=(TDynamicTypeInstance&& Other)
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
        {
            new (Data) Value::Inner(std::forward<CArgs>(Args)...);
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
