#include <DynamicType/DynamicType.hpp>

#include <gtest/gtest.h>
#include <print>

namespace dt = DynamicType;

struct TEitherTest_Data
{
  public:
    using DataType = TEitherTest_Data;

    static struct
    {
        bool IsDouble;
    } Settings;

    static struct
    {
        dt::FieldData x;
        dt::FieldData y;
        dt::FieldData z;
        size_t Size;
    } Offsets;

  public:
    dt::TEither<float, double, &Offsets.x, &Settings.IsDouble> x;
    dt::TEither<float, double, &Offsets.y, &Settings.IsDouble> y;
    dt::TEither<float, double, &Offsets.z, &Settings.IsDouble> z;

  public:
    static void InitializeOffsets()
    {
        Offsets.Size = dt::InitializeOffsets<TEitherTest_Data>();
    }

    static size_t DynamicSize()
    {
        return Offsets.Size;
    }
};

decltype(TEitherTest_Data::Settings) TEitherTest_Data::Settings = {};
decltype(TEitherTest_Data::Offsets) TEitherTest_Data::Offsets = {};

using TEitherTest = dt::TDynamicallySized<TEitherTest_Data>;

TEST(TEither, Left)
{
    TEitherTest::Settings.IsDouble = false;
    TEitherTest::InitializeOffsets();

    auto Instance = DT_STACKALLOC(TEitherTest);

    Instance->x.Set(100.0f);
    Instance->y.Set(200.0f);
    Instance->z.Set(350.0f);

    EXPECT_EQ(*Instance->x, 100.0f);
    EXPECT_EQ(*Instance->y, 200.0f);
    EXPECT_EQ(*Instance->z, 350.0f);
    EXPECT_EQ(TEitherTest::DynamicSize(), sizeof(float) * 3);
}

TEST(TEither, Right)
{
    TEitherTest::Settings.IsDouble = true;
    TEitherTest::InitializeOffsets();

    auto Instance = DT_STACKALLOC(TEitherTest);

    Instance->x.Set(100.0);
    Instance->y.Set(200.0);
    Instance->z.Set(350.0);

    EXPECT_EQ(*Instance->x, 100.0);
    EXPECT_EQ(*Instance->y, 200.0);
    EXPECT_EQ(*Instance->z, 350.0);
    EXPECT_EQ(TEitherTest::DynamicSize(), sizeof(double) * 3);
}

int main(int argc, char** argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
