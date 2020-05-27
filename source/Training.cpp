//
// Created by Richard on 23.04.2020.
//

#include "Training.h"

/**
 * e.value is the no of lines sent via uart ble
 * == no of exercises in Training
 * @param e
 */
void Training::onExercisesChanged(MicroBitEvent e) {
    //e.value == no of lines == no of new exercises
    Exercise exercise = Exercise();
    exercises.clear();
    for (int i = 0; i < e.value; ++i) {
        exercise.fromString(uart->readUntil("\n"));
        exercises.push_back(exercise);
//        MicroBitEvent evt(TrainingID,rcvd);
    }

}

void Training::onRemoveExercise(MicroBitEvent e) {
//// e.value == index of deleted exercise 0..n
//if(exercises.size()>e.value){
//    exercises.erase(exercises.begin()+e.value);
//}

}

/**
 * set name of Training via uart ble
 * @param e
 */
void Training::onSetName(MicroBitEvent e) {
    name = uart->readUntil("\n");
}

/**
 * set Date of Training via Uart. format int == day since 1970 etc.
 * @param e
 */
void Training::onSetDate(MicroBitEvent e) {
    customDate = atoi(uart->readUntil("\n").toCharArray());
}



void Training::inputLoopOverExercises() {
    myInput in;
    int loop_case = 0;
    for (std::vector<Exercise>::size_type e = 0; e != exercises.size(); ) {
        Exercise &exercise = exercises.at(e);
        for (std::vector<ExerciseSet>::size_type s = 0; s != exercise.sets.size(); s++) {
            ExerciseSet &eSet = exercise.sets.at(s);
            loop_case = 0;
            uint16_t oldID;
            uint16_t newID;
            while(loop_case<3) {
                switch (loop_case) {
                    case 0:
                        uBit.display.scroll("ID");
                        uBit.display.clear();
                        oldID = exercise.custom_id;
                        newID = in.input(WAITFORINPUTTIME, oldID);
                        uBit.sleep(500);
                        if(terminator){
                            terminator=false;
                            exercises.erase(exercises.begin()+e);
                            goto nextExercise;
                        }
                        //TODO scroll Name of Exercise, from Flash DB?
//                  //  exercise.name=read_to_string(ManagedString(exercise.custom_id))
                        uBit.sleep(500);
                        if (newID != oldID && oldID !=0) {
                            auto it=std::find_if(exercises.begin(),exercises.end(),[newID](const Exercise& ex){
                                return ex.custom_id == newID;
                            });
                            if(it!=exercises.end()) {
                                std::iter_swap(exercises.begin()+e, it);
                                //TODO MicrobitEvent evt(EXERCISES_SWAPED_ID,byte[from,to])

                            } else {
                                Exercise newEx;
                                newEx.custom_id = newID;
                                newEx.sets.reserve(10);

                                    exercises.insert(exercises.begin() + e, newEx);
                                //TODO MicrobitEvent evt(EXERCISE_ADDED_ID,byte[UUID,customID])

                            }
                            goto nextExercise;
                        }
                        else{
                            exercise.custom_id=newID;
                        }
                        uBit.sleep(500);
                        loop_case++;
                        break;
                    case 1:

                        uBit.display.scroll("Kg", LONGTEXTDELAY);
                        uBit.sleep(300);
                        if (goback != 0) {

                            break;
                        }
                        eSet.weight=in.input(WAITFORINPUTTIME, eSet.weight);
                        //TODO MicrobitEvent evt(SET_WEIGHT_CHANGED_ID,byte[UUID,weight])


                        uBit.sleep(500);
                        loop_case += 1 - goback;
                        break;

                    case 2:
                        uBit.display.scroll("RPS", LONGTEXTDELAY);
                        if (goback != 0) {
                            break;
                        }
                        eSet.cntReps(in);
                        uBit.sleep(500);
                        uBit.display.clear();
                        if(s==exercise.sets.size()-1){
                        uBit.display.scroll("+ Set?",LONGTEXTDELAY);
                            displayLeftArrow();
                            while(inputBuff==0){
                            uBit.sleep(100);
                        }
                            if(inputBuff==INCREMENT_A){
                                exercise.sets.push_back(eSet);
                                uBit.display.scrollAsync("+",LONGTEXTDELAY);
                                inputBuff=0;
                            }
                            eSet.isDone=true;
                            //TODO MicrobitEvent evt(SET_REPS_CHANGED_ID,byte[UUID,reps])
                        inputBuff=0;
                        }
                        loop_case += 1 - goback;
                        break;
                    default:

                        break;
                }

                goback = 0;

                if (terminator == 1) {
                    terminator = false;
                    goto nextExercise;
                }
            }
        }

        if(e==exercises.size()-1){
            uBit.display.scroll("+ Ex? -",FASTSCROLLTIME);
            displayLeftArrow();
            while(inputBuff==0){

                uBit.sleep(100);
            }
            if(inputBuff==INCREMENT_A){
                exercises.emplace_back();
                uBit.display.scrollAsync("+",FASTSCROLLTIME);
            }
            inputBuff=0;
        }

        displayTick();
        e++;
        // prüfe ob Set Loop verlassen werden soll.
        nextExercise:;

    }
    uBit.sleep(500);
}


