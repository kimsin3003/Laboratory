// Fill out your copyright notice in the Description page of Project Settings.

#include "Grabber.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("Grabber reporting for duty"));
	m_physicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if(!m_physicsHandle)
		UE_LOG(LogTemp, Error, TEXT("no physics handle"));

	m_inputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (m_inputComponent)
	{
		m_inputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
	}
	else
		UE_LOG(LogTemp, Error, TEXT("no input component"));
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	FVector playerViewPointLocation;
	FRotator playerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		playerViewPointLocation,
		playerViewPointRotation
	);

	FVector lineTraceEnd = playerViewPointLocation + playerViewPointRotation.Vector() * m_reach;
	DrawDebugLine(GetWorld(), playerViewPointLocation, lineTraceEnd, FColor(255, 0, 0), false, 0, 0, 1.0f);

	FCollisionQueryParams traceParameter(FName(TEXT("")), false, GetOwner());

	FHitResult hit;
	if (GetWorld()->LineTraceSingleByObjectType(
		hit,
		playerViewPointLocation,
		lineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		traceParameter
	))
	{
		AActor* hitActor = hit.GetActor();
		if(hitActor)
			UE_LOG(LogTemp, Warning, TEXT("Line trace hit: %s"), hitActor, *(hitActor->GetName()));
	}

}

void UGrabber::Grab()
{
	UE_LOG(LogTemp, Log, TEXT("Grab Pressed"));
}

