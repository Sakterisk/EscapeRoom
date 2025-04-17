// Fill out your copyright notice in the Description page of Project Settings.


#include "Lock.h"

ALock::ALock()
{
 	// Set this actor to call Tick() every frame.
    PrimaryActorTick.bCanEverTick = true;

    MeshComponent->SetSimulatePhysics(false);
    bIsGrabbable = false;

    CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
    CameraComponent->SetupAttachment(MeshComponent);
    CameraComponent->SetRelativeLocation(FVector(0, 2, 25));
    CameraComponent->SetRelativeRotation(FRotator(-90, 90, -180));

}

// Called when the game starts or when spawned
void ALock::BeginPlay()
{
    Super::BeginPlay();
    
}

// Called every frame
void ALock::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

}
