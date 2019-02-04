/*
* ARDONSO
*/

/***************************
*
* Parametros
*
***************************/

/*Parametros del laser*/
#define pin_detector A3
#define pin_laser 5

/*Parametros del disparador*/
#define pin_disparador 8

/*Parametros del potenciometro*/
#define pin_potenciometro A1
#define min_delay_potenciometro 5
#define max_delay_potenciometro 5000
#define potenciometro_magic_number 1023.0

/*Misc Params*/
#define duracion_delay 100
#define debug true

/*Variables Globales*/
bool laser_encendido;
int valor_laser;
int valor_potenciometro;
int delay_potenciometro;

/*Inicializacion*/
void setup()
{
  laser_encendido = false;
  valor_laser = 1;
  valor_potenciometro = 0;
  delay_potenciometro = 0;
  
  if (debug)
    Serial.begin(9600);
  
  pinMode(pin_disparador, OUTPUT);
  pinMode(pin_laser, OUTPUT);
  pinMode(pin_detector, INPUT); 
}

/*Funciones de Debug*/
void debug_log(String message){
  if (debug)
    Serial.print(message);
}

void debug_log_int(int value){
  if (debug)
    Serial.print(value);
}

void debug_log_ln(String message){
  if (debug)
    Serial.println(message);
}

/*Funciones de Laser*/
void encender_laser(){
  if (laser_encendido)
    return;
  digitalWrite(pin_laser, HIGH);
  laser_encendido = true;
  debug_log_ln("Laser encendido");
}

void apagar_laser(){
  if (!laser_encendido)
    return;
  digitalWrite(pin_laser, LOW);
  laser_encendido = false;
  debug_log_ln("Laser apagado");
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

/*Funciones para Disparar la camara*/
void disparar(){
  debug_log("Disparando...");
  digitalWrite(pin_disparador, HIGH);     //Enciendo Salida
  delay(100);
  digitalWrite(pin_disparador, LOW);     //Apago Salida
  debug_log_ln("Listo");
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

  debug_log("Delay: ");
  debug_log_int(delay_potenciometro);
  debug_log_ln("ms");
}

/*Funciones de Testeo*/
void test_de_componentes(){
  debug_log_ln("Test de Componentes");
  encender_laser();
  delay(1000);
  disparar();
  delay(1000);
  apagar_laser();
  delay(1000);
  delay(5000);
}

void test_potenciometro(){
  if (cambio_estado_potenciometro())
    actualizar_delay_potenciometro();
}

void main_loop(){
  encender_laser();

  if (cambio_estado_potenciometro())
    actualizar_delay_potenciometro();

  if (laser_interrumpido())  {
    apagar_laser();
    delay(delay_potenciometro);
    disparar();
    delay(duracion_delay);
    encender_laser();
  }
}

void loop(){
  main_loop();
}
