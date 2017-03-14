/*!
* \file stylus.h
* \brief File containing structures and prototypes linked to the stylus
*/
#ifndef STYLUS_H
#define STYLUS_H


/*!
* \struct Stylus 
* \brief A Stylus is actuated by a servomotor
*/
typedef struct {
	int pin;				/*!< physical pin for the servomotor */
	int clickPosition;		/*!< the position (in uS*10) of the servomotor to press the stylus on the screen */
	int restPosition;		/*!< the position (in uS*10) of the servomotor to put the stylus out of the screen */
	int pressDelay;			/*!< the delay needed by the device to take the click into account */
} Stylus;


int attach(int pin, int clickPosition, int restPosition, int pressDelay);
void update();
void disable();
void enable();
void click();


#endif //STYLUS_H
