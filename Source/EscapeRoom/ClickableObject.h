// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableObject.h"
#include "ClickableObject.generated.h"

/**
 * 
 */
UCLASS()
class ESCAPEROOM_API AClickableObject : public AInteractableObject
{
	GENERATED_BODY()
	
public:
	AClickableObject();

protected:
	virtual void BeginPlay() override;
	
public:
	virtual void Tick(float DeltaTime) override;
	void OnClick(UPrimitiveComponent* TouchedComponent);
	virtual FString OnHover(UPrimitiveComponent* TouchedComponent) override;
	virtual bool IsNewComponent(UPrimitiveComponent* TouchedComponent) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	TArray<UStaticMeshComponent*> ClickableMeshes;

	UFUNCTION(BlueprintImplementableEvent, Category = "Clickable Object")
	void OnClickEvent(UStaticMeshComponent* TouchedComponent);

private:
	UPrimitiveComponent* LastTouchedComponent = nullptr;

};
