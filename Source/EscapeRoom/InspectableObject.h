// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Camera/CameraComponent.h"
#include "GrabbableObject.h"
#include "InspectableObject.generated.h"

UCLASS()
class ESCAPEROOM_API AInspectableObject : public AGrabbableObject
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInspectableObject();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void OnInspect();
	virtual void OnExitInspect();
	virtual void OnPressInInspect(FVector2D MousePosition);
	virtual void OnReleaseInInspect(FVector2D MousePosition);
	virtual void OnDragInInspect(FVector2D MousePosition);
	virtual FString OnHover(UPrimitiveComponent* TouchedComponent) override;
	virtual bool IsGrabbable() const override { return bIsGrabbable; }
	float GetDistanceFromPlayer() const { return DistanceFromPlayer; }
	UFUNCTION(BlueprintCallable)
	void SetCameraComponent(UCameraComponent* NewCameraComponent) { CameraComponent = NewCameraComponent; }
	UCameraComponent* GetCameraComponent() const { return CameraComponent; }

private:

	UPROPERTY(EditAnywhere, Category = "Settings")
	float DistanceFromPlayer = 100.f;

	UPROPERTY(EditAnywhere, Category = "Settings")
	float ScaleWhenInspected = 0.5f;

	UPROPERTY(EditAnywhere, Category = "Settings")
	float RotationSpeed = 0.2f;

	UPROPERTY(EditAnywhere, Category = "Settings")
	FRotator RotationOnInspect = FRotator(0.f, 0.f, 0.f);

	UPROPERTY(EditAnywhere, Category = "Settings")
	bool bDefaultRotationOnInspect = true;

	bool bIsRotating = false;
	FTransform OriginalTransform;
	FVector2D InitialMousePosition;
	FVector CameraUpVector;
	FVector CameraRightVector;

protected:

	UPROPERTY(EditAnywhere, Category = "Settings")
	UCameraComponent* CameraComponent = nullptr;

	UPROPERTY(EditAnywhere, Category = "Settings")
	bool bIsGrabbable = true;
};
