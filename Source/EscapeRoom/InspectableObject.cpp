// Fill out your copyright notice in the Description page of Project Settings.


#include "InspectableObject.h"

// Sets default values
AInspectableObject::AInspectableObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AInspectableObject::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AInspectableObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AInspectableObject::OnInspect()
{
	Tags.Add("Inspected");
	if (CameraComponent) return;
	OriginalTransform = GetActorTransform();
	SetActorScale3D(FVector(ScaleWhenInspected));
	MeshComponent->SetSimulatePhysics(false);

	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
    if (!PlayerController) return;
    FVector CameraLocation;
    FRotator CameraRotation;
    PlayerController->GetPlayerViewPoint(CameraLocation, CameraRotation);
    CameraUpVector = CameraRotation.Quaternion().GetUpVector();
	CameraRightVector = CameraRotation.Quaternion().GetRightVector();

	if (!bDefaultRotationOnInspect)
	{	
		FVector DirectionToCamera = CameraRotation.Quaternion().GetForwardVector() * -1.f;
		FRotator FinalRotation = FRotationMatrix::MakeFromX(DirectionToCamera).Rotator();
		FinalRotation += RotationOnInspect;
		SetActorRotation(FinalRotation);
	}
}

void AInspectableObject::OnExitInspect()
{
	Tags.Remove("Inspected");
	if (CameraComponent) return;
	SetActorTransform(OriginalTransform, false, nullptr, ETeleportType::TeleportPhysics);
	MeshComponent->SetSimulatePhysics(true);
}

void AInspectableObject::OnPressInInspect(FVector2D MousePosition)
{
	if (CameraComponent) return;
	InitialMousePosition = MousePosition;
	bIsRotating = true;
}

void AInspectableObject::OnReleaseInInspect(FVector2D MousePosition)
{
	if (CameraComponent) return;
	bIsRotating = false;
}

void AInspectableObject::OnDragInInspect(FVector2D MousePosition)
{
	if (CameraComponent) return;
	if (!bIsRotating) return;

	FVector2D Delta = MousePosition - InitialMousePosition;
	FQuat CurrentRotation = GetActorQuat();
	FQuat YawRotation = FQuat(CameraUpVector, FMath::DegreesToRadians(Delta.X * -RotationSpeed));
	FQuat PitchRotation = FQuat(CameraRightVector, FMath::DegreesToRadians(Delta.Y * -RotationSpeed));

	AddActorWorldRotation(PitchRotation);
	AddActorWorldRotation(YawRotation);

	InitialMousePosition = MousePosition;
}

FString AInspectableObject::OnHover(UPrimitiveComponent* TouchedComponent)
{
	if (CameraComponent) return "Inspect";
	return Super::OnHover(TouchedComponent);
}
