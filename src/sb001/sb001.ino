//*************************************************************************
//*  Electronic Beach Animal
//*  Unit 001
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
const int comp_BOT0 = 2;
int msg[3];

// Inicializamos pines
int pinMotorAIA = 3; // PWM Velocidad Motor Izquierdo
int pinMotorAIB = 4; // Dirección motor Izquierdo
int pinMotorBIA = 5; // PWM Velocidad Motor Derecho
int pinMotorBIB = 6; // Dirección motor Derecho

int pinPiezzo = 2; // Piezzo
int pinLed = 7; // Led

// Velocidad de los motores
int velocidad;

void setup() {

  // Configuramos e iniciamos el monitor serie
  Serial.begin(9600);
  
  // Configuramos la radio y la iniciamos
  radio.begin();
  radio.openReadingPipe(1,pipe);
  radio.startListening();
  
  // Configuramos los pines
  pinMode(pinMotorAIA, OUTPUT);
  pinMode(pinMotorAIB, OUTPUT);
  pinMode(pinMotorBIA, OUTPUT);
  pinMode(pinMotorBIB, OUTPUT);

  pinMode(pinPiezzo, OUTPUT);
  pinMode(pinLed, OUTPUT);

  beepFlash();
  
  // Paramos los motores
  paraMotores();
}

void loop() {

  if (radio.available())
  {
    // Recibimos la posición del joystick
    /*int done = */radio.read(msg, sizeof(msg));
            
    // Ajustamos motores
    if (msg[comp_X] >= 500) {
      giraIzquierda();
    }
    else if (msg[comp_X] <= 485) {
      giraDerecha();
    }
    else if (msg[comp_Y] <= 490) {
      // Retroceso
      velocidad = map(msg[comp_Y],0,490,0,120);
      avanzaMotores(velocidad);
    }
    else if (msg[comp_Y] >= 495) {
      // Avance
      velocidad = map(msg[comp_Y],495,1023,150,255);
      retrocedeMotores(velocidad);
    }
    else {
      // Paramos motores
      velocidad = 0;
      paraMotores();
    }
    
    Serial.print("X: ");
    Serial.print(msg[comp_X]);
    Serial.print(" | Y: ");
    Serial.print(msg[comp_Y]);
    Serial.print(" | vel: ");
    Serial.print(velocidad);
    Serial.print(" | bot: ");
    Serial.println(msg[comp_BOT0]);
  }
  else{
    // Parada de seguridad si se pierde el enlace por radio
    Serial.println("La radio no recibe nada");
    velocidad = 0;
    paraMotores();
  }
}

void beepFlash() {
  
  tone(pinPiezzo, 440, 200);
  digitalWrite(pinLed,HIGH);
  delay(200);
  digitalWrite(pinLed,LOW);
  delay(200);
  digitalWrite(pinLed,HIGH);
  delay(200);
  digitalWrite(pinLed,LOW);
  delay(200);
  digitalWrite(pinLed,HIGH);
  delay(200);
  digitalWrite(pinLed,LOW);
  tone(pinPiezzo, 1000, 200);
}

void paraMotores() {
  
  digitalWrite(pinMotorAIA, LOW);
  digitalWrite(pinMotorAIB, LOW);
  digitalWrite(pinMotorBIA, LOW);
  digitalWrite(pinMotorBIB, LOW);
}

void avanzaMotores(int velocidad) {
  
  analogWrite(pinMotorAIA, velocidad);
  digitalWrite(pinMotorAIB, HIGH);
  analogWrite(pinMotorBIA, velocidad);
  digitalWrite(pinMotorBIB, HIGH);
}

void retrocedeMotores(int velocidad) {
  
  analogWrite(pinMotorAIA, velocidad);
  digitalWrite(pinMotorAIB, LOW);
  analogWrite(pinMotorBIA, velocidad);
  digitalWrite(pinMotorBIB, LOW);
}

void giraIzquierda() {
  
  analogWrite(pinMotorAIA, 50);
  digitalWrite(pinMotorAIB, HIGH);
  analogWrite(pinMotorBIA, 200);
  digitalWrite(pinMotorBIB, LOW);
  Serial.println("Girando izquierda");
}

void giraDerecha() {
  
  analogWrite(pinMotorBIA, 50);
  digitalWrite(pinMotorBIB, HIGH);
  analogWrite(pinMotorAIA, 200);
  digitalWrite(pinMotorAIB, LOW);
  Serial.println("Girando derecha");
}
