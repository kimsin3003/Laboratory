// Fill out your copyright notice in the Description page of Project Settings.

#include "DoorOpener.h"
#include "Engine/World.h"
#include "Runtime/Engine/Classes/GameFramework/PlayerController.h"

// Sets default values for this component's properties
UDoorOpener::UDoorOpener()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UDoorOpener::BeginPlay()
{
	Super::BeginPlay();

	m_actorThatOpens = GetWorld()->GetFirstPlayerController()->GetPawn();
	

}


void UDoorOpener::OpenDoor()
{
	AActor* Owner = GetOwner();
	Owner->SetActorRotation(m_openAngle);
}

void UDoorOpener::CloseDoor()
{
	AActor* Owner = GetOwner();
	Owner->SetActorRotation(m_closeAngle);
}
// Called every frame
void UDoorOpener::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (m_triggerVolume->IsOverlappingActor(m_actorThatOpens))
	{
		OpenDoor();
		m_lastOpenTime = GetWorld()->GetTimeSeconds();
	}

	if (GetWorld()->GetTimeSeconds() - m_lastOpenTime > m_closeDelay)
	{
		CloseDoor();
	}
}

