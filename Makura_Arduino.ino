// MPU-6050 Accelerometer + Gyro
  #include <Wire.h>
  #include <XBee.h>
 
  #define MPU6050_ACCEL_XOUT_H       0x3B   // R
  #define MPU6050_WHO_AM_I           0x75   // R
  #define MPU6050_PWR_MGMT_1         0x6B   // R/W
  #define MPU6050_I2C_ADDRESS 0x68
 
typedef union accel_t_gyro_union{
  struct{
    uint8_t x_accel_h;
    uint8_t x_accel_l;
    uint8_t y_accel_h;
    uint8_t y_accel_l;
    uint8_t z_accel_h;
    uint8_t z_accel_l;
    uint8_t t_h;
    uint8_t t_l;
    uint8_t x_gyro_h;
    uint8_t x_gyro_l;
    uint8_t y_gyro_h;
    uint8_t y_gyro_l;
    uint8_t z_gyro_h;
    uint8_t z_gyro_l;
  }
  reg;
  struct{
    int16_t x_accel;
    int16_t y_accel;
    int16_t z_accel;
    int16_t temperature;
    int16_t x_gyro;
    int16_t y_gyro;
    int16_t z_gyro;
  }
  value;
};

  float f_acc_x;
  float f_acc_y;
  float f_acc_z;
  float f_acc_angle_x;
  float f_acc_angle_y;
  float f_acc_angle_z;
  float f_a0;
  float f_a1;
  float f_a2;
  float f_a3;
/*
  XBee xbee = XBee();
  uint8_t payload[] = {0,0};
  XBeeAddress64 addr64 = XBeeAddress64(0x13A200, 0x405D35D7);
  ZBTxRequest zbTx = ZBTxRequest(addr64, payload, sizeof(payload));
 //ZBTxStatusResponse txStatus = ZBTxStatusResponse();
 */
 int f=0;
void setup(){
  
  Wire.begin();
  int error;
  uint8_t c;
  Serial.begin(9600);
  //8番ピンをデジタル出力
  pinMode(8,OUTPUT);
  //9番ピンをデジタル入力
  pinMode(9,INPUT);
 // xbee.setSerial(Serial);
  //Serial.print("InvenSense MPU-6050");
  //Serial.print("June 2012");
  error = MPU6050_read (MPU6050_WHO_AM_I, &c, 1);
  //Serial.print("WHO_AM_I : ");
  //Serial.print(c,HEX);
  //Serial.print(", error = ");
  //Serial.println(error,DEC);
  error = MPU6050_read (MPU6050_PWR_MGMT_1, &c, 1);
  //Serial.print("PWR_MGMT_1 : ");
  //Serial.print(c,HEX);
  //Serial.print(", error = ");
  //Serial.println(error,DEC);
  MPU6050_write_reg (MPU6050_PWR_MGMT_1, 0);

  accel_t_gyro_union accel_t_gyro;
  error = MPU6050_read (MPU6050_ACCEL_XOUT_H, (uint8_t *) &accel_t_gyro, sizeof(accel_t_gyro));
   uint8_t swap;
# define SWAP(x,y) swap = x; x = y; y = swap
  SWAP (accel_t_gyro.reg.x_accel_h, accel_t_gyro.reg.x_accel_l);
  SWAP (accel_t_gyro.reg.y_accel_h, accel_t_gyro.reg.y_accel_l);
  SWAP (accel_t_gyro.reg.z_accel_h, accel_t_gyro.reg.z_accel_l);
  SWAP (accel_t_gyro.reg.t_h, accel_t_gyro.reg.t_l);
  SWAP (accel_t_gyro.reg.x_gyro_h, accel_t_gyro.reg.x_gyro_l);
  SWAP (accel_t_gyro.reg.y_gyro_h, accel_t_gyro.reg.y_gyro_l);
  SWAP (accel_t_gyro.reg.z_gyro_h, accel_t_gyro.reg.z_gyro_l);

  //加速度初期値
  f_acc_x = accel_t_gyro.value.x_accel / 16384.0;
  f_acc_y = accel_t_gyro.value.y_accel / 16384.0;
  f_acc_z = accel_t_gyro.value.z_accel / 16384.0;

  //傾斜角度初期値
  f_acc_angle_x = atan2(f_acc_x, f_acc_z) * 360 / 2.0 / PI;
  f_acc_angle_y = atan2(f_acc_y, f_acc_z) * 360 / 2.0 / PI;
  f_acc_angle_z = atan2(f_acc_x, f_acc_y) * 360 / 2.0 / PI;

  //電圧初期値
  int i = analogRead(0);
  int i2 = analogRead(1);
  int i3 = analogRead(2);
  int i4 = analogRead(3);
  f_a0 = i * 5.0 / 1023.0;
  f_a1 = i2 * 5.0 / 1023.0;
  f_a2 = i3 * 5.0 / 1023.0;
  f_a3 = i4 * 5.0 / 1023.0;
}

/*
void set_float_to_payload(float value, int index) {
  uint8_t *value_array;
  value_array = reinterpret_cast<uint8_t*>(&value);
  for(int i=0; i<sizeof(value); i++){
    payload[i+index] = value_array[i];
  }
}
*/

  void Send(){
  //int counter = 0;
  //おまじない
  int error;
  accel_t_gyro_union accel_t_gyro;
  error = MPU6050_read (MPU6050_ACCEL_XOUT_H, (uint8_t *) &accel_t_gyro, sizeof(accel_t_gyro));
  //Serial.print(error,DEC);
  //Serial.print("\t");
  uint8_t swap;
  #define SWAP(x,y) swap = x; x = y; y = swap
  SWAP (accel_t_gyro.reg.x_accel_h, accel_t_gyro.reg.x_accel_l);
  SWAP (accel_t_gyro.reg.y_accel_h, accel_t_gyro.reg.y_accel_l);
  SWAP (accel_t_gyro.reg.z_accel_h, accel_t_gyro.reg.z_accel_l);
  SWAP (accel_t_gyro.reg.t_h, accel_t_gyro.reg.t_l);
  SWAP (accel_t_gyro.reg.x_gyro_h, accel_t_gyro.reg.x_gyro_l);
  SWAP (accel_t_gyro.reg.y_gyro_h, accel_t_gyro.reg.y_gyro_l);
  SWAP (accel_t_gyro.reg.z_gyro_h, accel_t_gyro.reg.z_gyro_l);

  //リアルタイム加速度
  float acc_x = accel_t_gyro.value.x_accel / 16384.0;
  float acc_y = accel_t_gyro.value.y_accel / 16384.0;
  float acc_z = accel_t_gyro.value.z_accel / 16384.0;
  //初期値との差分
  float diff_acc_x = abs(f_acc_x - acc_x);
  float diff_acc_y = abs(f_acc_y - acc_y);
  float diff_acc_z = abs(f_acc_z - acc_z);
    
  //リアルタイム傾斜角度
  float acc_angle_x = atan2(acc_x, acc_z) * 360 / 2.0 / PI;
  float acc_angle_y = atan2(acc_y, acc_z) * 360 / 2.0 / PI;
  float acc_angle_z = atan2(acc_x, acc_y) * 360 / 2.0 / PI;
  //初期値との差分
  float diff_acc_angle_x = abs(f_acc_angle_x - acc_angle_x);
  float diff_acc_angle_y = abs(f_acc_angle_y - acc_angle_y);
  float diff_acc_angle_z = abs(f_acc_angle_z - acc_angle_z);
  //リアルタイム電圧
  int i = analogRead(0);
  int i2 = analogRead(1);
  int i3 = analogRead(2);
  int i4 = analogRead(3);
  float a0 = i * 5.0 / 1023.0;
  float a1 = i2 * 5.0 / 1023.0;
  float a2 = i3 * 5.0 / 1023.0;
  float a3 = i4 * 5.0 / 1023.0;
  //初期値との差分
  float diff_a0 = abs(f_a0 - a0);
  float diff_a1 = abs(f_a1 - a1);
  float diff_a2 = abs(f_a2 - a2);
  float diff_a3 = abs(f_a3 - a3);

  //静電容量変化量の変数を用意
 int a=0;
  //8番ピンをHIGHで出力
  digitalWrite(8, HIGH);
  //指が触れたとき9番ピンがHIGHになるまでをカウント
  while (digitalRead(9)!=HIGH){
    //カウントする
    a++;
  }
  delay(1);
  //8番ピンをLOWにする
  digitalWrite(8, LOW);
  f+=(a-f)/2;
  
 

  //データ送信
    Serial.print(diff_acc_x);
    Serial.print("\t");
    Serial.print(diff_acc_y);
    Serial.print("\t");
    Serial.print(diff_acc_z);
    Serial.print("\t");
    Serial.print(diff_acc_angle_x);
    Serial.print("\t");
    Serial.print(diff_acc_angle_y);
    Serial.print("\t");
    Serial.print(diff_acc_angle_z);
    Serial.print("\t");
    Serial.print(diff_a0);
    Serial.print("\t");
    Serial.print(diff_a1);
    Serial.print("\t");
    Serial.print(diff_a2);
    Serial.print("\t"); 
    Serial.print(diff_a3);
    Serial.print("\t");
    Serial.print(a);
    Serial.println("");
    delay(1000);
  }

  void setLed()
  {
    if ( Serial.available() ) {
      char mode = Serial.read();
      switch (mode) {
        case '0' : digitalWrite(13, LOW);  break;
        case '1' : digitalWrite(13, HIGH); break;
      }
    }
  } 
 
  void loop(){
  Send();
  setLed();
  
}
 
// MPU6050_read
int MPU6050_read(int start, uint8_t *buffer, int size){
  int i, n, error;
  Wire.beginTransmission(MPU6050_I2C_ADDRESS);
  n = Wire.write(start);
  if (n != 1)
    return (-10);
  n = Wire.endTransmission(false);   // hold the I2C-bus
  if (n != 0)
    return (n);
  // Third parameter is true: relase I2C-bus after data is read.
  Wire.requestFrom(MPU6050_I2C_ADDRESS, size, true);
  i = 0;
  while(Wire.available() && i<size){
    buffer[i++]=Wire.read();
  }
  if ( i != size)
    return (-11);
  return (0);  // return : no error
}
 
// MPU6050_write
int MPU6050_write(int start, const uint8_t *pData, int size){
  int n, error;
  Wire.beginTransmission(MPU6050_I2C_ADDRESS);
  n = Wire.write(start);        // write the start address
  if (n != 1)
    return (-20);
  n = Wire.write(pData, size);  // write data bytes
  if (n != size)
    return (-21);
  error = Wire.endTransmission(true); // release the I2C-bus
  if (error != 0)
    return (error);
  return (0);         // return : no error
}
 
// MPU6050_write_reg
int MPU6050_write_reg(int reg, uint8_t data){
  int error;
  error = MPU6050_write(reg, &data, 1);
  return (error);
}
