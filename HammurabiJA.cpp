#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cstdio> // For printf and scanf
using namespace std;

// Constants for messages when resources are insufficient
#define SHORT_LAND_MSG printf("Hammurabi, think again. You own only %d acres. Now then,\n", acres)
#define SHORT_GRAIN_MSG printf("Hammurabi, think again. You have only %d bushels of grain. Now then,\n", grain)

// Variables for game state (pop = population, grain = bushels of grain, etc.)
int pop, acres, grain, starved, born, yield, rats, food, year;

// Resigns from stewardship if an invalid action is attempted
void resign() {
    printf("Hammurabi, I cannot do what you wish. Get yourself another steward!\n");
    exit(0);
}

// Ends the game with a rank based on performance (same logic as Java)
void endgame(int rank) {
    switch (rank) {
    case 0:
        // Worst performance
        printf("Due to this extreme mismanagement you have not only been impeached\n"
            "and thrown out of office but you have been also declared a national fink!\n");
        break;
    case 1:
        // Below average performance
        printf("Your heavy-handed performance smacks of Nero and Ivan IV. The people\n"
            "(remaining) find you an unpleasant ruler and frankly hate your guts!!\n");
        break;
    case 2:
        // Average performance
        printf("Your performance could have been somewhat better but really wasn't\n"
            "too bad at all. %d people dearly would like to see you assassinated\n"
            "but we all have our trivial problems.\n", pop * (rand() % 80) / 100);
        break;
    default:
        // Best performance
        printf("A fantastic performance!!! Charlemagne, Disraeli, and Jefferson\n"
            "combined could not have done better!!\n");
    }
}

// Displays the annual report (mirrors the Java code)
void report() {
    printf("\n\nHammurabi, I beg to report to you;\n\n");
    printf("In year %d, %d people starved, %d came to the city.\n", year, starved, born);
    // Plague event: occurs 15% of the time
    if (rand() < (RAND_MAX / 100 * 15) && year > 1) {
        printf("A horrible plague struck! Half the people died.\n");
        pop /= 2;
    }
    printf("Population is now %d.\nThe city now owns %d acres.\n"
        "You harvested %d bushels per acre.\nRats ate %d bushels.\n"
        "You now have %d bushels in store.\n\n", pop, acres, yield, rats, grain);
}

// Handles buying or selling land (same price fluctuation logic as Java)
void buysell() {
    int input;
    int price = rand() % 10 + 17; // Land price fluctuates between 17-26 bushels
    printf("Land is trading at %d bushels per acre.\n", price);
    printf("How many acres do you wish to buy? ");
    if (scanf_s("%d", &input) != 1 || input < 0) resign();

    // Check if enough grain is available to buy land
    while (price * input > grain) {
        SHORT_GRAIN_MSG;
        printf("How many do you wish to buy? ");
        if (scanf_s("%d", &input) != 1 || input < 0) resign();
    }

    if (input) {
        // Update land and grain after buying
        acres += input;
        grain -= price * input;
    }
    else {
        // Selling land if input is 0 or less
        printf("How many acres do you wish to sell? ");
        if (scanf_s("%d", &input) != 1 || input < 0) resign();

        while (input > acres) {
            SHORT_LAND_MSG;
            printf("How many do you wish to sell? ");
            if (scanf_s("%d", &input) != 1 || input < 0) resign();
        }

        // Update land and grain after selling
        acres -= input;
        grain += input * price;
    }
}

// Handles feeding the population (checks if enough grain is available)
void feed() {
    do {
        printf("How many bushels do you wish to feed your people? ");
        if (scanf_s("%d", &food) != 1 || food < 0) resign();
        if (food > grain) SHORT_GRAIN_MSG; // Not enough grain to feed
    } while (food > grain);
}

// Handles planting crops (checks if enough grain, land, and people are available)
void farm() {
    int input, valid;
    do {
        valid = 1; // Reset validation
        printf("How many acres do you wish to plant with seed? ");
        if (scanf_s("%d", &input) != 1 || input < 0) resign();

        if (input > acres) { SHORT_LAND_MSG; valid = 0; }
        else if (input / 2 > grain) { SHORT_GRAIN_MSG; valid = 0; }
        else if (input > 10 * pop) {
            // Not enough people to tend the fields
            printf("But you only have %d people to tend the fields! Now then,\n", pop);
            valid = 0;
        }
    } while (!valid);

    // Planting consumes grain and yields crops
    grain -= input / 2;
    yield = rand() % 5 + 1;  // Yield per acre (1-5 bushels)
    rats = rand() % 5 + 1;   // Rats eat grain, based on a random condition
    rats = (rats % 2) ? grain / rats : 0; // 50% chance for rats to eat
    grain += input * yield - rats; // Update grain after harvest and rats
}

// Main function (translated from Java)
int main() {
    int cstarved = 0; // Total starved
    int cps = 0;      // Cumulative percentage starved
    srand(static_cast<unsigned int>(time(NULL))); // Initialize random seed

    // Game setup
    printf("Hammurabi\n\nTry your hand at governing ancient Samaria for a ten-year term of office.\n");
    pop = 100;    // Starting population
    grain = 2800; // Initial grain in storage
    yield = 3;    // Initial yield per acre
    rats = 200;   // Rats eat 200 bushels initially
    acres = 1000; // Starting land
    born = 5;     // Initial new citizens

    // 10-year game loop
    for (year = 1; year <= 10 && starved < pop * 45 / 100; year++) {
        report();
        buysell();
        feed();
        farm();

        // Update population dynamics
        born = ((rand() % 5 + 1) * (20 * acres + grain) / pop / 100) + 1;
        starved = (pop < food / 20) ? 0 : pop - food / 20; // Starved if food is insufficient
        pop += born - starved; // Update population
        cstarved += starved;   // Track total starved
        cps += starved * 100 / pop; // Calculate starvation percentage
    }

    // Check if the player failed (too many starved in one year)
    if (starved >= pop * 45 / 100) {
        printf("\nYou starved %d people in one year!!!\n", starved);
        endgame(0);
    }
    else {
        year--; // End the game and calculate final results
        report();
        cps /= 10; // Average starvation percentage over 10 years
        printf("In your 10-year term of office %d%% of the people died per year\n"
            "on the average, i.e., a total of %d people died!\n", cps, cstarved);
        acres /= pop; // Acres per person
        printf("You started with 10 acres per person and ended with %d acres per\n"
            "person.\n\n", acres);
        endgame((cps < 34 && acres > 6) + (cps < 11 && acres > 8) + (cps < 4 && acres > 9));
    }

    return 0;
}
