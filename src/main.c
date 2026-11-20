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

// STEP 2: Open file
file = fopen("temperatures.txt", "a");

if (file == NULL) {
printf("File error!\n");
gpioTerminate();
return 1;
}

// STEP 3: Change temperature range
printf("Change temperature range? (1=Yes, 0=No): ");
scanf("%d", &choice);

if (choice == 1) {
printf("Enter minimum: ");
scanf("%f", &min);

printf("Enter maximum: ");
scanf("%f", &max);
}

if (min > max) {
printf("Invalid range!\n");
fclose(file);
gpioTerminate();
return 1;
}

// STEP 4: Enter temperatures
while (1) {
printf("\nEnter temperature (-999 to stop): ");
if (scanf("%f", &temp) != 1)
break;

if (temp == -999)
break;

// STEP 5: Check temperature
if (temp >= min && temp <= max) {
printf("NORMAL - LED OFF\n");
gpioWrite(PIN, 0);

fprintf(file, "%.1f NORMAL LED OFF\n", temp);
}
else {
printf("WARNING - LED ON\n");
gpioWrite(PIN, 1);

fprintf(file, "%.1f WARNING LED ON\n", temp);
}

// STEP 6: Calculate average
sum = sum + temp;
count++;
}

if (count > 0) {
printf("\nAverage: %.2f F\n", sum / count);
}

// STEP 7: Finish
gpioWrite(PIN, 0);
fclose(file);
gpioTerminate();

return 0;
}
