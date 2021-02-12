//
//  main.cpp
//  Blackjack game
//
//  Created by Martin Fahlander on 2021-02-10.
//

#include <iostream> // Input and output
#include <vector> // Deck and hand vectors
#include <random> // Deck shuffle
#include <chrono> // Deck shuffle
#include <sstream> // Stringstream
using namespace std;

// Get name of card
string getCardName (int card) {
    
    // Keep track of card name
    string name = "";
    
    // Decide card name
    switch (card) {
        case 1: name = "ACE"; break;
        case 11: name = "JACK"; break;
        case 12: name = "QUEEN"; break;
        case 13: name = "KING"; break;
        default: name = to_string(card);
    }
    return name;
}

// Calculate value of card
int getCardValue (int card, int handSum) {
    
    // Keep track of card value
    int value = 0;
    
    // Decide value of card
    switch (card) {
        // Give dressed cards value of 10
        case 11: case 12: case 13: value = 10; break;
        // Give other cards their numerical value
        default: value = card;
    }
    return value;
}

// Calculate total value of aces on hand
int getTotalAceValue (int aceCount, int handSum){
    
    // Keep track of total value of aces
    int value = 0;
    
    // Go through all aces
    for (int i = 0; i < aceCount; i++){
        
        // Calculate if sum of hand can handle an ace with value of 11
        if (handSum + value + 11 <= 21)
            value += 11;
        
        // Give ace value of 1
        else
            value += 1;
    }
    return value;
}

// Calculate sum of hand
int getHandSum (vector <int> hand) {
    
    // Keep track of sum of hand
    int handSum = 0;
    
    // Keep track of aces on hand
    int aceCount = 0;
    
    // Go through all cards on hand
    for (int i = 0; i < hand.size(); i++){
        
        // Check if card is an ace
        if (hand[i] == 1)
            aceCount++;
        
        // Calculate value of non-ace cards
        else
            handSum += getCardValue(hand[i], handSum);
    }
    
    // Check if hand contains ace and calculate value of aces
    if (aceCount > 0)
        handSum += getTotalAceValue(aceCount, handSum);
    return handSum;
}

// Show hand
void showHand (vector <int> hand, bool dealerHiddenCard) {
    
    // Show hand with first card hidden
    if (dealerHiddenCard) {
        cout << "HIDDEN ";
        for (int i = 1; i < hand.size(); i++)
            cout << getCardName(hand[i]) << " ";
    }
    
    // Show hand
    else
        for (int i = 0; i < hand.size(); i++)
            cout << getCardName(hand[i]) << " ";
    cout << endl;
}

// Play a game
int playGame (int bet, int gameCount, int deckNumber) {
    
    cout << endl << " - GAME " << gameCount << " - " << endl;
    
    // Create deck of cards
    vector <int> deck;
    for (int i = 0; i < 52 * deckNumber; i++)
        deck.push_back(i % 13 + 1);
    
    // Shuffle deck
    unsigned seed = (unsigned) chrono::system_clock::now().time_since_epoch().count();
    shuffle(begin(deck), end(deck), default_random_engine(seed));
    
    // Keep track the next card to give
    int activeCard = 0;
    
    // Create hands
    vector<int> playerHand;
    vector<int> dealerHand;
    
    // Give first card to player
    playerHand.push_back(deck[activeCard]);
    activeCard++;
    cout << "You are dealt " << getCardName(playerHand[0]) << endl;
    
    // Give first card to dealer
    dealerHand.push_back(deck[activeCard]);
    activeCard++;
    cout << "Dealer is dealt a hidden card" << endl;
    
    // Give second card to player
    playerHand.push_back(deck[activeCard]);
    activeCard++;
    cout << "You are dealt " << getCardName(playerHand[1]) << endl;
    
    // Give second card to dealer
    dealerHand.push_back(deck[activeCard]);
    activeCard++;
    cout << "Dealer is dealt " << getCardName(dealerHand[1]) << endl;
    
    // Keep track of hidden status of dealer's first card
    bool dealerHiddenCard = true;
    
    // Game still active
    bool gameActive = true;
    
    // Calculate sum of hands
    int playerHandSum = getHandSum(playerHand);
    int dealerHandSum = getHandSum(dealerHand);
    
    // Keep track of blackjacks and busts
    bool playerBlackjack = false;
    bool dealerBlackjack = false;
    bool playerBust = false;
    bool dealerBust = false;
    
    // Calculate blackjacks
    if (playerHandSum == 21){
        playerBlackjack = true;
        gameActive = false;
        cout << "You have a Blackjack!" << endl;
        
        // Show hands
        cout << "YOUR HAND: ";
        showHand(playerHand, false);
        cout << "DEALER'S HAND: ";
        showHand(dealerHand, dealerHiddenCard);
    }
    if (dealerHandSum == 21){
        dealerBlackjack = true;
    }
    
    // Game active
    while (gameActive){
        
        // Keep track of whether players are still playing
        bool playerPlaying = true;
        bool dealerPlaying = true;
        
        // Player's turn
        while (playerPlaying) {
            
            // Show hands
            cout << endl;
            cout << "YOUR HAND: ";
            showHand(playerHand, false);
            cout << "DEALER'S HAND: ";
            showHand(dealerHand, dealerHiddenCard);
            
            // Ask player for action
            cout << "1) Hit 2) Stand" << endl << "Choose action: ";
            string actInput = "";
            cin >> actInput;
            // Give player new card
            if (actInput == "1") {
                int newCard = deck[activeCard];
                playerHand.push_back(newCard);
                activeCard++;
                cout << "You are dealt " << getCardName(newCard) << endl;
            }
            
            // Player finished
            else if (actInput == "2") {
                playerPlaying = false;
                gameActive = false;
                cout << "You stand" << endl;
            }
            
            else {
                cout << "Invalid input, try again" << endl;
            }
            
            // Check if player has gone bust
            playerHandSum = getHandSum(playerHand);
            if (playerHandSum > 21) {
                gameActive = false;
                playerPlaying = false;
                dealerPlaying = false;
                playerBust = true;
                cout << endl << "You go bust! The sum of your hand exceeds 21" << endl;
            }
        }
        
        // Remove hidden status of dealer's first card
        dealerHiddenCard = false;
        
        // Dealer's turn
        while (dealerPlaying) {
            
            // Show hands
            dealerHandSum = getHandSum(dealerHand);
            cout << "YOUR HAND: ";
            showHand(playerHand, false);
            cout << "DEALER'S HAND: ";
            showHand(dealerHand, dealerHiddenCard);
            
            // State if dealer has blackjack
            if (dealerBlackjack) {
                dealerPlaying = false;
                gameActive = false;
                cout << endl << "Dealer has Blackjack!" << endl;
            }
            
            // Check if dealer has gone bust
            else if (dealerHandSum > 21) {
                gameActive = false;
                dealerPlaying = false;
                dealerBust = true;
                cout << endl << "Dealer goes bust! The sum of their hand exceeds 21" << endl;
            }
            
            // Dealer finished
            else if (dealerHandSum >= 17 && dealerHandSum <= 21){
                gameActive = false;
                dealerPlaying = false;
                cout << "Dealer stands" << endl;
            }
            
            // Give dealer new card
            else {
                int newCard = deck[activeCard];
                dealerHand.push_back(newCard);
                activeCard++;
                cout << "Dealer is dealt " << getCardName(newCard) << endl;
                cout << "YOUR HAND: ";
                
                // Show hands
                showHand(playerHand, false);
                cout << "DEALER'S HAND: ";
                showHand(dealerHand, dealerHiddenCard);
                cout << endl;
            }
        }
    }
    
    // Calculate sums of hands
    playerHandSum = getHandSum(playerHand);
    cout << "Sum of your hand: " << playerHandSum << endl;
    dealerHandSum = getHandSum(dealerHand);
    cout << "Sum of the dealer's hand: " << dealerHandSum << endl;
    
    // Calculate player's profit based on result
    int profit = 0;
    if (playerBust)
        profit = 0;
    else if (playerBlackjack && !dealerBlackjack)
        profit = bet * 2.5;
    else if (playerBlackjack && dealerBlackjack)
        profit = bet;
    else if (playerHandSum > dealerHandSum && playerHandSum <= 21)
        profit = bet * 2;
    else if (playerHandSum == dealerHandSum && playerHandSum <= 21)
        profit = bet;
    else if (playerHandSum <= 21 && dealerHandSum > 21)
        profit = bet * 2;
    else
        profit = 0;

    cout << " - END OF GAME - " << endl << endl;
    return profit;
}

int main() {
    
    cout << "Welcome to a game of Blackjack!" << endl;
    
    // Keep track of number of decks
    int deckNumber = 0;
    
    // Ask player for number of decks
    bool askForDeckNumber = true;
    while (askForDeckNumber) {
        
        cout << "Enter number of decks (1-8): ";
        string deckInput = "";
        cin >> deckInput;
        
        // Parse input through a stringstream
        stringstream ssDeck;
        ssDeck << deckInput;
        string temp = "";
        int tempNumber = 0;
        while (!ssDeck.eof()) {
            ssDeck >> temp;
            if(stringstream(temp) >> tempNumber)
                deckNumber = tempNumber;
        }
        if (deckNumber >= 1 && deckNumber <= 8)
            askForDeckNumber = false;
        else
            cout << "Invalid input, try again" << endl;
    }
    
    // Keep track of player's balance
    int balance = 100;
    cout << "You are given a balance of " << balance << " to start" << endl;
    
    // Keep track of number of games played
    int gameCount = 0;
    
    // While player still wants to proceed playing
    bool proceed = true;
    while (proceed) {
        
        // Ask player for amount to bet
        bool askToBet = true;
        
        // Keep track of bet
        int bet = 0;
        while (askToBet){
            string betInput = "";
            cout << "Enter betting amount: ";
            cin >> betInput;
            stringstream ssBet;
            ssBet << betInput;
            string temp = "";
            int tempNumber = 0;
            while (!ssBet.eof()) {
                ssBet >> temp;
                if(stringstream(temp) >> tempNumber)
                    bet = tempNumber;
            }
            if (bet > 0 && bet <= balance) {
                balance -= bet;
                askToBet = false;
            }
            else if (bet > 0)
                cout << "Insufficient funds, try a smaller amount" << endl;
            else
                cout << "Invalid bet, try again" << endl;
        }
        cout << "Balance: " << balance << endl;
        
        // Play a game and get profit
        gameCount++;
        int profit = 0;
        profit = playGame(bet, gameCount, deckNumber);
        if (profit > 0)
            cout << "You won " << profit << endl;
        else
            cout << "You won nothing" << endl;
        
        // Add profit to balance
        balance += profit;
        cout << "Balance: " << balance << endl;
        
        // Ask player to proceed playing
        bool askToProceed = true;
        while (askToProceed) {
            cout << "1) Start a new game 2) Quit playing" << endl << "Choose action: ";
            string proceedInput = "";
            cin >> proceedInput;
            
            // Player wants to play a new game
            if (proceedInput == "1") {
                askToProceed = false;
            }
            
            // Player does not want to play a new game
            else if (proceedInput == "2") {
                proceed = false;
                askToProceed = false;
            }
            else
                cout << "Invalid input, try again" << endl;
        }
    }
    cout << "Thanks for playing!" << endl;
    return 0;
}
