// Fill out your copyright notice in the Description page of Project Settings.

#include "ChooseNextWaypoint.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "PatrolRoute.h"

EBTNodeResult::Type UChooseNextWaypoint::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	UE_LOG(LogTemp, Warning, TEXT("Executing"));
	auto BlackboardComponent = OwnerComp.GetBlackboardComponent();

	// Get the AI Pawn
	auto AIOwner = OwnerComp.GetAIOwner();
	if (!ensure(AIOwner)) {return EBTNodeResult::Failed;}
	auto AIPawn = AIOwner->GetPawn();
	if (!ensure(AIPawn)) { return EBTNodeResult::Failed; }
	auto PatrolRouteComponent = AIPawn->FindComponentByClass<UPatrolRoute>();
	if (!ensure(PatrolRouteComponent)) { return EBTNodeResult::Failed; }

	// Get patrol points from the patrol route component
	auto PatrolPoints = PatrolRouteComponent->GetPatrolPoints();
	int32 NumPatrolPoints = PatrolPoints.Num();
	if (NumPatrolPoints <= 0) { 
		UE_LOG(LogTemp, Warning, TEXT("%s is missing patrol routes."), *AIPawn->GetName()); 
		return EBTNodeResult::Failed; 
	}

	auto Index = BlackboardComponent->GetValueAsInt(IndexKey.SelectedKeyName);
	
	// Calculate the next index position (don't choose value over length of array)
	auto NextIndex = (Index + 1) % NumPatrolPoints;

	// Set the current waypoint and next waypoint's index.
	BlackboardComponent->SetValueAsObject(WaypointKey.SelectedKeyName, PatrolPoints[Index]);
	BlackboardComponent->SetValueAsInt(IndexKey.SelectedKeyName, NextIndex);

	return EBTNodeResult::Succeeded;
}
