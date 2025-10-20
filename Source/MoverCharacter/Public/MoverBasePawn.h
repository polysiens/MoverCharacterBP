#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "MoverBasePawn.generated.h"

UCLASS(Blueprintable)
class MOVERCHARACTER_API AMoverBasePawn : public APawn
{
	GENERATED_BODY()

public:
	AMoverBasePawn();

	virtual void PreInitializeComponents() override;

	UFUNCTION(BlueprintImplementableEvent, Category = "Initialization", DisplayName = "On Pre-Initialize Components")
	void OnPreInitializeComponents();
};