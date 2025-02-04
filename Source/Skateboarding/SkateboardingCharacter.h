// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interaction/InteractionInterface.h"
#include "Logging/LogMacros.h"
#include "SkateboardingCharacter.generated.h"

class ASkateboardBase;
class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class ASkateboardingCharacter : public ACharacter, public IInteractionInterface
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

public:
	ASkateboardingCharacter();

	virtual void PossessedBy(AController* NewController) override;

	UPROPERTY()
	AActor* Skateboard = nullptr;
	
	virtual void SetSkateboard(AActor* NewSkateboard) override;

	UPROPERTY(BlueprintReadOnly)
	float MovingInput = 50.0f;

	float MovingInputValue = 0.f;

protected:
	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* TurnAction;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* JumpAction;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	FVector CurrentVelocity = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float AccelerationRate = 600.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float MaxSpeed = 5000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float FrictionCoefficient = 0.8f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float MaxTurnRate = 90.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float TurnInterpSpeed = 2.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	float CurrentTurnRate = 0.0f;
	
	void Move(const FInputActionValue& Value);
	void Turn(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);


protected:
	virtual void Tick(float DeltaTime) override;
	
	virtual void NotifyControllerChanged() override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	FORCEINLINE USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	
	FORCEINLINE UCameraComponent* GetFollowCamera() const { return FollowCamera; }

private:
	void InitNewSkateboard();
};

