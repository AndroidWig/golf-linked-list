// ECE 209 - Fall 2021 - Program 3
// This program reads information about a golf tournament
// and allows a user to view information about the course,
// the overall scores, and the individual players.
//
// Implementation of Course ADT and functions for Player and PlayerNode types
//
// Jason Robinson

#include "golf.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//struct golfPlayerNode {
//    Player* player;
//    struct golfPlayerNode * next;
//}

/*struct golfPlayer {
    char name[16]; // player's name (max string length is 15)
    Course course; // course on which this round is being played
    // note: for the following arrays, info about hole N will be
    // stored in array element N-1
    char * strokes[18]; // a string of strokes for each hole
    int strokeScore[18]; // score (# of strokes) for each hole
    int parScore[18]; // score (rel. to par) for each hole
};*/
struct golfCourse{
    char name[41];
    int par[18];
    int playercounter;
    PlayerNode * head;
    int God;
    PlayerNode * godhead;
    PlayerNode * wacky;
};

Player * newPlayer(const char * name, Course course){
    Player * pointer = NULL;
    pointer = (Player*) calloc(1, sizeof(Player));
    strcpy(pointer->name, name);
    pointer->course = course;
    addPlayer(pointer, course);
    return pointer;
}

int scoreHole(Player *p, int hole, const char* strokes){
    int score = 0;
    char * temp = calloc(1, sizeof(strokes));
    const int * par = courseHoles(p->course);


    strcpy(temp, strokes);

    p->strokes[hole-1] = temp;
    score = strlen(strokes);        //determine how many times the player hit the ball
    p->strokeScore[hole-1] = score;
    p->parScore[hole-1] = score - par[hole-1];          //hole score = hits - par


    return p->parScore[hole-1];
}

int totalStrokeScore(const Player *p){
    int total = 0;
    int i =0;

    while((p->strokeScore[i] != 0) && (i<18)){
        total = total + p->strokeScore[i];
        i++;
    }
    return total;
}

int totalParScore(const Player *p){
    int total = 0;
    int i =0;


    while((p->strokeScore[i] != 0) && (i<18)){
        total = total + p->parScore[i];
        i++;
    }
    return total;

}

int greensInReg(const Player *p){
    int count = 0;      //greens in reg
    int i = 0;          //holes
    int par = 0;
    char temp[11];
    char temp2;



    for(i=0;i<18;i++){
        par = p->course->par[i];
        while(par>=0){
            strcpy(temp, p->strokes[i]);
            temp2 = temp[par-3];
            if(temp2 == 'g' || temp2 == 'h'){
                count++;
                break;
            }
            par = par-1;

        }

    }


    return count;
}

void fairwaysHit(const Player *p, int *hit, int *holes){
    int i = 0;          //holes
    int par = 0;
    char temp[11];
    char temp2;
    int hit1 = 0;
    int holes1 = 0;



    for(i=0;i<18;i++){
        par = p->course->par[i];
        printf("par is %d\n", par);
        if(par==4 || par==5){
            holes1++;
            strcpy(temp, p->strokes[i]);
            temp2 = temp[0];
            if(temp2 == 'f' || temp2 == 'g' || temp2 == 'h'){
                hit1++;
            }
        }
    }

    *hit = hit1;
    *holes = holes1;



    return;
}

int countScores(const Player *p, int parScore){
    int count = 0;
    int i = 0;      //holenumber

    for(i=0;i<18;i++){
        if(p->parScore[i] == parScore){
            count++;
        }
    }
    return count;
}

typedef struct golfCourse * Course;
Course readCourse(const char * filename){


    Course first = (Course) calloc(1, sizeof(struct golfCourse));

    FILE *fp = fopen(filename, "r");
    if (fp == NULL){
        return 0;
    }

    fgets(first->name, 41, fp);

    first->name[strlen(first->name)-1]=0;       //code to get name from txt

    int j = 0;
    for(j=0; j<18; j++){
        fscanf(fp, "%d", &first->par[j]);
    }

    //read player info

    char strokes[11];
    char temporaryname[28];
    char hole1[3];
    char wholeline[28];
    char * temppointer;
    Player * pointer = NULL;
    int hole = 0;

    while(fscanf(fp, "%s", wholeline)!=EOF){
        temppointer = strtok(wholeline, ":");
        strcpy(temporaryname, temppointer);
        temppointer = strtok(NULL, ":");
        strcpy(hole1, temppointer);
        hole = atoi(hole1);
        temppointer = strtok(NULL, ":");
        strcpy(strokes, temppointer);
        if(findPlayer(temporaryname, first) != NULL){


            pointer = findPlayer(temporaryname, first);
            scoreHole(pointer, hole, strokes);

        }

        else {


            first->playercounter += 1;

            pointer = newPlayer(temporaryname, first);

            scoreHole(pointer, hole, strokes);
        }
    }
    fclose(fp);
    return first;

}
const char * courseName(Course c){
    return c->name;
}
const int * courseHoles(Course c){

    return c->par;
}
const PlayerNode * coursePlayers(Course c){
    return c->head;
}
PlayerNode * courseLeaders(Course c, int n){
    //copy into new linked list
    PlayerNode* current = c->head;    // used to iterate over the original list
    PlayerNode * stupid = c->head;
    PlayerNode* newList = NULL;    // head of the new list
    PlayerNode* tail = NULL;       // point to the last node in a new list

if(c->God<1) {
    while (current != NULL) {
        // special case for the first new node
        if (newList == NULL) {
            newList = (PlayerNode *) malloc(sizeof(PlayerNode));
            newList->player = current->player;
            newList->next = NULL;
            tail = newList;
        } else {
            tail->next = (PlayerNode *) malloc(sizeof(PlayerNode));
            tail = tail->next;
            tail->player = current->player;
            tail->next = NULL;
        }
        current = current->next;
    }
    //sorting code

    //Check whether list is empty
    if (newList == NULL) {
        return 0;
    }

    PlayerNode *temp2 = newList;

    for (int i = (c->playercounter) - 2; i >= 0; i--) {
        PlayerNode *current2 = newList;
        PlayerNode *next1 = current2->next;
        for (int counter = 0; counter <= i; counter++) {
            if (totalStrokeScore(current2->player) >
                totalStrokeScore(next1->player)) {       //if it is true it swaps the two's player data
                temp2->player = current2->player;
                current2->player = next1->player;
                next1->player = temp2->player;
                if(next1->player == NULL){
                    next1->player = newList->player;
                }
            }

            //traverse to next node
            current2 = next1;
            next1 = next1->next;
        }
    }
    c->God = 1;
    newList = newList->next;
    PlayerNode *currentp = newList;
//PlayerNode * nextp;
    PlayerNode *before;

    while (totalStrokeScore(currentp->player) < totalStrokeScore(stupid->player)) {
        before = currentp;
        currentp = currentp->next;
        // nextp = currentp->next;
    }
    before->next = stupid;
    stupid->next = currentp;
    c->godhead = newList;
}
PlayerNode * curn;
if(c->God != 1)  curn = newList;
if(c->God == 1){
    curn = c->godhead;
}
//return curn;

PlayerNode * test = curn;


    if(c->God == 1){


    }

if(n > c->playercounter){
    return curn;
}

for(int i=1; i<n;i++) test = test->next;
if(n < c->playercounter) {
    while((test->next != NULL) && (totalStrokeScore(test->player) == totalStrokeScore(test->next->player))){

        test = test->next;

    }
    if(test->next == NULL){
        c->wacky = test;
        return curn;
    }
    else{
       // c->wacky = test->next;
        test->next = NULL;
       // after = test->next;
    }
}
return curn;
return c->godhead;
}
int numPlayers(Course c){
    return c->playercounter;
}
Player * findPlayer(const char * name, Course c){


    PlayerNode * current = c->head;
    Player * find = NULL;


    int i = 0;


    while(current!=NULL){

        i = strcmp(name, current->player->name);
        if(i==0){
            find = current->player;
            return find;
        }
        current = current->next;

    }

    return NULL;

}
void addPlayer(Player *p, Course c){
    PlayerNode * before = NULL;
    PlayerNode * current = c->head;

    if(findPlayer(p->name, c)!=NULL){
       return;
    }

    PlayerNode * pn = (PlayerNode*) calloc(1, sizeof(PlayerNode));
    pn->next = NULL;
    pn->player = p;

    if(c->head == NULL){
        c->head = pn;
    }

        while(current != NULL){
            if(strcmp(pn->player->name, current->player->name)<0){
                if(before == NULL){
                    pn->next = c->head;
                    c->head = pn;
                    break;
                }
                else{
                    pn->next = current;
                    before->next = pn;
                    break;
                }
            }
            else{
                if(current->next == NULL){
                    current->next = pn;
                    break;
                }
                else{
                    before = current;
                    current = current->next;
                }
            }
        }
}
double avgTotalScore(Course c){
    PlayerNode * current = c->head;
    double total = 0;
    int i = 0;

    while(current != NULL){
        total = total + totalStrokeScore(current->player);
        current = current->next;
        i++;
    }

    total = total/i;



    return total;
}
double avgParScore(Course c){
    PlayerNode * current = c->head;
    double total = 0;
    int i = 0;

    while(current != NULL){
        total = total + totalParScore(current->player);
        i++;
        current = current->next;
    }

    total = total/i;

    return total;
}
double avgHoleScore(Course c, int hole){
    PlayerNode * current = c->head;
    double total = 0;
    int i =0;

    while(current != NULL){
        if(current->player->strokeScore[hole] != 0) {
            total = total + strlen(current->player->strokes[hole-1]);
            i++;
        }
        current = current->next;
    }

    total = total/i;

    return total;
}