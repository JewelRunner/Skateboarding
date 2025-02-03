// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interaction/InteractionInterface.h"
#include "InteractableActor.generated.h"

class UBoxComponent;

UCLASS()
class SKATEBOARDING_API AInteractableActor : public AActor, public IInteractionInterface
{
	GENERATED_BODY()
	
public:	
	AInteractableActor();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UBoxComponent* BoxComp;
protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);

	UFUNCTION()
	virtual void OnBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
public:	
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void Interact(ACharacter* InteractingCharacter) override;
};
