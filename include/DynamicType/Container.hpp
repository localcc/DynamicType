#pragma once
#include <DynamicType/FieldWrapper.hpp>

#include <reflect>

#include <type_traits>
#include <algorithm>

/**
 * @brief Allocate an instance of a dynamically sized type on the stack
 *
 * An instance allocated like this will get destroyed upon exiting from the stack frame
 */
#define DT_STACKALLOC(ty) DynamicType::TDynamicTypeInstance<ty>(reinterpret_cast<ty*>(alloca(ty::DynamicSize())));

namespace DynamicType
{
    /**
     * @brief Initialize offsets of a dynamically sized types
     *
     * @return Final size of the container
     */
    template <typename T>
        requires DynamicallySized<T> && std::is_constructible_v<T>
    size_t InitializeOffsets()
    {
        size_t RunningOffset = 0;
        size_t FinalSize = 0;

        reflect::for_each<T>([&RunningOffset, &FinalSize](auto I) {
            auto Value = reflect::get<I>(T{});
            FieldData& Data = decltype(Value)::InitializeOffsets(RunningOffset, reflect::offset_of<I, T>());

            RunningOffset += Data.Size;
            FinalSize = std::max(Data.Size + Data.Offset, FinalSize);
        });

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
        TDynamicallySized() = delete;
    };

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
        requires DynamicallySized<T>
    class TDynamicTypeInstance
    {
      public:
        /**
         * @brief Create a new instance of a dynamically sized type on the heap
         */
        TDynamicTypeInstance() : m_Owns(true)
        {
            void* Instance = ::operator new(T::DynamicSize());
            if (!Instance)
            {
                throw std::bad_alloc{};
            }

            memset(Instance, 0, T::DynamicSize());
            m_Instance = reinterpret_cast<T*>(Instance);
        }

        /**
         * @brief Use a dynamically sized type from pointer
         *
         * After constructing an instance using this constructor, the pointed object is **NOT** treated as owned,
         * meaning it won't be destroyed on destruction of this container
         */
        TDynamicTypeInstance(T* Instance) : m_Instance(Instance), m_Owns(false)
        {
        }

        ~TDynamicTypeInstance()
        {
            if (m_Instance && m_Owns)
            {
                ::operator delete(m_Instance);
            }
        }

      public:
        TDynamicTypeInstance(const TDynamicTypeInstance& Other)
        {
        }

      public:
        T operator*()
        {
            return *m_Instance;
        }

        T* operator->()
        {
            return m_Instance;
        }

      private:
        T* m_Instance = nullptr;
        bool m_Owns = false;
    };
} // namespace DynamicType
