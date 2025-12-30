// ---------- PIN SETUP ----------
int phPin = A0;

// Motor driver connections
int basic_pump_in1 = 6;  
int basic_pump_in2 = 7;

int acidic_pump_in3 = 8;  
int acidic_pump_in4 = 9;


// -------- YOUR SENSOR CALIBRATION ----------
// Voltage readings you gave:
// pH 10 (basic) = 1.999 V
// pH 7 (neutral) = 2.44 V
// pH 4 (acidic) = 3.666 V

// Linear interpolation values
float v7 = 2.44;
float v4 = 3.666;
float v10 = 1.999;

// Slopes for conversion (calculated)
float slope_low = (7.0 - 4.0) / (v7 - v4);    // for acidic side
float slope_high = (10.0 - 7.0) / (v10 - v7); // for basic side


void setup() {
  Serial.begin(9600);

  pinMode(basic_pump_in1, OUTPUT);
  pinMode(basic_pump_in2, OUTPUT);
  pinMode(acidic_pump_in3, OUTPUT);
  pinMode(acidic_pump_in4, OUTPUT);

  stopPumps();  // safety
}


void loop() {

  // ---------- READ pH SENSOR ----------
  int raw = analogRead(phPin);
  float voltage = raw * (5.0 / 1023.0);

  float pH;

  // Determine pH based on your voltage ranges
  if (voltage > v7) {
    // Acidic range (pH 7 → 4)
    pH = 7.0 - (voltage - v7) * slope_low;
  } else {
    // Basic range (pH 7 → 10)
    pH = 7.0 + (v7 - voltage) * slope_high;
  }

  
  // ---------- SERIAL OUTPUT ----------
  Serial.print("Voltage: ");
  Serial.print(voltage, 3);
  Serial.print(" V | pH: ");
  Serial.println(pH, 2);


  // ---------- CONTROL LOGIC ----------
  if (pH < 6.5) {
    // Too acidic → turn ON BASIC pump
    Serial.println("ACIDIC detected → Adding BASIC solution...");
    runBasicPump();

  } else if (pH > 7.5) {
    // Too basic → turn ON ACIDIC pump
    Serial.println("BASIC detected → Adding ACIDIC solution...");
    runAcidicPump();

  } else {
    // Neutral
    Serial.println("NEUTRAL → Pumps OFF");
    stopPumps();
  }

  delay(1500);
}



// ---------- PUMP FUNCTIONS ----------
void runBasicPump() {
  digitalWrite(basic_pump_in1, HIGH);
  digitalWrite(basic_pump_in2, LOW);

  digitalWrite(acidic_pump_in3, LOW);
  digitalWrite(acidic_pump_in4, LOW);
}

void runAcidicPump() {
  digitalWrite(acidic_pump_in3, HIGH);
  digitalWrite(acidic_pump_in4, LOW);

  digitalWrite(basic_pump_in1, LOW);
  digitalWrite(basic_pump_in2, LOW);
}

void stopPumps() {
  digitalWrite(basic_pump_in1, LOW);
  digitalWrite(basic_pump_in2, LOW);
  digitalWrite(acidic_pump_in3, LOW);
  digitalWrite(acidic_pump_in4, LOW);
}