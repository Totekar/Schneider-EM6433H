#include <ModbusMaster.h>

/*!
  We're using a MAX485-compatible RS485 Transceiver.
  Rx/Tx is hooked up to the hardware serial port at 'Serial'.
  The Data Enable and Receiver Enable pins are hooked up as follows:
*/
#define MAX485_DE      2
#define MAX485_RE_NEG  4

// instantiate ModbusMaster object
ModbusMaster node;

void preTransmission()
{
  digitalWrite(MAX485_RE_NEG, 1);
  digitalWrite(MAX485_DE, 1);
}

void postTransmission()
{
  digitalWrite(MAX485_RE_NEG, 0);
  digitalWrite(MAX485_DE, 0);
}

float V1,V2,V3;

void setup()
{
  pinMode(MAX485_RE_NEG, OUTPUT);
  pinMode(MAX485_DE, OUTPUT);
  // Init in receive mode
  digitalWrite(MAX485_RE_NEG, 0);
  digitalWrite(MAX485_DE, 0);

  // Modbus communication runs at 115200 baud
  Serial2.begin(9600, SERIAL_8E1);     //Even parity & 1 stop bit baud rate 19200
  Serial.begin (115200);
  // Modbus slave ID 1
  node.begin(1, Serial2);
  // Callbacks allow us to configure the RS485 transceiver correctly
  node.preTransmission(preTransmission);
  node.postTransmission(postTransmission);
}

bool state = true;

void loop()
{
  uint8_t result1,result2,result3;
  uint16_t data1[6],data2[6],data3[6], data4[6];

  // Read 16 registers starting at 0x3100)
/*
    result1 = node.readHoldingRegisters(0xA8C,2);
    delay(100);
    if (result1 == node.ku8MBSuccess){
      Serial.println(node.getResponseBuffer(0xA8C));
      Serial.println(node.getResponseBuffer(0xA8D));
    }*/
    result1 = node.readHoldingRegisters(0x0BB8,120);
    if (result1 == node.ku8MBSuccess){

      data1[0] = node.getResponseBuffer(0x0BB8);
      data1[1] = node.getResponseBuffer(0x0BB9);

      Serial.println( data1[0] );
      Serial.println( data1[1] );
      float I2 = *((float *)data1);
      Serial.println( I2 );

      data2[0] = node.getResponseBuffer(0x0BC1);
      data2[1] = node.getResponseBuffer(0x0BC2);
      float Iavg = *((float *)data2);
      Serial.println( Iavg );


      data3[0] = node.getResponseBuffer(0x0BEE);
      data3[1] = node.getResponseBuffer(0x0BEF);
      float power = *((float *)data3);
      Serial.println( power );
    
    }
  Serial.println( "......" );
  delay(1000);
}
