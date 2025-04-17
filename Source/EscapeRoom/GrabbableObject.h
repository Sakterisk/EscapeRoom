// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractableObject.h"
#include "GrabbableObject.generated.h"

UCLASS()
class ESCAPEROOM_API AGrabbableObject : public AInteractableObject
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGrabbableObject();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void OnGrab();
	virtual void OnRelease();
	virtual FString OnHover(UPrimitiveComponent* TouchedComponent) override;
	virtual bool IsGrabbable() const { return true; }
	virtual bool IsHeavy() const { return bIsHeavy; }
	virtual bool CustomDistanceToPlayer() const { return DistanceToPlayer > 0.f; }
	virtual float GetDistanceFromPlayer() const { return DistanceToPlayer; }

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	bool bIsHeavy = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	bool bShouldIgnorePlayer = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	FRotator RotationOnGrab = FRotator(0.f, 0.f, 0.f);
	
	UPROPERTY(EditAnywhere, Category = "Settings")
	bool bDefaultRotationOnGrab = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	float DistanceToPlayer = 0.f;

};
