// UShopWidget.cpp

#include "UShopWidget.h"
#include "../MyGameState.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"

void UShopWidget::NativeConstruct()
{
    Super::NativeConstruct();
    
    // Bind the button click event
    if (BuyButton)
    {
        BuyButton->OnClicked.AddDynamic(this, &UShopWidget::OnBuyButtonPressed);
    }

    // Update the display initially
    UpdateCurrencyDisplay();
}

void UShopWidget::UpdateCurrencyDisplay()
{
    AMyGameState* GameState = GetWorld()->GetGameState<AMyGameState>();
    if (GameState && CurrencyDisplay)
    {
        FString CurrencyText = FString::Printf(TEXT("Money: %d"), GameState->GlobalCurrency);
        CurrencyDisplay->SetText(FText::FromString(CurrencyText));
    }
}

void UShopWidget::OnBuyButtonPressed()
{
    AMyGameState* GameState = GetWorld()->GetGameState<AMyGameState>();
    if (GameState)
    {
        const int32 Cost = 20;  // Cost of the health upgrade
        if (GameState->GlobalCurrency >= Cost)
        {
            GameState->AddCurrency(-Cost);  // Subtract the cost from the player's currency
            // Here you would typically call another method to increase the player's health
            // For example: GameState->IncreaseHealth(10);
            
            UE_LOG(LogTemp, Warning, TEXT("Health upgrade purchased!"));
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Not enough money to purchase health upgrade."));
        }

        UpdateCurrencyDisplay();  // Update the currency display
    }
}
