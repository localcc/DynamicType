#include <iostream>
#include <DynamicType/DynamicType.hpp>
#include <print>

using namespace DynamicType;
//
// struct OptionTest_Data
// {
//     static struct
//     {
//         bool IsPresent;
//     } Settings;
//     static struct
//     {
//         FieldData TestField;
//         size_t Size;
//     } Offsets;
//
//   public:
//     TOption<double, &Offsets.TestField, &Settings.IsPresent> TestField;
//
//   public:
//     static void InitializeOffsets()
//     {
//         Offsets.Size = DynamicType::InitializeOffsets<OptionTest_Data>();
//     }
//
//     static size_t DynamicSize()
//     {
//         return Offsets.Size;
//     }
// };
//
// decltype(OptionTest_Data::Settings) OptionTest_Data::Settings = {};
// decltype(OptionTest_Data::Offsets) OptionTest_Data::Offsets = {};
//
// using OptionTest = TDynamicallySized<OptionTest_Data>;
//
// struct ConstructorTest
// {
//     double Value = 0.0;
//
//     ConstructorTest()
//     {
//     }
//     ConstructorTest(double Value)
//     {
//         this->Value = Value;
//     }
// };
//
// struct SingleTest_Data
// {
//     static struct
//     {
//         FieldData TestField;
//         size_t Size;
//     } Offsets;
//
//   public:
//     TSingle<double, &Offsets.TestField> TestField;
//
//   public:
//     static void InitializeOffsets()
//     {
//         Offsets.Size = DynamicType::InitializeOffsets<SingleTest_Data>();
//     }
//
//     static size_t DynamicSize()
//     {
//         return Offsets.Size;
//     }
// };
//
// decltype(SingleTest_Data::Offsets) SingleTest_Data::Offsets = {};
//
// struct SingleTest_Constructor : public SingleTest_Data
// {
//     SingleTest_Constructor()
//     {
//     }
//
//     SingleTest_Constructor(double Value)
//     {
//         this->TestField = Value;
//     }
// };
//
// using SingleTest = TDynamicallySized<SingleTest_Constructor>;

struct FVector_Data
{
  public:
    using DataType = FVector_Data;

    static struct
    {
        bool IsPlus500 = false;
    } Settings;
    static struct
    {
        FieldData x;
        FieldData y;
        FieldData z;
        size_t Size;
    } Offsets;

  public:
    TEither<float, double, &Offsets.x, &Settings.IsPlus500> x;
    TEither<float, double, &Offsets.y, &Settings.IsPlus500> y;
    TEither<float, double, &Offsets.z, &Settings.IsPlus500> z;

  public:
    static void InitializeOffsets()
    {
        Offsets.Size = DynamicType::InitializeOffsets<FVector_Data>();
    }

    static size_t DynamicSize()
    {
        return Offsets.Size;
    }
};

decltype(FVector_Data::Settings) FVector_Data::Settings = {};
decltype(FVector_Data::Offsets) FVector_Data::Offsets = {};

struct FVector_Constructor : public FVector_Data
{
    FVector_Constructor()
    {
    }

    FVector_Constructor(double X, double Y, double Z)
    {
        this->x.Set(X);
        this->y.Set(Y);
        this->z.Set(Z);
    }
};

using FVector = TDynamicallySized<FVector_Constructor>;

int main(int argc, char** argv)
{
    FVector::InitializeOffsets();

    TDynamicTypeInstance<FVector> Test = DT_STACKALLOC(FVector, 0, 150.0, 250.0f);
    std::print("X: {}, Y: {}, Z: {}\n", *Test->x, *Test->y, *Test->z);

    Test->x.Set(1400.0f);

    std::print("X: {}, Y: {}, Z: {}\n", *Test->x, *Test->y, *Test->z);
    //     OptionTest::Settings.IsPresent = true;
    //     OptionTest::InitializeOffsets();
    //
    //     {
    //         TDynamicTypeInstance<OptionTest> First{};
    //
    //         First->TestField.Set(123.0);
    //         std::print("Test: {}\n", *First->TestField);
    //     }
    //
    //     OptionTest::Settings.IsPresent = false;
    //     OptionTest::InitializeOffsets();
    //
    //     {
    //         TDynamicTypeInstance<OptionTest> Second{};
    //
    //         Second->TestField.Set(123.0);
    //         std::print("after: {}\n", *Second->TestField);
    //     }

    //     FVector::Settings.IsPlus500 = false;
    //     FVector::InitializeOffsets();
    //
    //     {
    //         TDynamicTypeInstance<FVector> First{};
    //
    //         First->x.Set(123.0f);
    //         First->y.Set(456.0);
    //         PrintVec(*First);
    //
    //         TDynamicTypeInstance<FVector> Second{};
    //         Second->x.Set(999.0f);
    //         PrintVec(*Second);
    //
    //         Second->x = First->x;
    //
    //         PrintVec(*First);
    //         PrintVec(*Second);
    //     }
    //
    //     std::print("Changing size\n");
    //
    //     FVector::Settings.IsPlus500 = true;
    //     FVector::InitializeOffsets();
    //
    //     {
    //         TDynamicTypeInstance<FVector> First{};
    //
    //         First->x.Set(123.0f);
    //         First->y.Set(456.0);
    //         std::print("X: {} Y: {} Z: {}\n", *First->x, *First->y, *First->z);
    //         std::print("Size: {}\n", FVector::DynamicSize());
    //     }

    return 0;
}
