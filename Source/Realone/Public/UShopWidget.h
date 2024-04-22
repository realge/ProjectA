// UShopWidget.h

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "UShopWidget.generated.h"

UCLASS()
class REALONE_API UShopWidget : public UUserWidget
{
    GENERATED_BODY()
    
public:
    UFUNCTION(BlueprintCallable, Category = "UI")
    void UpdateCurrencyDisplay();

    UFUNCTION(BlueprintCallable, Category = "UI")
    void OnBuyButtonPressed();

protected:
    virtual void NativeConstruct() override;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* CurrencyDisplay;

    UPROPERTY(meta = (BindWidget))
    UButton* BuyButton;
};
