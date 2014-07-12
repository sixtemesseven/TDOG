void AntT_init()
{
  pinMode(headingLock_switch, INPUT_PULLUP); 
  pinMode(tiltServo_pin , OUTPUT);
  pinMode(tiltServo_pin , OUTPUT);
  panServo.attach(panServo_pin);
  tiltServo.attach(tiltServo_pin);
}

void AntT_Configure()
{
        uint8_t x;
	uint16_t counter = millis();
	while (digitalRead(headingLock_switch) == LOW)
	{}
	counter = millis() - counter;

        //RSSI calibration
	if (counter > 4000)
	{
		uint16_t videoRSSI_1_average   =0;  
		uint16_t videoRSSI_2_average   =0;
                counter = 0;

		for(x = 0; x <= 20; x++);
		{
			digitalWrite(videoSwitch_1, HIGH);
			digitalWrite(videoSwitch_2, LOW);
			delay(30);
			videoRSSI_1_average = videoRSSI_1_average + analogRead(videoRSSI_1);
			videoRSSI_2_average = videoRSSI_2_average + analogRead(videoRSSI_2);
			digitalWrite(videoSwitch_2, HIGH);
			digitalWrite(videoSwitch_1, LOW);
			delay(30);
			videoRSSI_1_average = videoRSSI_1_average + analogRead(videoRSSI_1);
			videoRSSI_2_average = videoRSSI_2_average + analogRead(videoRSSI_2);
                      						
		}
		videoRSSI_1_offset = videoRSSI_1_average / 40;
		videoRSSI_2_offset = videoRSSI_2_average / 40;

                EEPROM.write(videoRSSI_1_offset_ADDR,(videoRSSI_1_offset/2));
                EEPROM.write(videoRSSI_2_offset_ADDR,(videoRSSI_2_offset/2));
		digitalWrite(videoSwitch_1, HIGH);
		digitalWrite(videoSwitch_2, HIGH);
		delay(1000);
		digitalWrite(videoSwitch_1, LOW);
		digitalWrite(videoSwitch_2, LOW);
		counter = 0;
	}
        //Setting antenna pos. at PWM 1500 (usfull for <360° servos...)
	if (1000 < counter)
	{        
                counter = 0;
                tiltServo.writeMicroseconds(tiltServo_neutral);
                panServo.writeMicroseconds(panServo_neutral);                
		counter = millis();
		
                for(x = 0; x < 10; x++);
		{
			digitalWrite(videoSwitch_1, HIGH);
			digitalWrite(videoSwitch_2, LOW);
			delay(500);
			digitalWrite(videoSwitch_2, HIGH);
			digitalWrite(videoSwitch_1, LOW);
			delay(500);
		}
		headingLock_zero = osd_heading;
                EEPROM.write(headingLock_zero_ADDR,(headingLock_zero/2));               
		digitalWrite(videoSwitch_1, HIGH);
		digitalWrite(videoSwitch_2, HIGH);
		delay(1000);
		digitalWrite(videoSwitch_1, LOW);
		digitalWrite(videoSwitch_2, LOW);
		counter = 0;
	}
}	
	

void AntT_update()
{ 
  //PAN-----------------------------------------------------------------------------

 /* debugg, to be deleted asap
 osd_lat  = 47.568977;
 osd_lon  = 8.838192;
 osd_home_lat  = 47.568507;
 osd_home_lon  = 8.839298;
 headingLock_zero = 0;
 
 osd_home_alt = 0.11;
 osd_alt = 50.123;
 osd_home_distance = 50.11;
 */
 
  //calculating the direction as seen from home pos
  int16_t vector_lat = (float(osd_lat - osd_home_lat)*10000);
  int16_t vector_lon = (float(osd_lon - osd_home_lon)*10000); 
  int16_t uavDirection = 57*(acos(float(vector_lat)/float((sqrt(pow(vector_lat,2)+pow(vector_lon,2))))));
  
  if(vector_lon < 0)
  {
    uavDirection = 360-uavDirection;
  }
   
  int16_t uavDirection_corrected = (uavDirection-headingLock_zero);  
  if((uavDirection-headingLock_zero)<=0)
  {
    uavDirection_corrected = 360+(uavDirection-headingLock_zero); //plus negative
  }
     
  //calculating the servo output based on the zeroHeading --> position of tracker (deg on compass rose) at neutral position 
  uint16_t panPWM = (panServo_neutral - ((360-uavDirection_corrected)*panServo_PWMdeg));
  if(uavDirection_corrected < 180)
  {
    panPWM=(panServo_neutral + (uavDirection_corrected*panServo_PWMdeg));
  }
  
  //servo protection....
  if(panPWM < panServo_min)
  {
    panPWM = panServo_min;
  }
  if(panPWM > panServo_max)
  {
    panPWM = panServo_max;
  }

  //TILT------------------------------------------------------------------------------------
  //trig
 
  float tiltAngle = 57*(atan((osd_alt-osd_home_alt)/osd_home_distance));
  uint16_t tiltPWM = tiltServo_neutral + (tiltAngle* tiltServo_PWMdeg );
  
  //servo protection....
  if(tiltPWM < tiltServo_min)
  {
    tiltPWM = tiltServo_min;
  }
  if(tiltPWM > tiltServo_max)
  {
    tiltPWM = tiltServo_max;
  } 
  tiltServo.writeMicroseconds(tiltPWM);
  panServo.writeMicroseconds(panPWM);
}

void update_diversity()
{
  if ((analogRead(videoRSSI_1) - videoRSSI_1_offset)*1.1 >= (analogRead(videoRSSI_2) - videoRSSI_2_offset))
  {
	  digitalWrite(videoSwitch_2, LOW);
	  digitalWrite(videoSwitch_1, HIGH);
  }
  if ((analogRead(videoRSSI_1) - videoRSSI_1_offset)*1.1 < (analogRead(videoRSSI_2) - videoRSSI_2_offset))
  {
	  digitalWrite(videoSwitch_1, LOW);
	  digitalWrite(videoSwitch_2, HIGH);
  } 
}

