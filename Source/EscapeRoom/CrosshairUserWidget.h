#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CrosshairUserWidget.generated.h"

UCLASS()
class ESCAPEROOM_API UCrosshairUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent, Category = "Crosshair")
	void ChangeCrosshair(const FString& NewCrosshair);
};
