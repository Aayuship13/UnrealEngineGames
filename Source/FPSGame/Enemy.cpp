// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "Components/BoxComponent.h"
#include "PlayerCharacter.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Animation/AnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Spawner.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DamageCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Damage Collision"));
	DamageCollision->SetupAttachment(RootComponent);

	AIPerComp = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AI PerceptionComponent"));
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));

	SightConfig->SightRadius = 2000.0f;
	SightConfig->LoseSightRadius = 2250.0f;
	SightConfig->PeripheralVisionAngleDegrees = 110.0f;
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	SightConfig->SetMaxAge(0.2f);

	AIPerComp->ConfigureSense(*SightConfig);
	AIPerComp->SetDominantSense(SightConfig->GetSenseImplementation());
	AIPerComp->OnPerceptionUpdated.AddDynamic(this, &AEnemy::OnSensed);

	CurrentVelocity = FVector::ZeroVector;
	MovementSpeed = 375.0f;

}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	DamageCollision->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::OnHit);
	
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!CurrentVelocity.IsZero())
	{
		NewLocation = GetActorLocation() + CurrentVelocity * DeltaTime;
		
			SetActorLocation(NewLocation);
			
	}

}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemy::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit)
{
	APlayerCharacter* Char = Cast<APlayerCharacter>(OtherActor);
	{
		bFromSweep = false;
		if (Char)
		{
			isAttacking = true;
			Char->DealDamage(DamageValue);
			
		}
	}
}

void AEnemy::OnSensed(const TArray<AActor*>& UpdatedActors)
{
	for (int i = 0; i < UpdatedActors.Num();i++ )
	{
		FActorPerceptionBlueprintInfo Info;
		AIPerComp->GetActorsPerception(UpdatedActors[i], Info);
		if (Info.LastSensedStimuli[0].WasSuccessfullySensed())
		{
			if (UpdatedActors[i]->ActorHasTag("Player"))
			{
				
				FVector PlayerLocation = UpdatedActors[i]->GetActorLocation();
				FVector EnemyLocation = GetActorLocation();
				
				FVector dir = PlayerLocation - EnemyLocation;
				//dir.Z = 0.0f;
				CurrentVelocity = dir.GetSafeNormal() * MovementSpeed;
				SetNewRotation(PlayerLocation, EnemyLocation);

				Distance = FVector::Distance(PlayerLocation, EnemyLocation);
				if (Distance <= StoppingDistance)
				{
					CurrentVelocity = FVector::ZeroVector;
					isAttacking = false;
					GetCharacterMovement()->StopMovementImmediately();

				}

			}
		}
		else
		{
			CurrentVelocity = FVector::ZeroVector;
		}
		
	}
}


void AEnemy::SetNewRotation(FVector TargetPosition, FVector CurrentPosition)
{
	FVector NewDirection = TargetPosition - CurrentPosition;
	NewDirection.Z = 0.0f;
	EnemyRotation = NewDirection.Rotation();
	SetActorRotation(EnemyRotation);

}

void AEnemy::DealDamage(float DamageAmount)
{
	
	Health -= DamageAmount;
	if (Health <= 0)
	{
		Score = Score + 1;
		CurrentVelocity = FVector::ZeroVector;
	}
}

