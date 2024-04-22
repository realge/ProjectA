// MyGameState.cpp

#include "MyGameState.h"

AMyGameState::AMyGameState() {
    // Initialize currency to zero or any other desired start value
    GlobalCurrency = 0;
}

void AMyGameState::AddCurrency(int32 Amount) {
    GlobalCurrency += Amount;
}