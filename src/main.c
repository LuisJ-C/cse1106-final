#include <stdio.h>
#include <pigpio.h>

#define PIN 17

int main() {

float temp, min = 68, max = 76;
float sum = 0;
int count = 0;
int choice;

FILE *file;

// STEP 1: Start Raspberry Pi GPIO
if (gpioInitialise() < 0) {
printf("GPIO failed!\n");
return 1;
}

gpioSetMode(PIN, PI_OUTPUT);
gpioWrite(PIN, 0);

// STEP 2: Open file to save temperatures
file = fopen("temperatures.txt", "a");

if (file == NULL) {
printf("Error opening file!\n");
gpioWrite(PIN, 0);
gpioTerminate();
return 1;
}

// STEP 3: Set temperature range
printf("SMART CLASSROOM COMFORT MONITOR\n");

printf("Change temperature range? (1=Yes, 0=No): ");
if (scanf("%d", &choice) != 1) {
printf("Invalid input!\n");
fclose(file);
gpioTerminate();
return 1;
}

if (choice == 1) {
printf("Enter minimum temperature: ");
if (scanf("%f", &min) != 1) {
printf("Invalid input!\n");
fclose(file);
gpioTerminate();
return 1;
}

printf("Enter maximum temperature: ");
if (scanf("%f", &max) != 1) {
printf("Invalid input!\n");
fclose(file);
gpioTerminate();
return 1;
}
}

if (min > max) {
printf("Invalid temperature range!\n");
fclose(file);
gpioTerminate();
return 1;
}

printf("\nAcceptable range: %.1f F - %.1f F\n",
min, max);

// STEP 4: Enter temperature readings
while (1) {

printf("\nEnter temperature (-999 to stop): ");

if (scanf("%f", &temp) != 1) {
printf("Invalid temperature input!\n");
break;
}

if (temp == -999) {
break;
}

// STEP 5: Check temperature and control LED
if (temp >= min && temp <= max) {

printf("Temperature: %.1f F\n", temp);
printf("Status: NORMAL\n");
printf("LED: OFF\n");

gpioWrite(PIN, 0);

// Save NORMAL reading to file
fprintf(file,
"%.1f F, NORMAL, LED OFF\n",
temp);
}
else {

printf("Temperature: %.1f F\n", temp);
printf("Status: WARNING\n");
printf("LED: ON\n");

gpioWrite(PIN, 1);

// Save WARNING reading to file
fprintf(file,
"%.1f F, WARNING, LED ON\n",
temp);
}

fflush(file);

// STEP 6: Add reading to average
sum = sum + temp;
count++;
}

// STEP 7: Calculate average
if (count > 0) {
printf("\nAverage temperature: %.2f F\n",
sum / count);
}
else {
printf("\nNo temperatures recorded.\n");
}

// STEP 8: Close file and turn LED off
gpioWrite(PIN, 0);

fclose(file);
gpioTerminate();

printf("Readings saved to temperatures.txt\n");

return 0;
}

