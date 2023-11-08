// Fill out your copyright notice in the Description page of Project Settings.


#include "Spawner.h"
#include "Enemy.h"

// Sets default values
ASpawner::ASpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void ASpawner::SpawnEnemy()
{
	if (EnemyClass)
	{

		FVector SpawnLocation = GetActorLocation() + FVector(FMath::RandRange(-1000, 1000), FMath::RandRange(-1000, 1000), 0);
		FRotator SpawnRotation = GetActorRotation() + FRotator(0, FMath::RandRange(0, 360), 0);
		SpawnLocation.Z = GetActorLocation().Z;
		ACharacter* NewEnemy = GetWorld()->SpawnActor<ACharacter>(EnemyClass, SpawnLocation, SpawnRotation);
		
	}

}

// Called when the game starts or when spawned
void ASpawner::BeginPlay()
{
	Super::BeginPlay();
	FTimerHandle SpawnTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle, this, &ASpawner::SpawnEnemy, SpawnInterval, true, SpawnInterval);
	
}

// Called every frame
void ASpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

