#include <Wire.h>
#include <LiquidCrystal_I2C.h>
// Set the LCD address to 0x3F for a 16 chars and 2 line display
// see scanner code for how to get address

LiquidCrystal_I2C lcd(0x3F, 16, 2);
int interval = 1000;
unsigned long time_now = 0;
void setup()
{
	// initialize the LCD
	lcd.init();
	lcd.backlight();
}
void loop()
{
	lcd.setCursor(0,0);
	// it will start from the 1st row and 1st column
	/* here is the 16X2 pixcle setcursor possition works
	* 0,0 1,0 2,0 3,0 4,0 5,0 6,0 7,0 8,0 9,0 10,0 11,0 12,0 13,0 14,0 15,0
	* 0,1 1,1 2,1 3,1 4,1 5,1 6,1 7,1 8,1 9,1 10,1 11,1 12,1 13,1 14,1 15,1
	* note that even though it can be control every pixcle possition, but
	* make sure it does not overlap * the data
	* It is only set the starting point. Make sure you have enough pixcle
	* possition for 1st one before
	* setting the 2nd pixcle set.
	* One way to slove this issue to use autoscroll() funtion, but you may
	* have to use delay instate of
	* millis().
	*
	*/

/* for (int i = 0; i <10; i++)
* // just a loop so it shift and display the data
// it is good when you have more than 16 data in an array
//{
lcd.print("Hello, world!");
{
*/
	lcd.autoscroll();

	if (millis() > time_now + interval)
	{
		for (int i = 0; i <10; i++){
			lcd.print("Hello, world!");
		// Use the snapshot to set track time until next event
		}
		time_now= millis();
	}
//lcd.setCursor(0,1);
// it will start from the 2st row and 1st column
// lcd.autoscroll();
//lcd.print("Hello, world!");
//delay (1000);
//lcd.noAutoscroll();
}