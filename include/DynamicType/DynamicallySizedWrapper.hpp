#pragma once

namespace DynamicType
{

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

    /**
     * @brief A concept for types wrapped in @ref TDynamicallySized
     * @tparam T Type
     */
    template <typename T>
    concept DynamicallySizedWrapper = std::same_as<T, TDynamicallySized<typename T::Inner>>;
} // namespace DynamicType