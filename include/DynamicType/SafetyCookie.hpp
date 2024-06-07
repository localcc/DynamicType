#pragma once
#include <DynamicType/DynamicallySizedWrapper.hpp>

namespace DynamicType
{
    /**
     * @brief Safety cookie for constructors.
     *
     * This safety cookie can only be constructed by library internals.
     */
    class SafetyCookie
    {
      private:
        SafetyCookie() = default;

      private:
        template <DynamicallySizedWrapper>
        friend class TDynamicTypeInstance;
    };
} // namespace DynamicType