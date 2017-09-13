//*************************************************************************
//*  Electronic Beach Animal
//*  Controller 001
//*  (c) 16/09/2017 - Rafael López Verdejo
//*  rlopezverdejo@gmail.com
//*
//*  This program is free software: you can redistribute it and/or modify
//*  it under the terms of the GNU General Public License as published by
//*  the Free Software Foundation, either version 3 of the License, or
//*  (at your option) any later version.
//*
//*  This program is distributed in the hope that it will be useful,
//*  but WITHOUT ANY WARRANTY; without even the implied warranty of
//*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//*  GNU General Public License for more details.
//*
//*  You should have received a copy of the GNU General Public License
//*  along with this program. If not, see <http://www.gnu.org/licenses/>.
//*
//*************************************************************************

#include <nRF24L01.h>
#include <RF24.h>
#include <RF24_config.h>
#include <SPI.h>

// Inicializamos lo necesario para la radio
RF24 radio(9,10);
const uint64_t pipe = 0xE8E8F0F0E1LL;
const int comp_X = 0;
const int comp_Y = 1;
const int comp_BOT0= 2;
int msg[3];

// Inicializamos pines
int pinX = A1; // X del joystick
int pinY = A0; // Y del joystick
int pinBOT0 = 8; // Botón 0 del joystick;

void setup() {
  
  // Configuramos e iniciamos el monitor serie
  Serial.begin(9600);

  // Configuramos la radio y la iniciamos
  radio.begin();
  radio.openWritingPipe(pipe);
  
  // Configuramos los pines
  pinMode(pinX, INPUT);
  pinMode(pinY, INPUT);
  pinMode(pinBOT0, INPUT_PULLUP);
}

void loop() {
  
  // Leemos posición del joystick
  msg[comp_X] = analogRead(pinX);
  msg[comp_Y] = analogRead(pinY);
  
  int boton0 = digitalRead(pinBOT0);
  if (boton0 == LOW) {
    //Serial.println("on");
    msg[comp_BOT0] = 1;
  }
  else if (boton0 == HIGH) {
    //Serial.println("off");
    msg[comp_BOT0] = 0;
  }
  else {
    //Serial.println("z");
    msg[comp_BOT0] = 0;
  }

  Serial.print("X: ");
  Serial.print(msg[comp_X]);
  Serial.print(" | Y: ");
  Serial.print(msg[comp_Y]);
  Serial.print(" | bot: ");
  Serial.println(msg[comp_BOT0]);
  
  // Transmitimos posición del joystick
  radio.write(msg, sizeof(msg));
}
