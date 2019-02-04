#define pin_disparador 8
#define pin_detector A3
#define pin_laser 5
#define pin_led_verde 3
#define duracion_delay 100
#define debug true

bool laser_encendido;
bool led_verde_encendido;
int valor_laser;

void setup()
{
  laser_encendido = false;
  led_verde_encendido = false;
  valor_laser = 1;
  
  if (debug)
    Serial.begin(9600);
  
  pinMode(pin_disparador, OUTPUT);
  pinMode(pin_laser, OUTPUT);
  pinMode(pin_detector, INPUT); 
  pinMode(pin_led_verde, OUTPUT);
}
void debug_log(String message){
  if (debug)
    Serial.print(message);
}
void debug_log_ln(String message){
  if (debug)
    Serial.println(message);
}
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

void encender_led_verde(){
  if (led_verde_encendido)
    return;
  digitalWrite(pin_led_verde, HIGH);
  led_verde_encendido = true;
  debug_log_ln("Led verde encendido");
}

void apagar_led_verde(){
  if (!led_verde_encendido)
    return;
  digitalWrite(pin_led_verde, LOW);
  led_verde_encendido = false;
  debug_log_ln("Led verde apagado");
}

void disparar(){
  debug_log("Disparando...");
  digitalWrite(pin_disparador, HIGH);     //Enciendo Salida
  delay(100);
  digitalWrite(pin_disparador, LOW);     //Apago Salida
  debug_log_ln("Listo");
}

void test_de_componentes(){
  debug_log_ln("Test de Componentes");
  encender_laser();
  delay(1000);
  encender_led_verde();
  delay(1000);
  disparar();
  delay(1000);
  apagar_led_verde();
  delay(1000);
  apagar_laser();
  delay(1000);
  delay(5000);
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
void loop()
{
  
  encender_laser();
  encender_led_verde();

  if (laser_interrumpido())
  {
    apagar_led_verde();
    apagar_laser();
    disparar();
    delay(duracion_delay);
    encender_laser();
    encender_led_verde();
  }
}
