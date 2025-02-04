// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/InteractableActor.h"

#include "Components/BoxComponent.h"
#include "Skateboarding/SkateboardingCharacter.h"

// Sets default values
AInteractableActor::AInteractableActor()
{
	PrimaryActorTick.bCanEverTick = true;

	PrimaryActorTick.bCanEverTick = true;
	
	BoxComp = CreateDefaultSubobject<UBoxComponent>("InteractableSphere");
		
	BoxComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	BoxComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	BoxComp->SetGenerateOverlapEvents(true);
	
	SetRootComponent(BoxComp);
}

void AInteractableActor::BeginPlay()
{
	Super::BeginPlay();

	BoxComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	BoxComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	BoxComp->OnComponentBeginOverlap.AddDynamic(this, &AInteractableActor::OnBoxBeginOverlap);
	BoxComp->OnComponentEndOverlap.AddDynamic(this, &AInteractableActor::OnBoxEndOverlap);


}

void AInteractableActor::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("OnBoxBeginOverlap()"))
	
	if (ASkateboardingCharacter* SkateboardingCharacter = Cast<ASkateboardingCharacter>(OtherActor))
	{
		Interact(SkateboardingCharacter);
	}
	
}

void AInteractableActor::OnBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	
}

void AInteractableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AInteractableActor::Interact(ACharacter* InteractingCharacter)
{
	
}

