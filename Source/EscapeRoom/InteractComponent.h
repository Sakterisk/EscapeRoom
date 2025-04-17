#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "GameFramework/PlayerController.h"
#include "GrabbableObject.h"
#include "InspectableObject.h"
#include "InteractableObject.h"
#include "ClickableObject.h"
#include "EscapeRoomCharacter.h"
#include "InteractComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ESCAPEROOM_API UInteractComponent : public USceneComponent
{
    GENERATED_BODY()

public:
    UInteractComponent();
    
protected:
    virtual void BeginPlay() override;

public:
	//virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
    void Press();
    void Release();
    void Inspect();
	void SetPhysicsHandle(UPhysicsHandleComponent* InPhysicsHandle) { PhysicsHandle = InPhysicsHandle; }

    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
    UPROPERTY()
    UPhysicsHandleComponent* PhysicsHandle;

    UPROPERTY()
    APlayerController* PlayerController;

    UPROPERTY(EditAnywhere, Category = "Sweep Settings")
    float ReachDistance = 250.f; // How far the player can reach

    UPROPERTY(EditAnywhere, Category = "Sweep Settings")
    float SphereRadius = 15.f; // Radius of the sphere used for the sweep

    UPROPERTY(EditAnywhere, Category = "Drag Settings")
    float LerpSpeed = 10.f; // How fast the grabbed object moves to the target location

    float DistanceFromPlayer = 0.f;
    float DistanceFromGround = 0.f;
    AGrabbableObject* GrabbedActor;
    AInspectableObject* InspectedActor;
    AInteractableObject* HoveredActor;
    FHitResult HitResult;

    bool GetFirstPhysicsBodyInReach(FHitResult& OutHitResult);
    bool DraggedInInspect();
    bool Dragged();
    void Hover();
    void ExitInspect();
    void PressedInInspect();
    void ReleasedInInspect();

};
