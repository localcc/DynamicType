# Dynamic type library

A library for using types that change their layout at runtime.

## Why?

The main use is for code that needs to run against several versions of an application, while not being compile for any particular one.

## Example

```cpp
#include <DynamicType/DynamicType.hpp>

#include <print>

namespace dt = DynamicType;

struct FVector_Data
{
  public:
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
        Offsets.Size = dt::InitializeOffsets<FVectorFVector_Data>();
    }

    static size_t DynamicSize()
    {
        return Offsets.Size;
    }
};

decltype(FVector_Data::Settings) FVector_Data::Settings = {};
decltype(FVector_Data::Offsets) FVector_Data::Offsets = {};

using FVector = dt::TDynamicallySized<FVector_Data>;

int main()
{
    FVector::Settings.IsDouble = true;
    FVector::InitializeOffsets();

    auto Instance = DT_STACKALLOC(FVector); // allocate a dynamic instance of FVector on the stack_

    Instance->x.Set(100.0f);
    Instance->y.Set(250.0);
    Instance->z.Set(300.0);

    std::print("X: {}, Y: {}, Z: {}\n", *Instance->x, *Instance->y, *Instance->z);
}
```