#include "MoverBPLibrary.h"
#include "MovementMode.h"
#include "MoverComponent.h"
#include "MoveLibrary/FloorQueryUtils.h"
#include "MoveLibrary/BasedMovementUtils.h"

void UMoverBPLibrary::FindFloor(
	const FMovingComponentSet& MovingComps,
	float FloorSweepDistance,
	float MaxWalkSlopeCosine,
	const FVector& Location,
	UPARAM(ref) FFloorCheckResult& OutFloorResult)
{
	UFloorQueryUtils::FindFloor(
		MovingComps, FloorSweepDistance, MaxWalkSlopeCosine, Location, OutFloorResult);
}

FRelativeBaseInfo UMoverBPLibrary::UpdateFloorAndBaseInfo(
	const FFloorCheckResult& FloorResult, UBaseMovementMode* Owner)
{
	FRelativeBaseInfo ReturnBaseInfo;
	if (!Owner || !Owner->GetMoverComponent())
	{
		return ReturnBaseInfo;
	}

	UMoverBlackboard* SimBlackboard =
		Owner->GetMoverComponent()->GetSimBlackboard_Mutable();
	if (!SimBlackboard)
	{
		return ReturnBaseInfo;
	}

	SimBlackboard->Set(CommonBlackboard::LastFloorResult, FloorResult);

	if (FloorResult.IsWalkableFloor() &&
		UBasedMovementUtils::IsADynamicBase(FloorResult.HitResult.GetComponent()))
	{
		ReturnBaseInfo.SetFromFloorResult(FloorResult);
	}

	return ReturnBaseInfo;
}

void UMoverBPLibrary::CaptureFinalState(
	USceneComponent* UpdatedComponent,
	UBaseMovementMode* Owner,
	bool bDidAttemptMovement,
	const FFloorCheckResult& FloorResult,
	const FMovementRecord& Record,
	UPARAM(ref) FMoverDefaultSyncState& OutputSyncState)
{
	if (!UpdatedComponent || !Owner || !Owner->GetMoverComponent())
	{
		return;
	}

	FRelativeBaseInfo PriorBaseInfo;
	UMoverBlackboard* SimBlackboard =
		Owner->GetMoverComponent()->GetSimBlackboard_Mutable();

	const bool bHasPriorBaseInfo =
		SimBlackboard
		? SimBlackboard->TryGet(
			CommonBlackboard::LastFoundDynamicMovementBase, PriorBaseInfo)
		: false;

	FRelativeBaseInfo CurrentBaseInfo =
		UMoverBPLibrary::UpdateFloorAndBaseInfo(FloorResult, Owner);

	if (CurrentBaseInfo.HasRelativeInfo() && bHasPriorBaseInfo &&
		!bDidAttemptMovement && PriorBaseInfo.UsesSameBase(CurrentBaseInfo))
	{
		CurrentBaseInfo.ContactLocalPosition = PriorBaseInfo.ContactLocalPosition;
	}

	if (CurrentBaseInfo.HasRelativeInfo())
	{
		if (SimBlackboard)
		{
			SimBlackboard->Set(
				CommonBlackboard::LastFoundDynamicMovementBase, CurrentBaseInfo);
		}

		OutputSyncState.SetTransforms_WorldSpace(
			UpdatedComponent->GetComponentLocation(),
			UpdatedComponent->GetComponentRotation(),
			Record.GetRelevantVelocity(),
			CurrentBaseInfo.MovementBase.Get(),
			CurrentBaseInfo.BoneName);
	}
	else
	{
		if (SimBlackboard)
		{
			SimBlackboard->Invalidate(CommonBlackboard::LastFoundDynamicMovementBase);
		}

		OutputSyncState.SetTransforms_WorldSpace(
			UpdatedComponent->GetComponentLocation(),
			UpdatedComponent->GetComponentRotation(),
			Record.GetRelevantVelocity(),
			nullptr);
	}

	UpdatedComponent->ComponentVelocity = OutputSyncState.GetVelocity_WorldSpace();
}