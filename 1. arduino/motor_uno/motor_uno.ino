int input1 = 5; // 定义uno的pin 5 向 input1 输出 
int input2 = 6; // 定义uno的pin 6 向 input2 输出
int input3 = 9; // 定义uno的pin 9 向 input3 输出
int input4 = 10; // 定义uno的pin 10 向 input4 输出
int HE1 = 8; //定义uno的pin2 为霍尔传感器输入
int sum = 0;
 
void setup() {
 Serial.begin (115200);
//初始化各IO,模式为OUTPUT 输出模式
pinMode(input1,OUTPUT);
pinMode(input2,OUTPUT);
pinMode(input3,OUTPUT);
pinMode(input4,OUTPUT);
pinMode(HE1,INPUT);
 }
 
void loop() {
    Serial.println(digitalRead(HE1));
    // SG_back();
    SG_forward();
    delay(10);
    // Serial.println(millis());
    int vol = digitalRead(HE1);
    if(vol == 0)
    {   
        SG_stop();
        delay(500);
    }
 
    
}

void SG_back()

{
  digitalWrite(input1,HIGH); //给高电平
  digitalWrite(input2,LOW);  //给低电平
  
}

void SG_forward()
{
  digitalWrite(input1,LOW);
  digitalWrite(input2,HIGH);  
}

void SG_stop()
{
 digitalWrite(input1,LOW);
 digitalWrite(input2,LOW);  
}

void PT_forward()

{
  digitalWrite(input3,HIGH); //给高电平
  digitalWrite(input4,LOW);  //给低电平
  
}

void PT_back()
{ 
  digitalWrite(input3,LOW);
  digitalWrite(input4,HIGH);  
}

void PT_stop()
{
 digitalWrite(input3,LOW);
 digitalWrite(input4,LOW);  
}