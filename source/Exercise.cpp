//
// Created by Richard on 23.04.2020.
//

#include "Exercise.h"


/**
 * returns
 *     like this:
 *     "20;Kniebeuge hinten;100:5d/5d/5;120:4/5/6/7/7/8"
 *
 *      format fuer austausch. d steht for set isDone==true.
 * @return ManagedString formatted String representing the Exercise
 */
ManagedString Exercise::toString() {
    ManagedString text=myFormat("%d;%s;",custom_id,name.toCharArray());
        if(sets.size()>1){
        for (size_t n = 0; n < sets.size(); n++) {


            if(n<=sets.size()-1){
                ExerciseSet set=sets[n];
                ExerciseSet prev=sets[n-1];
                if(n==0) text=text+myFormat("%d:%d%s",set.weight, set.reps,set.isDone ? "d":"");
                else if(set.weight==prev.weight) text=text+myFormat("/%d%s",set.reps,set.isDone ? "d":"");
                else text=text+myFormat(";%d:%d%s",set.weight,set.reps,set.isDone ? "d":"");
            }
        }

    }
        else if(sets.size()==1){
            text=text+myFormat("%d:%d%s",sets[0].weight, sets[0].reps,sets[0].isDone ? "d":"");
        }
    return text;
}
/**
 * clears vector sets and assigns new name,id, and sets from String s
 * @param s [in] Managed String
 */
void Exercise::fromString(const ManagedString& s) {
    const char * exerciseString=s.toCharArray();
    char input[strlen(exerciseString)+1];
    strcpy(input,exerciseString);
    char *ptr;

// initialisieren und ersten Abschnitt erstellen
    ptr = strtok(input, ";");
    sets.clear();
    sets.reserve(10);
    custom_id=atoi(ptr);
    ExerciseSet exerciseSet;
    int cnt=1;
    while(ptr != nullptr) {
        ptr=strtok(ptr+strlen(ptr)+1,";");
        cnt++;
        if(cnt==2){
            name=ptr;
        }
        if(cnt>2){
            char line[strlen(ptr)+1];
            strcpy(line,ptr);
            char * weightptr;
            char * repsptr;


            weightptr=strtok(line,":");
            exerciseSet.weight=atoi(weightptr);
            repsptr=weightptr;
            while(repsptr!=NULL){
                repsptr=strtok(NULL,"/");
                if (repsptr!=NULL){
                    int done=strlen(repsptr)-1;
                    if(repsptr[done]=='d'){
                        exerciseSet.isDone=true;
                        repsptr[done]='\0';
                    } else {exerciseSet.isDone= false;}
                    exerciseSet.reps=atoi(repsptr);
                    sets.push_back(exerciseSet);
                }
            }
        }

    }
}

Exercise::Exercise() {
    sets.emplace_back();
    custom_id=0;
    name="";
}



