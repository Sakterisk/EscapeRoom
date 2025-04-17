// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractableObject.generated.h"

UCLASS(Abstract)
class ESCAPEROOM_API AInteractableObject : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInteractableObject();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual FString OnHover(UPrimitiveComponent* TouchedComponent);
	virtual bool IsNewComponent(UPrimitiveComponent* TouchedComponent);
	UFUNCTION(BlueprintCallable, Category = "Interactable")
	virtual UPrimitiveComponent* GetMesh() const { return MeshComponent; }

protected:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
	UStaticMeshComponent* MeshComponent;

};
