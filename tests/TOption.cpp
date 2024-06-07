#include <DynamicType/DynamicType.hpp>

#include <gtest/gtest.h>

namespace dt = DynamicType;

struct TOptionTest_Data
{
  public:
    explicit TOptionTest_Data(dt::SafetyCookie Cookie) : FirstField(Cookie), SecondField(Cookie)
    {
    }

  public:
    struct Settings
    {
        static inline bool IsFirstFieldPresent;
        static inline bool IsSecondFieldPresent;
    };

    struct Offsets
    {
        static inline dt::FieldData FirstField;
        static inline dt::FieldData SecondField;
        static inline size_t Size;
    };

  public:
    dt::TOption<int32_t, &Offsets::FirstField, &Settings::IsFirstFieldPresent> FirstField;
    dt::TOption<uint64_t, &Offsets::SecondField, &Settings::IsSecondFieldPresent> SecondField;

  public:
    static void InitializeOffsets()
    {
        Offsets::Size = dt::InitializeOffsets<TOptionTest_Data>(&TOptionTest_Data::FirstField, &TOptionTest_Data::SecondField);
    }

    static size_t DynamicSize()
    {
        return Offsets::Size;
    }
};

using TOptionTest = dt::TDynamicallySized<TOptionTest_Data>;

TEST(TOption, First)
{
    TOptionTest::Settings::IsFirstFieldPresent = true;
    TOptionTest::Settings::IsSecondFieldPresent = false;

    TOptionTest::InitializeOffsets();

    auto Instance = DT_STACKALLOC(TOptionTest);

    Instance->FirstField.Set(12);
    Instance->SecondField.Set(64);

    EXPECT_EQ(*Instance->FirstField, 12);
    EXPECT_EQ(*Instance->SecondField, 0); // should be 0, as the field is not present
    EXPECT_EQ(TOptionTest::DynamicSize(), sizeof(int32_t));
}

TEST(TOption, Second)
{
    TOptionTest::Settings::IsFirstFieldPresent = false;
    TOptionTest::Settings::IsSecondFieldPresent = true;

    TOptionTest::InitializeOffsets();

    auto Instance = DT_STACKALLOC(TOptionTest);

    Instance->FirstField.Set(12);
    Instance->SecondField.Set(64);

    EXPECT_EQ(*Instance->FirstField, 0); // should be 0, as the field is not present
    EXPECT_EQ(*Instance->SecondField, 64);
    EXPECT_EQ(TOptionTest::DynamicSize(), sizeof(uint64_t));
}

TEST(TOption, Third)
{
    TOptionTest::Settings::IsFirstFieldPresent = true;
    TOptionTest::Settings::IsSecondFieldPresent = true;

    TOptionTest::InitializeOffsets();

    auto Instance = DT_STACKALLOC(TOptionTest);

    Instance->FirstField.Set(12);
    Instance->SecondField.Set(64);

    EXPECT_EQ(*Instance->FirstField, 12);
    EXPECT_EQ(*Instance->SecondField, 64);
    EXPECT_EQ(TOptionTest::DynamicSize(), sizeof(int32_t) + sizeof(uint64_t));
}

struct NonDefaultConstructible
{
    NonDefaultConstructible(int)
    {
    }
};

struct TOptionExceptionTest_Data
{
  public:
    explicit TOptionExceptionTest_Data(dt::SafetyCookie Cookie) : Field(Cookie, NonDefaultConstructible(0))
    {
    }

  public:
    using DataType = TOptionExceptionTest_Data;

    struct Settings
    {
        static inline bool FieldPresent;
    };
    struct Offsets
    {
        static inline dt::FieldData Field;
        static inline size_t Size;
    };

  public:
    dt::TOption<NonDefaultConstructible, &Offsets::Field, &Settings::FieldPresent> Field;

  public:
    static void InitializeOffsets()
    {
        using T = TOptionExceptionTest_Data;
        Offsets::Size = dt::InitializeOffsets<TOptionExceptionTest_Data>(&T::Field);
    }

    static size_t DynamicSize()
    {
        return Offsets::Size;
    }
};

using TOptionExceptionTest = dt::TDynamicallySized<TOptionExceptionTest_Data>;

TEST(TOption, Exception)
{
    TOptionExceptionTest::Settings::FieldPresent = false;
    TOptionExceptionTest::InitializeOffsets();

    auto Instance = DT_STACKALLOC(TOptionExceptionTest);

    EXPECT_THROW(*Instance->Field, std::bad_optional_access);
}

int main(int argc, char** argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
