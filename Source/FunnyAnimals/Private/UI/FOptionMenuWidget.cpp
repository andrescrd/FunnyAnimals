// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/FOptionMenuWidget.h"

UFOptionMenuWidget::UFOptionMenuWidget(const FObjectInitializer& ObjectInitializer) : UUserWidget(ObjectInitializer)
{
	WelcomeMessageVis = ESlateVisibility::Hidden;
}
