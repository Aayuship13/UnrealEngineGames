// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "FPSGameMode.generated.h"

/**
 * 
 */
UCLASS()
class FPSGAME_API AFPSGameMode : public AGameMode
{
	GENERATED_BODY()
	
public:
	void RestartGameplay(bool Won);

private:
	void ResetLevel();

public:

	UPROPERTY(BlueprintReadOnly)
		int TimerCount = 305;

private:

	FTimerHandle CountdownTimerHandle = FTimerHandle();

	void CountdownTimer();

public:

	void BeginPlay() override;

};
