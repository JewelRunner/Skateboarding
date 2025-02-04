// Copyright Epic Games, Inc. All Rights Reserved.

#include "SkateboardingCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Actor/Skateboard/SkateboardBase.h"
#include "UI/HUDBase.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

ASkateboardingCharacter::ASkateboardingCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationYaw = false;
	
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	
	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;
	
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; 
	CameraBoom->bUsePawnControlRotation = true;
	
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;
}

void ASkateboardingCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	
	UE_LOG(LogTemp, Warning, TEXT("PossessedBy is called"))
	
	APlayerController* PlayerController = Cast<APlayerController>(NewController);

	if (PlayerController->GetHUD())
	{
		UE_LOG(LogTemp, Warning, TEXT("HUD is valid"))
	}
	
	if (PlayerController)
	{		
		if (AHUDBase* HUDBase = Cast<AHUDBase>(PlayerController->GetHUD()))
		{
			UE_LOG(LogTemp, Warning, TEXT("Before InitOverlay"))
			HUDBase->InitOverlay();
		}
	}
}

void ASkateboardingCharacter::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();
	
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void ASkateboardingCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
		
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ASkateboardingCharacter::Move);
		EnhancedInputComponent->BindAction(TurnAction, ETriggerEvent::Triggered, this, &ASkateboardingCharacter::Turn);
		
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ASkateboardingCharacter::Look);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void ASkateboardingCharacter::Move(const FInputActionValue& Value)
{
	MovingInputValue = Value.GetMagnitude();
	MovingInput += MovingInputValue*5.f;
	MovingInput = FMath::Clamp(MovingInput, 1.0f, 100.0f);
	
	const float InputValue = Value.GetMagnitude();
	const float DeltaTime = GetWorld()->GetDeltaSeconds();
	
	FVector ForwardDir = GetActorForwardVector();
	
	FVector NewVelocity = CurrentVelocity + (ForwardDir * InputValue * AccelerationRate * DeltaTime);
	
	if (NewVelocity.Size() > MaxSpeed)
	{
		NewVelocity = NewVelocity.GetSafeNormal() * MaxSpeed;
	}
    
	CurrentVelocity = NewVelocity;
}

void ASkateboardingCharacter::Turn(const FInputActionValue& Value)
{
	constexpr float Deadzone = 0.1f;
	
	const float TurnInput = Value.GetMagnitude();
	const float DeltaTime = GetWorld()->GetDeltaSeconds();
	
	const float DesiredTurnRate = (FMath::Abs(TurnInput) < Deadzone) ? 0.f : TurnInput * MaxTurnRate;
	
	if (CurrentTurnRate * DesiredTurnRate < 0.f)
	{
		CurrentTurnRate = 0.f;
	}
	
	CurrentTurnRate = FMath::FInterpTo(CurrentTurnRate, DesiredTurnRate, DeltaTime, TurnInterpSpeed);
	
	if (!FMath::IsNearlyZero(CurrentTurnRate))
	{
		AddActorLocalRotation(FRotator(0.f, CurrentTurnRate * DeltaTime, 0.f));
	}
}

void ASkateboardingCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D LookInput = Value.Get<FVector2D>();
	
	AddControllerYawInput(LookInput.X);
	AddControllerPitchInput(LookInput.Y);
}

void ASkateboardingCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	MovingInput = FMath::FInterpTo(MovingInput, 50.0f, DeltaTime, 10.f);
	
	UCharacterMovementComponent* MoveComp = GetCharacterMovement();
    if (MoveComp && MoveComp->IsMovingOnGround())
    {
        const FVector ActorLocation = GetActorLocation();
        const FVector TraceStart = ActorLocation;
        const FVector TraceEnd = ActorLocation - FVector(0.f, 0.f, 150.f);
        FHitResult Hit;
    	
        if (GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, ECC_Visibility))
        {
            const FVector FloorNormal = Hit.ImpactNormal;
            
            if (FVector::DotProduct(FloorNormal, FVector::UpVector) < 0.99f)
            {
                const float SlopeAngle = FMath::Acos(FloorNormal.Z);
                const float Gravity = 980.f;
                
                const float EffectiveAccel = Gravity * FMath::Sin(SlopeAngle);
            	
                const FVector Downward(0.f, 0.f, -1.f);
                
                FVector DownhillDir = FVector::VectorPlaneProject(Downward, FloorNormal);
                DownhillDir.Normalize();
            	
                const float Alignment = FMath::Abs(FVector::DotProduct(GetActorForwardVector(), DownhillDir));
                const float DirectionMultiplier = FMath::Lerp(0.001f, 1.0f, Alignment);
            	
                CurrentVelocity += DownhillDir * EffectiveAccel * DeltaTime * DirectionMultiplier;
            }
        }
    }
	
    if (!CurrentVelocity.IsNearlyZero())
    {
        CurrentVelocity = FMath::VInterpTo(CurrentVelocity, FVector::ZeroVector, DeltaTime, FrictionCoefficient);
        AddActorWorldOffset(CurrentVelocity * DeltaTime, true);
    }
}

void ASkateboardingCharacter::SetSkateboard(AActor* NewSkateboard)
{
	Skateboard = NewSkateboard;
	InitNewSkateboard();
}

void ASkateboardingCharacter::AddScore(int32 NewScore)
{
	CurrentScore += NewScore;
	OnScoreChanged.Broadcast(CurrentScore);
}

void ASkateboardingCharacter::InitNewSkateboard()
{
	Skateboard->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("LeftFootSocket"));
}
