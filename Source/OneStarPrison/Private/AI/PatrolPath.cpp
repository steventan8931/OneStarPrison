// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/PatrolPath.h"

// Sets default values
APatrolPath::APatrolPath()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

FTransform APatrolPath::GetPatrolPoint(int const Index) const
{
	if (PatrolPoints.Num() > 0)
	{
		return PatrolPoints[Index];
	}
	else
	{
		return FTransform::Identity;
	}
}

int APatrolPath::Num() const
{
	return PatrolPoints.Num();
}

void APatrolPath::AddPatrolPoint(FTransform const& NewPoint)
{
	PatrolPoints.Add(NewPoint);
}
