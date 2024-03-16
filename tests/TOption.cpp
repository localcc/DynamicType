#include <DynamicType/DynamicType.hpp>

#include <gtest/gtest.h>

namespace dt = DynamicType;

struct TOptionTest_Data
{
  public:
    static struct
    {
        bool IsFirstFieldPresent;
        bool IsSecondFieldPresent;
    } Settings;

    static struct
    {
        dt::FieldData FirstField;
        dt::FieldData SecondField;
        size_t Size;
    } Offsets;

  public:
    dt::TOption<int32_t, &Offsets.FirstField, &Settings.IsFirstFieldPresent> FirstField;
    dt::TOption<uint64_t, &Offsets.SecondField, &Settings.IsSecondFieldPresent> SecondField;

  public:
    static void InitializeOffsets()
    {
        Offsets.Size = dt::InitializeOffsets<TOptionTest_Data>();
    }

    static size_t DynamicSize()
    {
        return Offsets.Size;
    }
};

decltype(TOptionTest_Data::Settings) TOptionTest_Data::Settings = {};
decltype(TOptionTest_Data::Offsets) TOptionTest_Data::Offsets = {};

using TOptionTest = dt::TDynamicallySized<TOptionTest_Data>;

TEST(TOption, First)
{
    TOptionTest::Settings.IsFirstFieldPresent = true;
    TOptionTest::Settings.IsSecondFieldPresent = false;

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
    TOptionTest::Settings.IsFirstFieldPresent = false;
    TOptionTest::Settings.IsSecondFieldPresent = true;

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
    TOptionTest::Settings.IsFirstFieldPresent = true;
    TOptionTest::Settings.IsSecondFieldPresent = true;

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
    static struct
    {
        bool FieldPresent;
    } Settings;
    static struct
    {
        dt::FieldData Field;
        size_t Size;
    } Offsets;

  public:
    dt::TOption<NonDefaultConstructible, &Offsets.Field, &Settings.FieldPresent> Field;

  public:
    static void InitializeOffsets()
    {
        Offsets.Size = dt::InitializeOffsets<TOptionExceptionTest_Data>();
    }

    static size_t DynamicSize()
    {
        return Offsets.Size;
    }
};

decltype(TOptionExceptionTest_Data::Settings) TOptionExceptionTest_Data::Settings = {};
decltype(TOptionExceptionTest_Data::Offsets) TOptionExceptionTest_Data::Offsets = {};

using TOptionExceptionTest = dt::TDynamicallySized<TOptionExceptionTest_Data>;

TEST(TOption, Exception)
{
    TOptionExceptionTest::Settings.FieldPresent = false;
    TOptionExceptionTest::InitializeOffsets();

    auto Instance = DT_STACKALLOC(TOptionExceptionTest);

    EXPECT_THROW(*Instance->Field, std::bad_optional_access);
}

int main(int argc, char** argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
