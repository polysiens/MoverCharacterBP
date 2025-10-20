#include "MoverBasePawn.h"

AMoverBasePawn::AMoverBasePawn()
{
}

void AMoverBasePawn::PreInitializeComponents()
{
	Super::PreInitializeComponents();

	OnPreInitializeComponents();
}