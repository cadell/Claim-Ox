/*
 * Claim 0x 
 * Determines if car crash happens and reports data at the of the accident
 * 
 */

#include <Wire.h>

long accelX, accelY, accelZ;
float gForceX, gForceY, gForceZ;
int trigger = 0;



float maxGx, maxGy1, maxGz1;
float maxGy2, maxGz2;

long gyroX, gyroY, gyroZ;
float rotX, rotY, rotZ;

void setup() {
  
  Serial.begin(9600);
  Wire.begin();
  setupMPU();
  pinMode(12, OUTPUT );
}


void loop() {
  while (trigger == 0)
  {
    digitalWrite(12, HIGH);
    recordAccelRegisters();
    recordGyroRegisters();
    
    printData();
    maxAccel();
    delay(100);
    digitalWrite(12, LOW);
    
  }
}

void setupMPU(){
  Wire.beginTransmission(0b1101000); //This is the I2C address of the MPU (b1101000/b1101001 for AC0 low/high datasheet sec. 9.2)
  Wire.write(0x6B); //Accessing the register 6B - Power Management (Sec. 4.28)
  Wire.write(0b00000000); //Setting SLEEP register to 0. (Required; see Note on p. 9)
  Wire.endTransmission();  
  Wire.beginTransmission(0b1101000); //I2C address of the MPU
  Wire.write(0x1B); //Accessing the register 1B - Gyroscope Configuration (Sec. 4.4) 
  Wire.write(0x00000000); //Setting the gyro to full scale +/- 250deg./s 
  Wire.endTransmission(); 
  Wire.beginTransmission(0b1101000); //I2C address of the MPU
  Wire.write(0x1C); //Accessing the register 1C - Acccelerometer Configuration (Sec. 4.5) 
  Wire.write(0b00000000); //Setting the accel to +/- 2g
  Wire.endTransmission(); 
}

void recordAccelRegisters() {
  Wire.beginTransmission(0b1101000); //I2C address of the MPU
  Wire.write(0x3B); //Starting register for Accel Readings
  Wire.endTransmission();
  Wire.requestFrom(0b1101000,6); //Request Accel Registers (3B - 40)
  while(Wire.available() < 6);
  accelX = Wire.read()<<8|Wire.read(); //Store first two bytes into accelX
  accelY = Wire.read()<<8|Wire.read(); //Store middle two bytes into accelY
  accelZ = Wire.read()<<8|Wire.read(); //Store last two bytes into accelZ
  processAccelData();

}

void processAccelData(){
  gForceX = accelX / 16384.0;
  gForceY = accelY / 16384.0; 
  gForceZ = accelZ / 16384.0;

}



void maxAccel(){
  if(maxGy1 < gForceY)
    {maxGy1 = gForceY;}
  if(maxGz1 < gForceZ)
    {maxGz1 = gForceZ;}
  if(maxGy2 > gForceY)
    {maxGy2 = gForceY;}
  if(maxGz2 < gForceZ)
    {maxGz2 = gForceZ;}
    
    Serial.print("Y-Max: ");
    Serial.print(maxGy1);
    Serial.print("  Z-Max: ");
    Serial.print(maxGz1);
    Serial.println(" ");

   if (maxGy1 >= 1.75 || maxGz1>= 1.75 || maxGy2 <= -1.75 || maxGz2 <= -1.75 )
   {
    Serial.println(" ");
    Serial.println("------------- ACCIDENT DETERMINED ---------------- ");
    Serial.println(" ");
    if (maxGy1 >= 1.75)
    {Serial.println("Your car was hit from behind" );}
    if (maxGy2 <= -1.75)
    {Serial.println("You have a hit a vehicle or object!");}
    if (maxGz1 >= 1.75)
    {Serial.println("Your car was hit from the right!");}
    if (maxGz2 <= -1.75)
    {Serial.println("Your car was hit from the left!");}

    //Serial.print("Date of accident: " + month() + " / " + day() + " / " + year());
    //Serial.println("Time of accident: " + hour() + " : " + minute() + " : " + second());
 
   // Serial.close();
    trigger = 1 ; 
    Serial.end();
    }
  }

void recordGyroRegisters() {
  Wire.beginTransmission(0b1101000); //I2C address of the MPU
  Wire.write(0x43); //Starting register for Gyro Readings
  Wire.endTransmission();
  Wire.requestFrom(0b1101000,6); //Request Gyro Registers (43 - 48)
  while(Wire.available() < 6);
  gyroX = Wire.read()<<8|Wire.read(); //Store first two bytes into accelX
  gyroY = Wire.read()<<8|Wire.read(); //Store middle two bytes into accelY
  gyroZ = Wire.read()<<8|Wire.read(); //Store last two bytes into accelZ
  processGyroData();
}

void processGyroData() {
  rotX = gyroX / 131.0;
  rotY = gyroY / 131.0; 
  rotZ = gyroZ / 131.0;
}

void printData() {
  Serial.print("Gyro (deg)");
  Serial.print(" X=");
  Serial.print(rotX);
  Serial.print(" Y=");
  Serial.print(rotY);
  Serial.print(" Z=");
  Serial.print(rotZ);
  Serial.print(" Accel (g)");
  Serial.print(" X=");
  Serial.print(gForceX);
  Serial.print(" Y=");
  Serial.print(gForceY);
  Serial.print(" Z=");
  Serial.println(gForceZ);
  
  Serial.print("Max acceleration: ");

  //Serial.print(maxGx);
}
