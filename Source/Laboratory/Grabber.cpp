// Fill out your copyright notice in the Description page of Project Settings.

#include "Grabber.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "Runtime/Engine/Classes/PhysicsEngine/PhysicsHandleComponent.h"

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
	FindPhysicsHandleComponent();
	SetupInputComponent();
}

void UGrabber::SetupInputComponent()
{
	m_inputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (m_inputComponent)
	{
		m_inputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		m_inputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	else
		UE_LOG(LogTemp, Error, TEXT("no input component"));
}

void UGrabber::FindPhysicsHandleComponent()
{
	m_physicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (m_physicsHandle == nullptr)
		UE_LOG(LogTemp, Error, TEXT("no physics handle"));
}

void UGrabber::Grab()
{
	UE_LOG(LogTemp, Log, TEXT("Grab Pressed"));

	FHitResult hitResult = GetFirstPhysicsBodyInReach();
	auto componentToGrab = hitResult.GetComponent();
	auto actorHit = hitResult.GetActor();
	AActor* hitActor = hitResult.GetActor();
	if (hitActor)
	{
		m_physicsHandle->GrabComponent(componentToGrab, NAME_None, actorHit->GetActorLocation(), true);
	}
}

void UGrabber::Release()
{
	UE_LOG(LogTemp, Log, TEXT("Release Pressed"));
	if (m_physicsHandle->GrabbedComponent)
	{
		m_physicsHandle->ReleaseComponent();
	}
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	auto lineTraceEnd = GetReachLineEnd();
	if (m_physicsHandle->GrabbedComponent)
	{
		m_physicsHandle->SetTargetLocation(lineTraceEnd);
	}

}

FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{
	FVector playerViewPointLocation = GetReachLineStart();
	FVector lineTraceEnd = GetReachLineEnd();

	FCollisionQueryParams traceParameter(FName(TEXT("")), false, GetOwner());

	FHitResult hit;
	GetWorld()->LineTraceSingleByObjectType(
		hit,
		playerViewPointLocation,
		lineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		traceParameter
	);
	return hit;
}

FVector UGrabber::GetReachLineStart()
{
	FVector playerViewPointLocation;
	FRotator playerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		playerViewPointLocation,
		playerViewPointRotation
	);

	return playerViewPointLocation;
}

FVector UGrabber::GetReachLineEnd()
{
	FVector playerViewPointLocation;
	FRotator playerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		playerViewPointLocation,
		playerViewPointRotation
	);

	return playerViewPointLocation + playerViewPointRotation.Vector() * m_reach;
}
