// Fill out your copyright notice in the Description page of Project Settings.


#include "ClickableObject.h"

AClickableObject::AClickableObject()
{
    MeshComponent->SetCollisionProfileName(TEXT("Interactable"));
}

void AClickableObject::BeginPlay()
{
    Super::BeginPlay();
}

void AClickableObject::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AClickableObject::OnClick(UPrimitiveComponent* TouchedComponent)
{
    UStaticMeshComponent* Mesh = Cast<UStaticMeshComponent>(TouchedComponent);
    if (!Mesh) return;
    if (ClickableMeshes.Find(Mesh) == -1) return;
    if (ActorHasTag("Moving")) return;
    if (Mesh->ComponentHasTag("Locked")) return;
    OnClickEvent(Mesh);
}

FString AClickableObject::OnHover(UPrimitiveComponent* TouchedComponent)
{
    int32 MeshIndex = ClickableMeshes.Find(Cast<UStaticMeshComponent>(TouchedComponent));
    if (MeshIndex != -1)
    {
        return TEXT("Click");
    }
    return TEXT("Default");
}

bool AClickableObject::IsNewComponent(UPrimitiveComponent* TouchedComponent)
{
    if (LastTouchedComponent == TouchedComponent) return false;
    LastTouchedComponent = TouchedComponent;
    return true;
}


