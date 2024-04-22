// UInteractable.h

#pragma once

#include "UObject/Interface.h"
#include "Interactable.generated.h"

UINTERFACE(MinimalAPI, BlueprintType, Blueprintable)
class UInteractable : public UInterface
{
    GENERATED_BODY()
};

class REALONE_API IInteractable
{
    GENERATED_BODY()

public:
    // The function that's called when a player interacts with an object
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interaction")
    void Interact(AActor* Caller);
    virtual void Interact_Implementation(AActor* Caller) = 0;
};
