#include <DynamicType/DynamicType.hpp>

#include <print>
#include <iostream>
#include <gtest/gtest.h>

namespace dt = DynamicType;

struct TSingleTest_Data
{
  public:
    explicit TSingleTest_Data(dt::SafetyCookie Cookie) : FirstField(Cookie, 0), SecondField(Cookie, 0)
    {
    }

    TSingleTest_Data(dt::SafetyCookie Cookie, int32_t FirstField, uint64_t SecondField) : FirstField(Cookie, FirstField), SecondField(Cookie, SecondField)
    {
    }

  public:
    using DataType = TSingleTest_Data;

    struct Offsets
    {
        static inline dt::FieldData FirstField;
        static inline dt::FieldData SecondField;
        static inline size_t Size;
    };

  public:
    dt::TSingle<int32_t, &Offsets::FirstField> FirstField;
    dt::TSingle<uint64_t, &Offsets::SecondField> SecondField;

  public:
    static void InitializeOffsets()
    {
        Offsets::Size = dt::InitializeOffsets<TSingleTest_Data>(&TSingleTest_Data::FirstField, &TSingleTest_Data::SecondField);
    }

    static size_t DynamicSize()
    {
        return Offsets::Size;
    }
};

using TSingleTest = dt::TDynamicallySized<TSingleTest_Data>;

TEST(TSingle, Paramless)
{
    TSingleTest::InitializeOffsets();

    auto SingleInstance = DT_STACKALLOC(TSingleTest);

    SingleInstance->FirstField.Set(12);
    SingleInstance->SecondField.Set(64);

    EXPECT_EQ(*SingleInstance->FirstField, 12);
    EXPECT_EQ(*SingleInstance->SecondField, 64);
    EXPECT_EQ(TSingleTest::DynamicSize(), sizeof(int32_t) + sizeof(uint64_t));
}

TEST(TSingle, Params)
{
    TSingleTest::InitializeOffsets();

    auto SingleInstance = DT_STACKALLOC(TSingleTest, 32, 64);

    EXPECT_EQ(*SingleInstance->FirstField, 32);
    EXPECT_EQ(*SingleInstance->SecondField, 64);
    EXPECT_EQ(TSingleTest::DynamicSize(), sizeof(int32_t) + sizeof(uint64_t));
}

int main(int argc, char** argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
