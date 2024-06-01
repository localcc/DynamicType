#include <DynamicType/DynamicType.hpp>

#include <gtest/gtest.h>

namespace dt = DynamicType;

struct Item_Data
{
  public:
    using DataType = Item_Data;

    struct Offsets
    {
        static inline dt::FieldData First;
        static inline dt::FieldData Second;
        static inline dt::FieldData Third;
        static inline size_t Size;
    };

    struct Settings
    {
        static inline bool IsRight;
    };

  public:
    dt::TSingle<float, &Offsets::First> First;
    dt::TEither<int32_t, int64_t, &Offsets::Second, &Settings::IsRight> Second;
    dt::TSingle<float, &Offsets::Third> Third;

  public:
    static void InitializeOffsets()
    {
        Offsets::Size = dt::InitializeOffsets<Item_Data>();
    }

    static size_t DynamicSize()
    {
        return Offsets::Size;
    }
};

using Item = dt::TDynamicallySized<Item_Data>;

struct Container_Data
{
  public:
    using DataType = Container_Data;

    struct Offsets
    {
        static inline dt::FieldData FirstItem;
        static inline dt::FieldData SecondItem;
        static inline size_t Size;
    };

  public:
    dt::TSingle<Item, &Offsets::FirstItem> FirstItem;
    dt::TSingle<Item, &Offsets::SecondItem> SecondItem;

  public:
    static void InitializeOffsets()
    {
        Offsets::Size = dt::InitializeOffsets<Container_Data>();
    }

    static size_t DynamicSize()
    {
        return Offsets::Size;
    }
};

using Container = dt::TDynamicallySized<Container_Data>;

void TestItem(const Item& Fill, float First, int64_t Second, float Third)
{
    EXPECT_FLOAT_EQ(*Fill.First, First);
    EXPECT_EQ(*Fill.Second, Second);
    EXPECT_FLOAT_EQ(*Fill.Third, Third);
}

void FillItem(Item& Fill, float First, int64_t Second, float Third)
{
    Fill.First = First;
    Fill.Second.Set(Second);
    Fill.Third = Third;
    TestItem(Fill, First, Second, Third);
}

TEST(Nested, Default)
{
    Item::Settings::IsRight = false;
    Item::InitializeOffsets();

    Container::InitializeOffsets();

    auto FirstItem = DT_STACKALLOC(Item);
    FillItem(*FirstItem, 12.0f, 128, 45.0f);

    auto SecondItem = DT_STACKALLOC(Item);
    FillItem(*SecondItem, 64.0f, 256, 113.0f);

    auto Cont = DT_STACKALLOC(Container);
    Cont->FirstItem = *FirstItem;
    Cont->SecondItem = *SecondItem;

    TestItem(*Cont->FirstItem, 12.0f, 128, 45.0f);
    TestItem(*Cont->SecondItem, 64.0f, 256, 113.0f);
}

int main(int argc, char** argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}