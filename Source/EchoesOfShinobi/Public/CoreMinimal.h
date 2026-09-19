#pragma once

#if __has_include_next(<CoreMinimal.h>)
#include_next <CoreMinimal.h>
#else

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

#ifndef IMPLEMENT_PRIMARY_GAME_MODULE
#define IMPLEMENT_PRIMARY_GAME_MODULE(ModuleClass, ModuleName, GameModuleName)
#endif

#ifndef DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams
#define DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(DelegateName, Param1Type, Param1Name, Param2Type, Param2Name) \
    struct DelegateName { void Broadcast(Param1Type, Param2Type) {} };
#endif

#ifndef TEXT
#define TEXT(x) L##x
#endif

class FString
{
public:
    FString() {}
    FString(const char*) {}
    FString(const wchar_t*) {}
    const wchar_t* operator*() const { return L""; }
};

class FName
{
public:
    FName() {}
    FName(const char*) {}
    FName(const wchar_t*) {}
    bool IsNone() const { return false; }
    FString ToString() const { return FString(); }
};

class FText
{
public:
    FText() {}
    static FText FromString(const FString&) { return FText(); }
    FString ToString() const { return FString(); }
};

template<typename T>
class TArray
{
public:
    TArray() {}
    int32 Num() const { return 0; }
    void Add(const T&) {}
};

template<typename KeyType, typename ValueType>
class TMap
{
public:
    TMap() {}
    ValueType& FindOrAdd(const KeyType&) { static ValueType Dummy; return Dummy; }
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

#endif
