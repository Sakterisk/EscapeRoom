// Copyright Epic Games, Inc. All Rights Reserved.

#include "EscapeRoomCharacter.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Engine/LocalPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/PlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "InteractComponent.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// AEscapeRoomCharacter

AEscapeRoomCharacter::AEscapeRoomCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
		
	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	//Mesh1P->SetRelativeRotation(FRotator(0.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));

	PhysicsHandle = CreateDefaultSubobject<UPhysicsHandleComponent>(TEXT("PhysicsHandle"));
	InteractComponent = CreateDefaultSubobject<UInteractComponent>(TEXT("InteractComponent"));
	InteractComponent->SetupAttachment(FirstPersonCameraComponent);
	InteractComponent->SetPhysicsHandle(PhysicsHandle);
	
	GetCharacterMovement()->NavAgentProps.bCanCrouch = false;
	GetCharacterMovement()->bUseFlatBaseForFloorChecks = true;

}

void AEscapeRoomCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();
}

//////////////////////////////////////////////////////////////////////////// Input

void AEscapeRoomCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{	
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AEscapeRoomCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AEscapeRoomCharacter::Look);

		// Clicking
		EnhancedInputComponent->BindAction(ClickAction, ETriggerEvent::Started, this, &AEscapeRoomCharacter::Press);
		EnhancedInputComponent->BindAction(ClickAction, ETriggerEvent::Completed, this, &AEscapeRoomCharacter::Release);

		// Inspecting
		EnhancedInputComponent->BindAction(InspectAction, ETriggerEvent::Completed, this, &AEscapeRoomCharacter::Inspect);

		// Restarting
		EnhancedInputComponent->BindAction(ResetAction, ETriggerEvent::Completed, this, &AEscapeRoomCharacter::ResetLevel);

		// Pausing
		EnhancedInputComponent->BindAction(PauseAction, ETriggerEvent::Completed, this, &AEscapeRoomCharacter::Pause);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AEscapeRoomCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add movement 
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void AEscapeRoomCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X * 0.5f);
		AddControllerPitchInput(LookAxisVector.Y * 0.5f);
	}
}

void AEscapeRoomCharacter::Press(const FInputActionValue& Value)
{
	InteractComponent->Press();
}

void AEscapeRoomCharacter::Release(const FInputActionValue& Value)
{
	InteractComponent->Release();
}

void AEscapeRoomCharacter::Inspect(const FInputActionValue& Value)
{
	InteractComponent->Inspect();
}

void AEscapeRoomCharacter::SwitchInspecting(bool inspecting)
{	
	bIsInspecting = inspecting;
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	PlayerController->bShowMouseCursor = inspecting;
	PlayerController->bEnableClickEvents = inspecting;
	PlayerController->bEnableMouseOverEvents = inspecting;

	if (inspecting)
	{
		CrosshairWidget->SetVisibility(ESlateVisibility::Hidden);
		FInputModeGameAndUI InputMode;
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		InputMode.SetHideCursorDuringCapture(false);
		PlayerController->SetInputMode(InputMode);
	}
	else
	{
		CrosshairWidget->SetVisibility(ESlateVisibility::Visible);
		FInputModeGameOnly InputMode;
		PlayerController->SetInputMode(InputMode);
	}
	PlayerController->SetIgnoreLookInput(inspecting);
	PlayerController->SetIgnoreMoveInput(inspecting);
}

void AEscapeRoomCharacter::ChangeCrosshair(FString NewCrosshair)
{
	if (CrosshairWidget) 
	{
		CrosshairWidget->ChangeCrosshair(NewCrosshair);
	}
}

void AEscapeRoomCharacter::InteruptInspecting()
{
	InteractComponent->Inspect();
}

void AEscapeRoomCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEscapeRoomCharacter::ResetLevel(const FInputActionValue& Value)
{
	UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), false);
}

void AEscapeRoomCharacter::Pause(const FInputActionValue& Value)
{
	if (bIsInspecting) 
	{
		InteractComponent->Inspect();
		return;
	}
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
	{
		if (PlayerController->IsPaused())
		{
			UnpauseGame(PlayerController);
		}
		else
		{
			PlayerController->bShowMouseCursor = true;
			PlayerController->bEnableClickEvents = true;
			PlayerController->bEnableMouseOverEvents = true;
			if (PauseWidget)
			{
				PauseWidget->AddToViewport();
			}
			if (CrosshairWidget)
			{
				CrosshairWidget->SetVisibility(ESlateVisibility::Hidden);
			}
			FInputModeUIOnly InputMode;
			InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
			InputMode.SetWidgetToFocus(PauseWidget->TakeWidget());
			PlayerController->SetInputMode(InputMode);
			PlayerController->SetIgnoreLookInput(true);
			PlayerController->SetIgnoreMoveInput(true);
		}
	}
}

void AEscapeRoomCharacter::UnpauseGame(APlayerController* PlayerController)
{
	PlayerController->bShowMouseCursor = false;
	PlayerController->bEnableClickEvents = false;
	PlayerController->bEnableMouseOverEvents = false;
	if (PauseWidget)
	{
		PauseWidget->RemoveFromParent();
	}
	if (CrosshairWidget)
	{
		CrosshairWidget->SetVisibility(ESlateVisibility::Visible);
	}
	FInputModeGameOnly InputMode;
	PlayerController->SetInputMode(InputMode);
	PlayerController->SetIgnoreLookInput(false);
	PlayerController->SetIgnoreMoveInput(false);
}