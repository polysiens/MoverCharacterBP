#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MoverDataModelTypes.h"
#include "MoverBPLibrary.generated.h"

class UBaseMovementMode;
struct FMovementRecord;
struct FFloorCheckResult;
struct FRelativeBaseInfo;

UCLASS()
class MOVERCHARACTER_API UMoverBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Mover Library")
		static void FindFloor(
			const FMovingComponentSet& MovingComps,
			float FloorSweepDistance,
			float MaxWalkSlopeCosine,
			const FVector& Location,
			UPARAM(ref) FFloorCheckResult& OutFloorResult);
	
	
	UFUNCTION(BlueprintCallable, Category = "Mover Library")
	static FRelativeBaseInfo UpdateFloorAndBaseInfo(
		const FFloorCheckResult& FloorResult, UBaseMovementMode* Owner);


	UFUNCTION(BlueprintCallable, Category = "Mover Library")
	static void CaptureFinalState(
		USceneComponent* UpdatedComponent,
		UBaseMovementMode* Owner,
		bool bDidAttemptMovement,
		const FFloorCheckResult& FloorResult,
		const FMovementRecord& Record,
		UPARAM(ref) FMoverDefaultSyncState& OutputSyncState);
};