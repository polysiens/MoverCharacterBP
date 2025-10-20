#pragma once

#include "CoreMinimal.h"
#include "MoverDataModelTypes.h"
#include "Math/Vector2D.h"
#include "CharacterInputs_Struct.generated.h"

USTRUCT(BlueprintType)
struct FCharacterInputs_Struct : public FMoverDataStructBase
{
    GENERATED_BODY()

    FCharacterInputs_Struct()
        : jumping(false)
        , CurrentVelocity2D(FVector2D::ZeroVector)
    {
    }

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mover Data")
    bool jumping;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mover Data")
    FVector2D CurrentVelocity2D;

    virtual bool NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess) override;

    virtual FMoverDataStructBase* Clone() const override;
    virtual UScriptStruct* GetScriptStruct() const override;
    virtual bool ShouldReconcile(const FMoverDataStructBase& AuthorityState) const override;
    virtual void Interpolate(const FMoverDataStructBase& From, const FMoverDataStructBase& To, float Pct) override;
};