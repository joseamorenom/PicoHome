// Definición de pines
#define IN1 2
#define IN2 3
#define IN3 4
#define IN4 5
#define SENSOR_PIN 6

// Definición de variables
int paso = 0;          // Variable para almacenar el estado actual del paso del motor
int pasosPorGiro = 4096; // El motor 28BYJ-48 tiene 4096 pasos por revolución
bool sentidoHorario = true; // Variable para almacenar el sentido de rotación actual del motor

void setup() {
  // Configurar pines como salida
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  // Configurar pin del sensor como entrada con resistencia pull-up
  pinMode(SENSOR_PIN, INPUT_PULLUP);
}

void loop() {
  // Girar en el sentido actual
  if (sentidoHorario) {
    giroHorario();
  } else {
    giroAntihorario();
  }

  // Verificar si se presionó el sensor final de carrera
  if (digitalRead(SENSOR_PIN) == LOW) {
    // Cambiar el sentido de rotación
    sentidoHorario = !sentidoHorario;
    delay(1000); // Retraso para evitar rebotes en el sensor
  }

  delay(5); // Pequeño retraso entre pasos
}

// Función para girar el motor en sentido horario
void giroHorario() {
  switch (paso) {
    case 0:
      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, LOW);
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, LOW);
      break;
    case 1:
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, HIGH);
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, LOW);
      break;

    case 2:
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, LOW);
      digitalWrite(IN3, HIGH);
      digitalWrite(IN4, LOW);
      break;
    
    case 3:
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, LOW);
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, HIGH);
      break;
    // Agregar casos para los pasos 2 y 3 según corresponda
    // ...
  }

  paso++;
  if (paso == 4) {
    paso = 0; // Reiniciar el contador de pasos
  }
}

// Función para girar el motor en sentido antihorario
void giroAntihorario() {
  switch (paso) {
    case 0:
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, LOW);
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, HIGH);
      break;
    case 1:
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, LOW);
      digitalWrite(IN3, HIGH);
      digitalWrite(IN4, LOW);
      break;

    case 3:
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, LOW);
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, HIGH);
      break;

    case 4:
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, LOW);
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, HIGH);
      break;
      
  }

  paso--;
  if (paso == -1) {
    paso = 3; // Reiniciar el contador de pasos
  }
}
