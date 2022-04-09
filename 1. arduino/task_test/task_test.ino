#include <TaskScheduler.h>

// Callback methods prototypes
void atommain();
void t2Callback();
void t3Callback();

//Tasks (delay_ms,times,func)
Task t4();
Task t1(2000, 10, &atommain);
Task t2(3000, TASK_FOREVER, &t2Callback);
Task t3(5000, TASK_FOREVER, &t3Callback);

Scheduler runner;


void atommain() {
    Serial.print("t1: ");
    Serial.println(millis());
    
    if (t1.isFirstIteration()) {
      runner.addTask(t3);
      t3.enable();
      Serial.println("t1: enabled t3 and added to the chain");
    }
    
    if (t1.isLastIteration()) {
      t3.disable();
      runner.deleteTask(t3);
      t2.setInterval(500);
      Serial.println("t1: disable t3 and delete it from the chain. t2 interval set to 500");
    }
}

void t2Callback() {
    Serial.print("t2: ");
    Serial.println(millis());
  
}

void t3Callback() {
    Serial.print("t3: ");
    Serial.println(millis());
  
}

void setup () {
  Serial.begin(115200);
  Serial.println("Scheduler TEST");
  
  runner.init();
  Serial.println("Initialized scheduler");
  
  runner.addTask(t1);
  Serial.println("added t1");
  
  runner.addTask(t2);
  Serial.println("added t2");
  
  t1.enable();
  Serial.println("Enabled t1");
}


void loop () {
  runner.execute();
}
