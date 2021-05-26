/* 
 * This file is part of the AS6221 Arduino Library.
 * Copyright (c) 2021 Michael Selinger.
 * 
 * This program is free software: you can redistribute it and/or modify  
 * it under the terms of the GNU General Public License as published by  
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but 
 * WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License 
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */
#include "AS6221.h"
#include "Arduino.h"
#include <stdio.h>
#include <Wire.h>

as6221_config_t device_conf;
uint8_t device_address;

void AS6221::init(uint8_t addr, as6221_config_t conf)
{
  Wire.begin();
  device_address = addr;
  device_conf = conf;
  updateDevice(conf);
}

double AS6221::getTemp()
{
  uint8_t rec[2] = {};
  uint8_t data[1] = {AS6221_REG_TVAL};

  writeData(data, 1);
  readData(rec, 2);

  // Calculate before comma
  uint8_t t1 = rec[0] << 1;
  t1 |= (rec[1] >> 7);
  // Calculate after comma
  uint8_t t2 = rec[1] << 1;
  t2 = t2 >> 1;
  // Add together
  double t = t2 * 0.0078125; 
  t += t1;                  
  return t;
};

void AS6221::setAlertLimits(uint16_t low, uint16_t high){
  uint8_t data[3] ={AS6221_REG_TLOW, (uint8_t)(low >> 1), (uint8_t)(low << 7)};
  writeData(data,3);
  data[0] = AS6221_REG_THIGH;
  data[1] = (uint8_t)(high >> 1);
  data[2] = (uint8_t)(high << 7);
  writeData(data,3);
}

void AS6221::sleepMode(){
  device_conf.state = AS6221_STATE_SLEEP;
  updateDevice(device_conf);
}

void AS6221::ccMode(){
  device_conf.state = AS6221_STATE_ACTIVE;
  updateDevice(device_conf);
}

void AS6221::triggerSingleShot(){
  device_conf.singleShot = true;
  updateDevice(device_conf);
  device_conf.singleShot = false;
}

void AS6221::updateDevice(as6221_config_t conf){
  uint16_t c = conf.cr | conf.alert_mode | conf.alert_polarity | conf.cf | conf.state;
  if(conf.singleShot){
    c |= AS6221_SINGLE_SHOT;
  }
  uint8_t data[3] = {AS6221_REG_CONFIG, (uint8_t)(c >> 8), (uint8_t)c};
  writeData(data, 3);
}

void AS6221::writeData(uint8_t* data, uint8_t len) {
  Wire.beginTransmission(device_address);
  Wire.write(data, len);
  Wire.endTransmission();
}

void AS6221::readData(uint8_t *rec, uint8_t len) {
  Wire.requestFrom(device_address, len);
  uint8_t i = 0;
  while (Wire.available() && i < len) {
    rec[i] = Wire.read();
    i++;
  };
}
