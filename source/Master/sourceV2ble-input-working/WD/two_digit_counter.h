#if !defined(_TWO_DIGIT_COUNTER)
#define _TWO_DIGIT_COUNTER

#include "MicroBit.h"




const uint8_t ei[][25] {{0,0,0,250,250,
                        0,0,0,250,250,
                        0,0,0,250,250,
                        0,0,0,250,250,
                        0,0,0,250,250,
                        } ,//0
                          {
                          0,0,0,0,250,
                          0,0,0,0,250,
                          0,0,0,0,250,
                          0,0,0,0,250,
                          0,0,0,0,250,
                          }, //01
                          {
                          0,0,0,250,250,
                          0,0,0,0,  250,
                          0,0,0,250,250,
                          0,0,0,250,0,
                          0,0,0,250,250
                          },//2
                          {
                          0,0,0,250,250,
                          0,0,0,0,  250,
                          0,0,0,250,250,
                          0,0,0,0,  250,
                          0,0,0,250,250,
                          } , //3
                          {
                          0,0,0,250,0,  
                          0,0,0,250,0,  
                          0,0,0,250,250,
                          0,0,0,0,250  ,
                          0,0,0,0,250
                          },    //4
                          {
                          0,0,0,250,250,
                          0,0,0,250,  0,
                          0,0,0,250,250,
                          0,0,0,0,  250,
                          0,0,0,250,250
                          } , //5
                          {
                          0,0,0,250,  0,
                          0,0,0,250,  0,
                          0,0,0,250,250,
                          0,0,0,250,250,
                          0,0,0,250,250
                            } ,//6
                          {              
                          0,0,0,250,250,
                          0,0,0,0,  250,
                          0,0,0,0,  250,
                          0,0,0,0,  250,
                          0,0,0,0,  250 
                            } , //7
                          {
                          0,0,0,250,250,
                          0,0,0,250,250,
                          0,0,0,    0,0,
                          0,0,0,250,250,
                          0,0,0,250,250,  
                           } , //8
                          {
                          0,0,0,250,250,
                          0,0,0,250,250,
                          0,0,0,250,250,
                          0,0,0,0,  250,
                          0,0,0,0,  250,
                           } , //9
                        };

const uint8_t ze[][25] { {250,250,0,0,0,
                          250,250,0,0,0,
                          250,250,0,0,0,
                          250,250,0,0,0,
                          250,250,0,0,0} ,//0
                          {
                          0,250,0,0,0,
                          0,250,0,0,0,
                          0,250,0,0,0,
                          0,250,0,0,0,
                          0,250,0,0,0},//250
                          {
                          250,250,0,0,0,
                            0,250,0,0,0,
                          250,250,0,0,0,
                          250,0,0,0,0,
                          250,250,0,0,0} ,//2
                          {
                          250,250,0,0,0,
                            0,250,0,0,0,
                          250,250,0,0,0,
                            0,250,0,0,0,
                          250,250,0,0,0 } , //3
                          {
                          250,0,0,0,0,
                          250,0,0,0,0,
                          250,250,0,0,0,
                          0,250,0,0,0,
                          0,250,0,0,0} , //4
                          {
                          250,250,0,0,0,
                          250,0,0,0,0,
                          250,250,0,0,0,
                          0,  250,0,0,0,
                          250,250,0,0,0} , //5
                          {
                          250,0,0,0,0,
                          250,0,0,0,0,
                          250,250,0,0,0,
                          250,250,0,0,0,
                          250,250,0,0,0} ,//6
                          {
                          250,250,0,0,0,
                          0,  250,0,0,0,
                          0,  250,0,0,0,
                          0,  250,0,0,0,
                          0,  250,0,0,0} , //7
                          {
                          250,250,0,0,0,
                          250,250,0,0,0,
                              0,0,0,0,0,
                          250,250,0,0,0,
                          250,250,0,0,0} , //8
                          {
                          250,250,0,0,0,
                          250,250,0,0,0,
                          250,250,0,0,0,
                          0,  250,0,0,0,
                          0,  250,0,0,0} , //9
                        };




MicroBitImage initDigit(&MicroBitImage mbimage,int* numberarray);

void printTwoDigit(int wert,MicroBitImage* imgs_ze,MicroBitImage* imgs_ei);
#endif // _TWO_DIGIT_COUNTER