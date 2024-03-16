#include <DynamicType/DynamicType.hpp>

#include <gtest/gtest.h>

namespace dt = DynamicType;

struct TSingleTest_Data
{
  public:
    static struct
    {
        dt::FieldData FirstField;
        dt::FieldData SecondField;
        size_t Size;
    } Offsets;

  public:
    dt::TSingle<int32_t, &Offsets.FirstField> FirstField;
    dt::TSingle<uint64_t, &Offsets.SecondField> SecondField;

  public:
    static void InitializeOffsets()
    {
        Offsets.Size = dt::InitializeOffsets<TSingleTest_Data>();
    }

    static size_t DynamicSize()
    {
        return Offsets.Size;
    }
};

decltype(TSingleTest_Data::Offsets) TSingleTest_Data::Offsets = {};

using TSingleTest = dt::TDynamicallySized<TSingleTest_Data>;

TEST(TSingle, Default)
{
    TSingleTest::InitializeOffsets();

    auto SingleInstance = DT_STACKALLOC(TSingleTest);

    SingleInstance->FirstField.Set(12);
    SingleInstance->SecondField.Set(64);

    EXPECT_EQ(*SingleInstance->FirstField, 12);
    EXPECT_EQ(*SingleInstance->SecondField, 64);
    EXPECT_EQ(TSingleTest::DynamicSize(), sizeof(int32_t) + sizeof(uint64_t));
}

int main(int argc, char** argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
