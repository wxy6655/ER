/******************************************
   基于Arduino 328控制板+Speaker
   播放儿歌《小星星》和《国际歌》背景音乐
   TIDY BY YFROBOT
 *****************************************/

int song[] = {

  /* 儿歌《小星星》*/
  277, 277, 415, 415, 466, 466, 415,
  370, 370, 330, 330, 311, 311, 277,
  415, 415, 370, 370, 330, 330, 311,
  415, 415, 370, 370, 330, 330, 311,
  277, 277, 415, 415, 466, 466, 415,
  370, 370, 330, 330, 311, 311, 277,

  /*《国际歌》*/
  370, 494, 466, 554, 494, 370, 311, 415, 330,
  415, 554, 494, 466, 415, 370, 330, 311,
  370, 494, 466, 554, 494, 370, 311, 415, 330,
  415, 554, 494, 466, 554, 659, 494,
  622, 554, 466, 415, 466, 494, 415, 466, 370,
  370, 330, 370, 415, 415, 554, 494, 466,
  554, 554, 466, 370, 370, 330, 370, 622, 494,
  415, 466, 494, 466, 554, 494, 415, 370,
  622, 554, 494, 370, 311, 415, 330,
  554, 494, 466, 415, 370,
  370, 622, 554, 370, 494, 466,
  466, 415, 415, 415, 554, 554,
  622, 554, 494, 370, 311, 415, 330, 330,
  554, 494, 466, 415, 370, 622, 311, 622,
  740, 659, 622, 554, 622, 659,
  659, 622, 622, 554, 554, 494,

};

int noteDurations[] = {

  /* 儿歌《小星星》*/
  2, 2, 2, 2, 2, 2, 1,
  2, 2, 2, 2, 2, 2, 1,
  2, 2, 2, 2, 2, 2, 1,
  2, 2, 2, 2, 2, 2, 1,
  2, 2, 2, 2, 2, 2, 1,
  2, 2, 2, 2, 2, 2, 1,

  /*《国际歌》*/
  1, 1, 2, 2, 2, 2, 2, 1, 2,
  2, 1, 2, 2, 2, 2, 2, 1,
  1, 1, 2, 2, 2, 2, 2, 1, 2,
  2, 2, 2, 1, 1, 1, 1, 2,
  2, 2, 1, 2, 2, 2, 2, 1, 2,
  2, 2, 2, 2, 1, 1, 1, 2,
  2, 1, 2, 2, 2, 2, 2, 1, 2,
  2, 2, 2, 1, 1, 2, 1, 2,
  2, 2, 1, 1, 2, 1, 2,
  2, 2, 1, 1, 2, 2,
  1, 1, 1, 1, 1, 1,
  2, 1, 2, 1, 2, 1, 2,
  2, 2, 1, 1, 2, 1, 2, 2,
  2, 2, 1, 1, 1, 1, 2, 1,
  1, 1, 1, 1, 2, 2,
  1, 1, 2, 1, 2, 1,

};

int buzzerPin = 9;

void setup() {
  pinMode(buzzerPin, OUTPUT);
  
  sing(); 
}

void loop() {
  // do nothing
}

void sing() {
  for (int thisNote = 0; thisNote < 154; thisNote++) {
    int noteDuration = 1000 / noteDurations[thisNote];
    // 计算每个节拍的时间，以一个节拍一秒为例
    // 四分之一拍就是1000/4毫秒，八分之一拍就是1000/8毫秒
    tone(buzzerPin, song[thisNote], noteDuration);
    int pauseBetweenNotes = noteDuration * 1.30;
    // 每个音符间的停顿间隔，以该音符的130%为佳
    delay(pauseBetweenNotes);
    noTone(buzzerPin);
  }
}
