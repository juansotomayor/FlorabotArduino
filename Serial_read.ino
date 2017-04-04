void serialEvent() {  
  int valores = 0;
  while (Serial.available() > 0) {// get the new byte:    
    char inChar = (char)Serial.read(); 
    if(inChar == '*' ){
      mensaje++;
    }else  if (inChar == '-') {
      stringComplete = true;
      mensaje = 0;  
      valores = mensaje;  
      break;
    }else{
      if(inChar != ' '){
        Msj[mensaje] += inChar;
      }
    }
  }
  
  if (stringComplete) {   
    /*Serial.print('(');
    Serial.print(Msj[0]);        
    Serial.print(',');    
    Serial.print(Msj[1]);
    Serial.print(',');    
    Serial.print(Msj[2]);
    Serial.print(")"); */
    stringComplete = false;     
    if(Msj[1] == "Blooming"){
      if(Msj[2] == "HIGH"){
        digitalWrite(led_crecimiento , HIGH);
        digitalWrite(led_floracion , HIGH);
      }else if(Msj[2] == "LOW"){
        digitalWrite(led_floracion , LOW);
        digitalWrite(led_crecimiento , LOW);
      }
    }else if(Msj[1] == "Growth"){
      if(Msj[2] == "HIGH"){
        digitalWrite(led_floracion , LOW);
        digitalWrite(led_crecimiento , HIGH);
      }else if(Msj[2] == "LOW"){
        digitalWrite(led_floracion , LOW);
        digitalWrite(led_crecimiento , LOW);
      }
      
    }
    if(Msj[1] == "Ventilacion"){
      if(Msj[2] == "HIGH"){
        ventilador_derecho = true;
        ventilador_izquierdo = true;
      }else if(Msj[2] == "LOW"){
        ventilador_derecho = false;
        ventilador_izquierdo = false;
      }
      digitalWrite(intractor_derecho , ventilador_derecho);
      digitalWrite(intractor_izquierdo , ventilador_izquierdo);
    }
    if(Msj[1] == "Door"){
      if(Msj[2] == "OFF"){
        estado_puerta = 90;
        Orden_Puerta = true;
        tiempo_apertura_puerta = millis();
      }else if(Msj[2] == "ON"){
        estado_puerta = 35;
      }
      door.write(estado_puerta);           
    }

    for(int i=0; i<6; i++){
      Msj[i] = "";
    }      
  }
}
