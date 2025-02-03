// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/InteractableActor.h"
#include "SkateboardBase.generated.h"

/**
 * 
 */
UCLASS()
class SKATEBOARDING_API ASkateboardBase : public AInteractableActor
{
	GENERATED_BODY()

	ASkateboardBase();
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skateboard")
	UStaticMeshComponent* SkateboardMesh;

	virtual void Interact(ACharacter* InteractingCharacter) override;
};
