// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/TriggerVolume.h"
#include "DoorOpener.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LABORATORY_API UDoorOpener : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDoorOpener();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	void OpenDoor();
	void CloseDoor();
	UPROPERTY(EditAnywhere)
	ATriggerVolume* m_triggerVolume = nullptr;

	UPROPERTY(EditAnywhere)
	float m_closeDelay = 0;
	UPROPERTY(EditAnywhere)
	FRotator m_openAngle;
	UPROPERTY(EditAnywhere)
	FRotator m_closeAngle;

	AActor* m_actorThatOpens = nullptr;
	float m_lastOpenTime = 0;
	
};
