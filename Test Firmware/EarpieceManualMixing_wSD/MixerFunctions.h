
#include "State.h"

//Please note that the "LEFT" mic is above the pink mic jack, 
//   and the "RIGHT" mic is above the black headphone jack.  
//   Ignore the labels on the PCB
#define LEFT_FRONT  0
#define LEFT_REAR   1
#define RIGHT_FRONT 2
#define RIGHT_REAR  3

extern Tympan myTympan;
extern EarpieceShield earpieceShield;
extern AudioMixer4_F32 inputMixerL;
extern AudioMixer4_F32 inputMixerR;

//set the desired input source 
void setInputSource(int micInput) { 
  switch (micInput) {
    case INPUT_PCBMICS:
      //Select Input
      myTympan.inputSelect(TYMPAN_INPUT_ON_BOARD_MIC); // use the on-board microphones
      earpieceShield.inputSelect(TYMPAN_INPUT_ON_BOARD_MIC);
      break;
    case INPUT_MICJACK_MIC:
      //Select Input
      myTympan.inputSelect(TYMPAN_INPUT_JACK_AS_MIC); // use the mic jack
      earpieceShield.inputSelect(TYMPAN_INPUT_JACK_AS_MIC);
      break;
    case INPUT_MICJACK_LINEIN:
      //Select Input
      myTympan.inputSelect(TYMPAN_INPUT_JACK_AS_LINEIN); // use the mic jack
      earpieceShield.inputSelect(TYMPAN_INPUT_JACK_AS_LINEIN);
      break;
    case INPUT_PDMMICS:
      //The Tympan earpieces use PDM Digital Microphones.  Choose this option for Tympan earpieces
      myTympan.enableDigitalMicInputs(true);
      earpieceShield.enableDigitalMicInputs(true);
      break;
  }

  // If turning on digital mics, enable them. Otherwise disable them.
  if (micInput==INPUT_PDMMICS)  {
    //Enable Digital Mic (enable analog inputs)
    myTympan.enableDigitalMicInputs(true);
    earpieceShield.enableDigitalMicInputs(true);
  }  else  {
    //Disable Digital Mic (enable analog inputs)
    myTympan.enableDigitalMicInputs(false);
    earpieceShield.enableDigitalMicInputs(false);
  }
    
}


//Sets the input mixer gain for a given mic channel.  
//Mixes between front and rear microphone of each earpiece
void setInputMixer(int micChannelName, float gainVal) {     
  //initialize to muted
  inputMixerL.gain(LEFT_FRONT, 0.0);
  inputMixerL.gain(LEFT_REAR,  0.0);
  inputMixerL.gain(RIGHT_FRONT,0.0);
  inputMixerL.gain(RIGHT_REAR, 0.0);
  inputMixerR.gain(LEFT_FRONT, 0.0);
  inputMixerR.gain(LEFT_REAR,  0.0);
  inputMixerR.gain(RIGHT_FRONT,0.0);
  inputMixerR.gain(RIGHT_REAR, 0.0);
  
  //turn on the desired channels
  switch (micChannelName) {
    case MIC_FRONT_LEFT:
      inputMixerL.gain(LEFT_FRONT,gainVal); 
      Serial.print("Mic Left-Front, L-only: Gain ");
      break;
    case MIC_REAR_LEFT:
      inputMixerL.gain(LEFT_REAR,gainVal); 
      Serial.print("Mic Left-Rear, L-only: Gain ");
      break;
    case MIC_FRONT_RIGHT:
      inputMixerR.gain(RIGHT_FRONT,gainVal);
      Serial.print("Mic Right-Front, R-only: Gain ");
      break;
    case MIC_REAR_RIGHT:
      Serial.print("Mic Right-Rear, R-only: Gain ");
      inputMixerR.gain(RIGHT_REAR,gainVal); 
      break;
    case MIC_FRONT_BOTH:
      Serial.print("Front Mics, L/R: Gain ");
      inputMixerL.gain(LEFT_FRONT,gainVal); 
      inputMixerR.gain(RIGHT_FRONT,gainVal); 
      break;         
    case MIC_REAR_BOTH:
      Serial.print("Rear Mics, L/R: Gain ");
      inputMixerL.gain(LEFT_REAR,gainVal); 
      inputMixerR.gain(RIGHT_REAR,gainVal); 
      break;
    case MIC_AIC0:
      Serial.print("Tympan AIC, L/R: Gain ");
      inputMixerL.gain(0,gainVal); 
      inputMixerR.gain(1,gainVal); 
      break;         
    case MIC_AIC1:
      Serial.print("Earpiece Shield AIC, L/R: Gain ");
      inputMixerL.gain(2,gainVal); 
      inputMixerR.gain(3,gainVal); 
      break; 
    case ALL_MICS:
      Serial.print("Front+Rear Mics, L/R: Gain ");
      inputMixerL.gain(LEFT_FRONT, gainVal);  //Left-Front Mic
      inputMixerL.gain(LEFT_REAR,  gainVal);  //Left-Rear Mic
      inputMixerR.gain(RIGHT_FRONT,gainVal);  //Right-Front Mic
      inputMixerR.gain(RIGHT_REAR, gainVal);  //Right-Rear Mic
      break;     
    // For testing, sends the front mic to the left output, and the rear mics to the right output
    case MIC_FRONT_TO_LEFT_REAR_TO_RIGHT:
      Serial.print("Front Mics output to Left Side, Rear Mics to Right Side. Frt/Rear: Gain ");
      inputMixerL.gain(LEFT_FRONT,  gainVal);
      inputMixerR.gain(LEFT_REAR,   gainVal);
      break;     
  }
  Serial.println(gainVal);
}
