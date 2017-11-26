// Fill out your copyright notice in the Description page of Project Settings.

#include "ChooseNextWaypoint.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "PatrollingGuard.h"
#include "AIController.h"

EBTNodeResult::Type UChooseNextWaypoint::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	UE_LOG(LogTemp, Warning, TEXT("Executing"));
	auto BlackboardComponent = OwnerComp.GetBlackboardComponent();

	// Get the Controlling Patrolling Guard (this is bad architecture, need to de-couple)

	auto AIOwner = OwnerComp.GetAIOwner();
	if (!AIOwner) {
		UE_LOG(LogTemp, Warning, TEXT("AI Owner"));
		return EBTNodeResult::Failed;
	}
	auto AIPawn = Cast<APatrollingGuard>(AIOwner->GetPawn());
	if (!AIPawn) {
		UE_LOG(LogTemp, Warning, TEXT("AI Pawn"));
		return EBTNodeResult::Failed;
	}
	auto PatrolPoints = AIPawn->PatrolPointsCPP;

	auto Index = BlackboardComponent->GetValueAsInt(IndexKey.SelectedKeyName);
	
	// Calculate the next index position (don't choose value over length of array)
	auto NextIndex = (Index + 1) % PatrolPoints.Num();

	// Set the current waypoint and next waypoint's index.
	BlackboardComponent->SetValueAsObject(WaypointKey.SelectedKeyName, PatrolPoints[Index]);
	BlackboardComponent->SetValueAsInt(IndexKey.SelectedKeyName, NextIndex);
	
	UE_LOG(LogTemp, Warning, TEXT("Waypoint Key: %s"), *WaypointKey.SelectedKeyName.ToString());
	UE_LOG(LogTemp, Warning, TEXT("Index Key: %s"), *IndexKey.SelectedKeyName.ToString());

	return EBTNodeResult::Succeeded;
}
