// Fill out your copyright notice in the Description page of Project Settings.


#include "GrabbableObject.h"

// Sets default values
AGrabbableObject::AGrabbableObject()
{
	PrimaryActorTick.bCanEverTick = true;
	MeshComponent->SetSimulatePhysics(true);
}

// Called when the game starts or when spawned
void AGrabbableObject::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGrabbableObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGrabbableObject::OnGrab()
{
	Tags.Add("Grabbed");
	MeshComponent->WakeAllRigidBodies();
	MeshComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);

	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
    if (!PlayerController) return;
    FVector CameraLocation;
    FRotator CameraRotation;
    PlayerController->GetPlayerViewPoint(CameraLocation, CameraRotation);

	if (!bDefaultRotationOnGrab)
	{	
		FVector DirectionToCamera = CameraRotation.Quaternion().GetForwardVector() * -1.f;
		FRotator FinalRotation = FRotationMatrix::MakeFromX(DirectionToCamera).Rotator();
		FinalRotation += RotationOnGrab;
		SetActorRotation(FinalRotation);
	}
}

void AGrabbableObject::OnRelease()
{
	Tags.Remove("Grabbed");
	if (bShouldIgnorePlayer) return;
	MeshComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);
}

FString AGrabbableObject::OnHover(UPrimitiveComponent* TouchedComponent)
{
	return TEXT("HoverGrab");
}