#pragma once

#ifndef ECHOESOFSHINOBI_API
#define ECHOESOFSHINOBI_API
#endif

// Custom Tailored Unreal Engine Core Types & Macro Fallbacks for IDE Static Analysis
typedef unsigned char uint8;
typedef signed int int32;

#ifndef UCLASS
#define UCLASS(...)
#endif

#ifndef USTRUCT
#define USTRUCT(...)
#endif

#ifndef UENUM
#define UENUM(...)
#endif

#ifndef UPROPERTY
#define UPROPERTY(...)
#endif

#ifndef UFUNCTION
#define UFUNCTION(...)
#endif

#ifndef UMETA
#define UMETA(...)
#endif

#ifndef GENERATED_BODY
#define GENERATED_BODY(...)
#endif

#ifndef ATTRIBUTE_ACCESSORS
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName)
#endif

#ifndef DECLARE_LOG_CATEGORY_EXTERN
#define DECLARE_LOG_CATEGORY_EXTERN(CategoryName, DefaultVerbosity, CompileTimeVerbosity) \
    struct FLogCategory##CategoryName {}; \
    extern FLogCategory##CategoryName CategoryName;
#endif

#ifndef DEFINE_LOG_CATEGORY
#define DEFINE_LOG_CATEGORY(CategoryName) \
    FLogCategory##CategoryName CategoryName;
#endif

#ifndef UE_LOG
#define UE_LOG(Category, Verbosity, Format, ...)
#endif

#ifndef Log
#define Log 0
#endif
#ifndef Warning
#define Warning 1
#endif
#ifndef Error
#define Error 2
#endif

#ifndef IMPLEMENT_PRIMARY_GAME_MODULE
#define IMPLEMENT_PRIMARY_GAME_MODULE(ModuleClass, ModuleName, GameModuleName)
#endif

#ifndef DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam
#define DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(DelegateName, Param1Type, Param1Name) \
    struct DelegateName { bool IsBound() const { return true; } void Broadcast(Param1Type) {} };
#endif

#ifndef DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams
#define DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(DelegateName, Param1Type, Param1Name, Param2Type, Param2Name) \
    struct DelegateName { bool IsBound() const { return true; } void Broadcast(Param1Type, Param2Type) {} };
#endif

#include <string>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <utility>
#include <iostream>

#ifndef TEXT
#define TEXT(x) L##x
#endif

class FString
{
private:
    std::string Str;

public:
    FString() : Str("") {}
    FString(const char* InStr) : Str(InStr ? InStr : "") {}
    FString(const wchar_t* InWStr)
    {
        if (InWStr)
        {
            while (*InWStr)
            {
                Str.push_back(static_cast<char>(*InWStr++));
            }
        }
    }
    FString(const std::string& InStr) : Str(InStr) {}

    const char* operator*() const { return Str.c_str(); }
    operator const char*() const { return Str.c_str(); }
    operator std::string() const { return Str; }

    bool operator==(const FString& Other) const { return Str == Other.Str; }
    bool operator!=(const FString& Other) const { return Str != Other.Str; }
    bool operator<(const FString& Other) const { return Str < Other.Str; }
    FString operator+(const FString& Other) const { return FString(Str + Other.Str); }
    FString& operator+=(const FString& Other) { Str += Other.Str; return *this; }
    bool IsEmpty() const { return Str.empty(); }
    int32 Len() const { return static_cast<int32>(Str.length()); }

    template<typename... Args>
    static FString Printf(const wchar_t*, Args...) { return FString(); }
};

class FName
{
private:
    std::string Name;

public:
    FName() : Name("") {}
    FName(const char* InName) : Name(InName ? InName : "") {}
    FName(const wchar_t* InWName)
    {
        if (InWName)
        {
            while (*InWName)
            {
                Name.push_back(static_cast<char>(*InWName++));
            }
        }
    }
    bool IsNone() const { return Name.empty(); }
    FString ToString() const { return FString(Name.c_str()); }
    bool operator==(const FName& Other) const { return Name == Other.Name; }
    bool operator<(const FName& Other) const { return Name < Other.Name; }
};

class FText
{
private:
    std::string Text;

public:
    FText() : Text("") {}
    FText(const char* InText) : Text(InText ? InText : "") {}
    FText(const wchar_t* InWText)
    {
        if (InWText)
        {
            while (*InWText)
            {
                Text.push_back(static_cast<char>(*InWText++));
            }
        }
    }
    FText& operator=(const char* InText) { Text = (InText ? InText : ""); return *this; }
    FText& operator=(const wchar_t* InWText)
    {
        Text.clear();
        if (InWText)
        {
            while (*InWText)
            {
                Text.push_back(static_cast<char>(*InWText++));
            }
        }
        return *this;
    }
    static FText FromString(const FString& InStr) { return FText(*InStr); }
    FString ToString() const { return FString(Text.c_str()); }
};

template<typename T>
class TArray
{
private:
    std::vector<T> Items;

public:
    TArray() {}
    TArray(std::initializer_list<T> InList) : Items(InList) {}

    int32 Num() const { return static_cast<int32>(Items.size()); }
    void Add(const T& Item) { Items.push_back(Item); }
    void AddUnique(const T& Item)
    {
        if (!Contains(Item))
        {
            Items.push_back(Item);
        }
    }
    T Pop()
    {
        if (Items.empty()) return T();
        T Val = Items.back();
        Items.pop_back();
        return Val;
    }
    void Empty() { Items.clear(); }
    T& Last() { return Items.back(); }
    const T& Last() const { return Items.back(); }
    void RemoveAt(int32 Index)
    {
        if (Index >= 0 && Index < static_cast<int32>(Items.size()))
        {
            Items.erase(Items.begin() + Index);
        }
    }
    bool Contains(const T& Item) const
    {
        for (const auto& It : Items)
        {
            if (It == Item) return true;
        }
        return false;
    }
    bool IsValidIndex(int32 Index) const
    {
        return Index >= 0 && Index < static_cast<int32>(Items.size());
    }
    T& operator[](int32 Index) { return Items[Index]; }
    const T& operator[](int32 Index) const { return Items[Index]; }

    auto begin() { return Items.begin(); }
    auto end() { return Items.end(); }
    auto begin() const { return Items.begin(); }
    auto end() const { return Items.end(); }
};

template<typename KeyType, typename ValueType>
class TMap
{
private:
    std::map<KeyType, ValueType> Map;

public:
    TMap() {}

    int32 Num() const { return static_cast<int32>(Map.size()); }
    void Add(const KeyType& Key, const ValueType& Value) { Map[Key] = Value; }
    bool Contains(const KeyType& Key) const { return Map.find(Key) != Map.end(); }
    void Empty() { Map.clear(); }

    ValueType* Find(const KeyType& Key)
    {
        auto It = Map.find(Key);
        return (It != Map.end()) ? &(It->second) : nullptr;
    }
    const ValueType* Find(const KeyType& Key) const
    {
        auto It = Map.find(Key);
        return (It != Map.end()) ? &(It->second) : nullptr;
    }
    ValueType& FindOrAdd(const KeyType& Key) { return Map[Key]; }

    ValueType& operator[](const KeyType& Key) { return Map[Key]; }
    const ValueType& operator[](const KeyType& Key) const { return Map.at(Key); }

    auto begin() { return Map.begin(); }
    auto end() { return Map.end(); }
    auto begin() const { return Map.begin(); }
    auto end() const { return Map.end(); }
};

template<typename T>
class TSet
{
private:
    std::set<T> Set;

public:
    TSet() {}
    void Add(const T& Item) { Set.insert(Item); }
    void Remove(const T& Item) { Set.erase(Item); }
    bool Contains(const T& Item) const { return Set.find(Item) != Set.end(); }
    int32 Num() const { return static_cast<int32>(Set.size()); }
    void Empty() { Set.clear(); }

    auto begin() { return Set.begin(); }
    auto end() { return Set.end(); }
    auto begin() const { return Set.begin(); }
    auto end() const { return Set.end(); }
};

template<typename T>
class TObjectPtr
{
public:
    TObjectPtr() : Ptr(nullptr) {}
    TObjectPtr(T* InPtr) : Ptr(InPtr) {}
    T* operator->() const { return Ptr; }
    operator bool() const { return Ptr != nullptr; }
private:
    T* Ptr;
};

struct FVector2D
{
    float X = 0.0f;
    float Y = 0.0f;
    FVector2D() {}
    FVector2D(float InX, float InY) : X(InX), Y(InY) {}
    bool operator==(const FVector2D& Other) const { return true; }
    FVector2D operator*(float Scale) const { return FVector2D(X * Scale, Y * Scale); }
};

struct FVector
{
    float X = 0.0f;
    float Y = 0.0f;
    float Z = 0.0f;
    FVector() {}
    FVector(float InX, float InY, float InZ) : X(InX), Y(InY), Z(InZ) {}
    static FVector ZeroVector;
};

struct FRotator
{
    float Pitch = 0.0f;
    float Yaw = 0.0f;
    float Roll = 0.0f;
    FRotator() {}
    FRotator(float InP, float InY, float InR) : Pitch(InP), Yaw(InY), Roll(InR) {}
    static FRotator ZeroRotator;
};

struct FMath
{
    static float Clamp(float Val, float Min, float Max) { return Val < Min ? Min : (Val > Max ? Max : Val); }
    static bool IsNearlyEqual(float A, float B, float ErrorTolerance = 1.e-4f) { return true; }
    static float Sin(float Val) { return 0.0f; }
    static float Cos(float Val) { return 0.0f; }
};

struct FGameplayAttributeData { float BaseValue; float CurrentValue; };
struct FGameplayAttribute { FName AttributeName; };
struct FGameplayAbilitySpecHandle {};
struct FGameplayAbilityActorInfo {};
struct FGameplayAbilityActivationInfo {};
struct FGameplayEventData {};
struct FSubsystemCollectionBase {};

class UObject {};
class AActor : public UObject {};
class ACharacter : public AActor
{
public:
    ACharacter() {}
    virtual void BeginPlay() {}
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) {}
};
class UActorComponent : public UObject {};
class UPrimaryDataAsset : public UObject
{
public:
    virtual struct FPrimaryAssetId GetPrimaryAssetId() const;
};

struct FPrimaryAssetId
{
    FName PrimaryAssetType;
    FName PrimaryAssetName;
    FPrimaryAssetId() {}
    FPrimaryAssetId(FName Type, FName Name) : PrimaryAssetType(Type), PrimaryAssetName(Name) {}
};

class UAttributeSet : public UObject
{
public:
    virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) {}
    virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) {}
};

class UAbilitySystemComponent : public UActorComponent
{
public:
    void SetIsReplicated(bool bReplicated) {}
    void SetReplicationMode(int Mode) {}
    void InitAbilityActorInfo(UObject* Owner, UObject* Avatar) {}
};

class UGameplayAbility : public UObject
{
public:
    int InstancingPolicy;
    virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) {}
};

class UEnhancedInputComponent : public UActorComponent {};
class UGameInstanceSubsystem : public UObject
{
public:
    virtual void Initialize(FSubsystemCollectionBase& Collection) {}
    virtual void Deinitialize() {}
};
class UInputComponent {};
class FDefaultGameModuleImpl {};

namespace EGameplayEffectReplicationMode { enum Type { Mixed }; }
namespace EGameplayAbilityInstancingPolicy { enum Type { InstancedPerActor }; }

struct FLinearColor
{
    float R = 0.0f;
    float G = 0.0f;
    float B = 0.0f;
    float A = 1.0f;

    FLinearColor() : R(0.0f), G(0.0f), B(0.0f), A(1.0f) {}
    FLinearColor(float InR, float InG, float InB, float InA = 1.0f) : R(InR), G(InG), B(InB), A(InA) {}
};
