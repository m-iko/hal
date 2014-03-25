#include <SoftwareSerial.h>

SoftwareSerial com(3, 4);

#define ANIM_LEN 128

struct Animation {
    byte pin;
    unsigned int interval, last_time;
    byte len, frame;
    byte curve[ANIM_LEN];
};

#define N_ANIMS sizeof(anims)/sizeof(Animation)
Animation anims[] = {{0}, {1}};


byte i, j;
unsigned int now;
void setup(){
    com.begin(57600);

    for (i=0; i<N_ANIMS; i++){
        anims[i].interval = 16;
        anims[i].last_time = 0;
        anims[i].len = ANIM_LEN;
        anims[i].frame = 0;
        for (j=0; j<ANIM_LEN; j++)
            anims[i].curve[j] = j;
    }
}

#define waitSerial() while (! com.available())
void readSerial(){
    if (! com.available())
        return;

    switch (com.read()){
        case '#':
            waitSerial();
            i = com.read(); /* i: animation id */
            waitSerial();
            j = com.read(); /* j: fps */
            if (i >= N_ANIMS || j == 0)
                com.write('!');
            else {
                anims[i].interval = 1000;
                anims[i].interval /= j;
                com.write('#');
            }
            break;

        case 'U':
            waitSerial();
            i = com.read(); /* i: animation id */
            waitSerial();
            j = com.read(); /* j: length */
            if (i >= N_ANIMS || j > ANIM_LEN){
                while (j>0){
                    waitSerial();
                    com.read();
                    j--;
                }
                com.write('!');
            } else {
                anims[i].len = j;
                for (j=0; j<anims[i].len; j++){
                    waitSerial();
                    anims[i].curve[j] = com.read();
                }
                com.write('U');
            }
    }
}

void loop(){
    readSerial();

    now = millis();
    for (i=0; i<N_ANIMS; i++){
        if (now - anims[i].last_time > anims[i].interval){
            anims[i].frame = (anims[i].frame+1)%anims[i].len;
            anims[i].last_time = now;
            analogWrite(anims[i].pin, anims[i].curve[anims[i].frame]);
        }
    }
}
