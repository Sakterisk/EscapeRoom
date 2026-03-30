#include "InteractComponent.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Camera/CameraComponent.h"

UInteractComponent::UInteractComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void UInteractComponent::BeginPlay()
{
    Super::BeginPlay();
	PlayerController = GetWorld()->GetFirstPlayerController();
	PhysicsHandle->LinearStiffness = 1000.0f;
	PhysicsHandle->LinearDamping = 400.0f;
	PhysicsHandle->AngularStiffness = 1000.0f;
	PhysicsHandle->AngularDamping = 400.0f;
}

void UInteractComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (Dragged()) return;

	if (DraggedInInspect()) return;
	
	Hover();
}

bool UInteractComponent::Dragged()
{
	if (!PhysicsHandle || !PhysicsHandle->GrabbedComponent) return false;

	FVector TargetLocation = GetComponentLocation() + GetForwardVector() * DistanceFromPlayer;
	TargetLocation.Z = (GrabbedActor->IsHeavy()) ? DistanceFromGround : TargetLocation.Z;
	PhysicsHandle->SetTargetLocationAndRotation(TargetLocation, GetComponentRotation());

	return true;
}

bool UInteractComponent::DraggedInInspect()
{
	if (!InspectedActor || !PlayerController) return false;

	FVector2D CurrentMousePosition;
	PlayerController->GetMousePosition(CurrentMousePosition.X, CurrentMousePosition.Y);
	InspectedActor->OnDragInInspect(CurrentMousePosition);

	return true;
}

void UInteractComponent::Hover()
{
	AEscapeRoomCharacter* PlayerCharacter = Cast<AEscapeRoomCharacter>(GetOwner());
	if (GetFirstPhysicsBodyInReach(HitResult))
	{    
		AInteractableObject* NewHoveredActor = Cast<AInteractableObject>(HitResult.GetActor());

		if (NewHoveredActor && (NewHoveredActor != HoveredActor || NewHoveredActor->IsNewComponent(HitResult.GetComponent())))
		{
			HoveredActor = NewHoveredActor;
			PlayerCharacter->ChangeCrosshair(HoveredActor->OnHover(HitResult.GetComponent()));
		}
	}
	else if (HoveredActor)
	{
		PlayerCharacter->ChangeCrosshair("Default");
		HoveredActor = nullptr;
	}
}

bool UInteractComponent::GetFirstPhysicsBodyInReach(FHitResult& OutHitResult)
{
    FVector EndLocation = GetComponentLocation() + GetForwardVector() * ReachDistance;

    return GetWorld()->SweepSingleByChannel(
		OutHitResult, 
		GetComponentLocation(),
		EndLocation, 
		FQuat::Identity, 
		ECollisionChannel::ECC_GameTraceChannel2,
		FCollisionShape::MakeSphere(SphereRadius)
	);
}

void UInteractComponent::Press()
{	
	if (InspectedActor)
	{
		PressedInInspect();
		return;
	}

	if (!PhysicsHandle) return;
	GrabbedActor = Cast<AGrabbableObject>(HitResult.GetActor());
	if (!GrabbedActor) return;

	if (!GrabbedActor->IsGrabbable())
	{
		GrabbedActor = nullptr;
		return;
	}
	
	GrabbedActor->OnGrab();

	Cast<AEscapeRoomCharacter>(GetOwner())->ChangeCrosshair("Grab");
	GrabbedActor->GetMesh()->SetSimulatePhysics(true);
	GrabbedActor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

		
	DistanceFromGround = HitResult.ImpactPoint.Z + .1f;
	
	if (GrabbedActor->CustomDistanceToPlayer()) DistanceFromPlayer = GrabbedActor->GetDistanceFromPlayer();
	else DistanceFromPlayer = FVector::Distance(HitResult.ImpactPoint, GetComponentLocation());

	float Mass = HitResult.GetComponent()->GetMass();
	float ScaledLerpSpeed = FMath::GetMappedRangeValueClamped(FVector2D(100.0f, 500.0f), FVector2D(15.0f, 0.75f), Mass);
	PhysicsHandle->SetInterpolationSpeed(ScaledLerpSpeed);

	PhysicsHandle->GrabComponentAtLocationWithRotation(
		HitResult.GetComponent(),
		NAME_None,
		HitResult.ImpactPoint,
		GetComponentRotation()
	);

}

void UInteractComponent::Release()
{	
	if (InspectedActor)
	{
		ReleasedInInspect();
		return;
	}

	if (PhysicsHandle && PhysicsHandle->GrabbedComponent)
	{
		GrabbedActor->OnRelease();
		PhysicsHandle->ReleaseComponent();
		Cast<AEscapeRoomCharacter>(GetOwner())->ChangeCrosshair("HoverGrab");
		GrabbedActor = nullptr;
		return;
	}

	AClickableObject* ClickedActor = Cast<AClickableObject>(HoveredActor);
	if (ClickedActor)
	{
		ClickedActor->OnClick(HitResult.GetComponent());
		return;
	}

	AInspectableObject* InspectableActor = Cast<AInspectableObject>(HoveredActor);
	if (InspectableActor && InspectableActor->GetCameraComponent())
	{
		Inspect();
		return;
	}

}

void UInteractComponent::Inspect()
{
	if (GrabbedActor) 
	{
		AInspectableObject* InspectableActor = Cast<AInspectableObject>(GrabbedActor);
		if (!InspectableActor) return;
		Release();
	}
	if (InspectedActor)
	{
		ExitInspect();
		return;
	}

	InspectedActor = Cast<AInspectableObject>(HoveredActor);
	if (!InspectedActor) return;

	InspectedActor->OnInspect();
	if (!InspectedActor->GetCameraComponent())
	{
		FVector TargetLocation = GetComponentLocation() + GetForwardVector() * InspectedActor->GetDistanceFromPlayer();
		InspectedActor->SetActorLocation(TargetLocation);
	}
	else
	{
		InspectedActor->GetCameraComponent()->SetActive(true);
		PlayerController->SetViewTargetWithBlend(InspectedActor, 0.5f, EViewTargetBlendFunction::VTBlend_Cubic, 0.5f, false);
	}

	Cast<AEscapeRoomCharacter>(GetOwner())->SwitchInspecting(true);

}

void UInteractComponent::ExitInspect()
{
	InspectedActor->OnExitInspect();

	if (InspectedActor->GetCameraComponent())
	{
		InspectedActor->GetCameraComponent()->SetActive(false);
		PlayerController->SetViewTargetWithBlend(GetOwner(), 0.5f, EViewTargetBlendFunction::VTBlend_Cubic, 0.5f, true);
	}

	Cast<AEscapeRoomCharacter>(GetOwner())->SwitchInspecting(false);

	InspectedActor = nullptr;
}

void UInteractComponent::PressedInInspect()
{
	if (!PlayerController) return;
	FVector2D CurrentMousePosition;
	PlayerController->GetMousePosition(CurrentMousePosition.X, CurrentMousePosition.Y);
	InspectedActor->OnPressInInspect(CurrentMousePosition);
}

void UInteractComponent::ReleasedInInspect()
{
	if (!PlayerController) return;
	FVector2D CurrentMousePosition;
	PlayerController->GetMousePosition(CurrentMousePosition.X, CurrentMousePosition.Y);
	InspectedActor->OnReleaseInInspect(CurrentMousePosition);
}

