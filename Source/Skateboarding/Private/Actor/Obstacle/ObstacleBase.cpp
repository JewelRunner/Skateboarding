// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Obstacle/ObstacleBase.h"

#include "Components/BoxComponent.h"
#include "GameFramework/Character.h"

AObstacleBase::AObstacleBase()
{
	ObstacleMesh = CreateDefaultSubobject<UStaticMeshComponent>("ObstacleMesh");
	ObstacleMesh->SetupAttachment(GetRootComponent());

	BoxComp->SetLineThickness(5.f);
	BoxComp->SetBoxExtent(FVector(30.f, 30.f, 200.f));
}

void AObstacleBase::Interact(ACharacter* InteractingCharacter)
{
	if (InteractingCharacter->GetClass()->ImplementsInterface(UInteractionInterface::StaticClass()))
	{
		IInteractionInterface* InteractionInterface = Cast<IInteractionInterface>(InteractingCharacter);
		InteractionInterface->AddScore(Points);
	}
}
