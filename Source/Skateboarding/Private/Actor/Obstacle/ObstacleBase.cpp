// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Obstacle/ObstacleBase.h"

#include "Components/BoxComponent.h"

AObstacleBase::AObstacleBase()
{
	ObstacleMesh = CreateDefaultSubobject<UStaticMeshComponent>("ObstacleMesh");
	ObstacleMesh->SetupAttachment(GetRootComponent());

	BoxComp->SetLineThickness(5.f);
	BoxComp->SetBoxExtent(FVector(30.f, 30.f, 200.f));
}

void AObstacleBase::Interact(ACharacter* InteractingCharacter)
{
	UE_LOG(LogTemp, Warning, TEXT("Interact with obstacle"))
	
}
