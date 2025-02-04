// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/InteractableActor.h"
#include "ObstacleBase.generated.h"

/**
 * 
 */
UCLASS()
class SKATEBOARDING_API AObstacleBase : public AInteractableActor
{
	GENERATED_BODY()
	AObstacleBase();
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Obstacle")
	UStaticMeshComponent* ObstacleMesh = nullptr;

	virtual void Interact(ACharacter* InteractingCharacter) override;
private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Obstacle", meta = (AllowPrivateAccess = "true"))
	int32 Points = 1;
};
