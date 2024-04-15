#include <DynamicType/DynamicType.hpp>

#include <gtest/gtest.h>

namespace dt = DynamicType;

struct TMixedTest_Data
{
  public:
    using DataType = TMixedTest_Data;

    static struct
    {
        bool EnableOption;
        bool RightSelect;
    } Settings;

    static struct
    {
        dt::FieldData FirstOption;
        dt::FieldData Selectable;
        dt::FieldData SecondOption;
        dt::FieldData Constant;
        size_t Size;
    } Offsets;

  public:
    dt::TOption<int64_t, &Offsets.FirstOption, &Settings.EnableOption> FirstOption;
    dt::TEither<float, double, &Offsets.Selectable, &Settings.RightSelect> Selectable;
    dt::TOption<int32_t, &Offsets.SecondOption, &Settings.EnableOption> SecondOption;
    dt::TSingle<int64_t, &Offsets.Constant> Constant;

  public:
    static void InitializeOffsets()
    {
        Offsets.Size = dt::InitializeOffsets<TMixedTest_Data>();
    }

    static size_t DynamicSize()
    {
        return Offsets.Size;
    }
};

decltype(TMixedTest_Data::Settings) TMixedTest_Data::Settings = {};
decltype(TMixedTest_Data::Offsets) TMixedTest_Data::Offsets = {};

using TMixedTest = dt::TDynamicallySized<TMixedTest_Data>;

TEST(TMixed, Everything)
{
    TMixedTest::Settings.EnableOption = true;
    TMixedTest::Settings.RightSelect = true;

    TMixedTest::InitializeOffsets();

    auto Instance = DT_STACKALLOC(TMixedTest);

    Instance->FirstOption.Set(35);
    Instance->Selectable.Set(350.0);
    Instance->SecondOption.Set(32);
    Instance->Constant.Set(100);

    EXPECT_EQ(*Instance->FirstOption, 35);
    EXPECT_EQ(*Instance->Selectable, 350.0);
    EXPECT_EQ(*Instance->SecondOption, 32);
    EXPECT_EQ(*Instance->Constant, 100);
    EXPECT_EQ(TMixedTest::DynamicSize(), sizeof(int64_t) + sizeof(double) + sizeof(int32_t) + sizeof(int64_t));
}

TEST(TMixed, NoOptions)
{
    TMixedTest::Settings.EnableOption = false;
    TMixedTest::Settings.RightSelect = true;

    TMixedTest::InitializeOffsets();

    auto Instance = DT_STACKALLOC(TMixedTest);

    Instance->Selectable.Set(350.0);
    Instance->Constant.Set(100);

    EXPECT_EQ(*Instance->Selectable, 350.0);
    EXPECT_EQ(*Instance->Constant, 100);
    EXPECT_EQ(TMixedTest::DynamicSize(), sizeof(double) + sizeof(int64_t));
}

TEST(TMixed, LeftSelectWithOptions)
{
    TMixedTest::Settings.EnableOption = true;
    TMixedTest::Settings.RightSelect = false;

    TMixedTest::InitializeOffsets();

    auto Instance = DT_STACKALLOC(TMixedTest);

    Instance->FirstOption.Set(3100);
    Instance->Selectable.Set(100.0f);
    Instance->SecondOption.Set(100);
    Instance->Constant.Set(250);

    EXPECT_EQ(*Instance->FirstOption, 3100);
    EXPECT_EQ(*Instance->Selectable, 100.0f);
    EXPECT_EQ(*Instance->SecondOption, 100);
    EXPECT_EQ(*Instance->Constant, 250);
    EXPECT_EQ(TMixedTest::DynamicSize(), sizeof(int64_t) + sizeof(float) + sizeof(int32_t) + sizeof(int64_t));
}

TEST(TMixed, LeftSelectNoOption)
{
    TMixedTest::Settings.EnableOption = false;
    TMixedTest::Settings.RightSelect = false;

    TMixedTest::InitializeOffsets();

    auto Instance = DT_STACKALLOC(TMixedTest);

    Instance->Selectable.Set(100.0f);
    Instance->Constant.Set(250);

    EXPECT_EQ(*Instance->Selectable, 100.0f);
    EXPECT_EQ(*Instance->Constant, 250);
    EXPECT_EQ(TMixedTest::DynamicSize(), sizeof(float) + sizeof(int64_t));
}

int main(int argc, char** argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
