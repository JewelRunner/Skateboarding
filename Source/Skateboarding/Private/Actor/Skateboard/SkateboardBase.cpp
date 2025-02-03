// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Skateboard/SkateboardBase.h"

#include "GameFramework/Character.h"

ASkateboardBase::ASkateboardBase()
{
	SkateboardMesh = CreateDefaultSubobject<UStaticMeshComponent>("SkateboardMesh");
	static ConstructorHelpers::FObjectFinder<UStaticMesh>MeshAsset(TEXT("/Game/Assets/Skateboard/Skateboard.Skateboard"));
	UStaticMesh* Asset = MeshAsset.Object;

	SkateboardMesh->SetStaticMesh(Asset);
	SkateboardMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SkateboardMesh->SetupAttachment(GetRootComponent());
}

void ASkateboardBase::Interact(ACharacter* InteractingCharacter)
{
	if (InteractingCharacter->GetClass()->ImplementsInterface(UInteractionInterface::StaticClass()))
	{
		IInteractionInterface* InteractionInterface = Cast<IInteractionInterface>(InteractingCharacter);
		InteractionInterface->SetSkateboard(this);
	}
}


