// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUDBase.h"
#include "UI/UserWidgetBase.h"


AHUDBase::AHUDBase()
{
	
}

void AHUDBase::InitOverlay()
{
	if (OverlayWidgetClass)
	{
		UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), OverlayWidgetClass);
		OverlayWidget = Cast<UUserWidgetBase>(Widget);
		OverlayWidget->AddToViewport();
		UE_LOG(LogTemp, Warning, TEXT("InitOverlay()"))
	}
}
