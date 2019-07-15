/*
* Laser
*/

/***************************
*
* Parametros
*
***************************/


/*Configuracion de PINs*/
#define pin_disparador 7
#define pin_detector A3
#define pin_laser 5
#define pin_camara 8


/*Parametros del potenciometro*/
#define pin_potenciometro A1
#define min_delay_potenciometro 1  
#define max_delay_potenciometro 5000
#define potenciometro_magic_number 1023.0

/*Misc Params*/
#define duracion_delay 100

/*Variables Globales*/
bool laser_encendido;
int valor_laser;
int valor_potenciometro;
int delay_potenciometro;

void setup()
{
  valor_potenciometro = 0;
  delay_potenciometro = 0;

  pinMode(pin_disparador, INPUT_PULLUP); 
  
  laser_encendido = false;
  valor_laser = 1;
  pinMode(pin_camara, OUTPUT);
  pinMode(pin_laser, OUTPUT);
  pinMode(pin_detector, INPUT); 
}

/*Funciones de Laser*/
void encender_laser(){
  if (laser_encendido)
    return;
  digitalWrite(pin_laser, HIGH);
  laser_encendido = true;
}

void apagar_laser(){
  if (!laser_encendido)
    return;
  digitalWrite(pin_laser, LOW);
  laser_encendido = false;
}

bool cambio_estado_laser(){
  int valor = digitalRead(pin_detector);
  if (valor == valor_laser)
    return false;
  valor_laser = valor;
  return true;
}

bool laser_interrumpido(){
  if (cambio_estado_laser() && valor_laser == 0)
    return true;
  return false;
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
  encender_laser();

  if (cambio_estado_potenciometro())
    actualizar_delay_potenciometro();

  if (laser_interrumpido())  {
    apagar_laser();
    delay(delay_potenciometro);
    disparar_camara();
    delay(duracion_delay);
    encender_laser();
  }
}
