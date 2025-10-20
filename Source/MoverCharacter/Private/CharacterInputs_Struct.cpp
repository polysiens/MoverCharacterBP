#include "CharacterInputs_Struct.h"
#include "MoverDataModelTypes.h"
#include "MoverLog.h"
#include "Math/Vector2D.h"
#include "Math/UnrealMathUtility.h"

bool FCharacterInputs_Struct::NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess)
{
    Super::NetSerialize(Ar, Map, bOutSuccess);

    Ar << jumping;
    Ar << CurrentVelocity2D;

    bOutSuccess = true;
    return true;
}

FMoverDataStructBase* FCharacterInputs_Struct::Clone() const
{
    FCharacterInputs_Struct* CopyPtr = new FCharacterInputs_Struct(*this);
    return CopyPtr;
}

UScriptStruct* FCharacterInputs_Struct::GetScriptStruct() const
{
    return FCharacterInputs_Struct::StaticStruct();
}

bool FCharacterInputs_Struct::ShouldReconcile(const FMoverDataStructBase& AuthorityState) const
{
    const FCharacterInputs_Struct* AuthorityTestState = static_cast<const FCharacterInputs_Struct*>(&AuthorityState);

    if (jumping != AuthorityTestState->jumping)
    {
        return true;
    }

    if (!CurrentVelocity2D.Equals(AuthorityTestState->CurrentVelocity2D, 0.1f))
    {
        return true;
    }

    return false;
}

void FCharacterInputs_Struct::Interpolate(const FMoverDataStructBase& From, const FMoverDataStructBase& To, float Pct)
{
    const FCharacterInputs_Struct* FromState = static_cast<const FCharacterInputs_Struct*>(&From);
    const FCharacterInputs_Struct* ToState = static_cast<const FCharacterInputs_Struct*>(&To);

    this->jumping = ToState->jumping;

    this->CurrentVelocity2D = FVector2D(
        FMath::Clamp(ToState->CurrentVelocity2D.X, -1.0f, 1.0f),
        FMath::Clamp(ToState->CurrentVelocity2D.Y, -1.0f, 1.0f)
    );
}