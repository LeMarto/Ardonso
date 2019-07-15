/*
* Gotas
*/

/***************************
*
* Parametros
*
***************************/

/*Configuracion de PINs*/
#define pin_disparador 7
#define pin_valvula 10
#define pin_camara 8
#define pin_potenciometro A1

#define duracion_gota 20 // cuanto tiempo esta la valvula abierta por gota
#define cantidad_gotas 2
#define delay_entre_gotas 250 //cuanto se espera entre gota y gota

/*Parametros del potenciometro*/
#define min_delay_potenciometro 1  
#define max_delay_potenciometro 5000
#define potenciometro_magic_number 1023.0

/*Misc Params*/
#define duracion_delay 100
#define debug true

/*Variables Globales*/
int valor_potenciometro;
int delay_potenciometro;

bool valvula_abierta;

/*Inicializacion*/

void setup()
{
  valor_potenciometro = 0;
  delay_potenciometro = 0;

  pinMode(pin_disparador, INPUT_PULLUP); 
  
  if (debug)
    Serial.begin(9600);

  valvula_abierta = false;
  pinMode(pin_camara, OUTPUT);
  pinMode(pin_valvula, OUTPUT);
}

bool disparador_presionado(){
  int val = digitalRead(pin_disparador);
  if (val == LOW)
    return true;
  else if (val == HIGH)
    return false;
}

/*Funciones para disparar la camara*/
void disparar_camara(){
  digitalWrite(pin_camara, HIGH);     //Enciendo Salida
  delay(100);
  digitalWrite(pin_camara, LOW);     //Apago Salida
}

/*Funciones de Valvula*/
void abrir_valvula(){
  if (valvula_abierta)
    return;
  digitalWrite(pin_valvula, HIGH);
  valvula_abierta = true;
}

void cerrar_valvula(){
  if (!valvula_abierta)
    return;
  digitalWrite(pin_valvula, LOW);
  valvula_abierta = false;
}

/*Funciones del Potenciometro*/
bool cambio_estado_potenciometro(){
  int valor = analogRead(pin_potenciometro);
  if (valor == valor_potenciometro || abs(valor - valor_potenciometro) < 50 )
    return false;
  valor_potenciometro = valor;
  return true;
}

void actualizar_delay_potenciometro(){
  int diff = max_delay_potenciometro - min_delay_potenciometro;
  float multiplier = valor_potenciometro / potenciometro_magic_number;
  delay_potenciometro = min_delay_potenciometro + (diff * multiplier);
}

void loop()
{
  if (!disparador_presionado())
    return;

  if (cambio_estado_potenciometro())
    actualizar_delay_potenciometro();

  for(int i = 0; i < cantidad_gotas; i++)
  {
    abrir_valvula();
    delay(duracion_gota);
    cerrar_valvula();
    
    if (cantidad_gotas > 1)
      delay(delay_entre_gotas);
  }
  delay(delay_potenciometro);
  disparar_camara();
}
