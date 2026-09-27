#include <Wire.h>
#include <MPU6050.h>
#include <LiquidCrystal_I2C.h>
#include <WiFi.h>
#include <ESP_Mail_Client.h>

// WiFi
#define WIFI_SSID "WIFI_SSID"
#define WIFI_PASS "WIFI_PASSWORD"

// Email
#define SMTP_HOST "smtp.gmail.com"
#define SMTP_PORT 465

#define AUTHOR_EMAIL "SENDER_EMAIL"
#define AUTHOR_PASS "GMAIL_APP_PASSWORD"
#define RECIPIENT_EMAIL "RECIPIENT_EMAIL"

SMTPSession smtp;
SMTP_Message message;

// Pins
#define VIB_SENSOR 34
#define BUTTON_PIN 25

MPU6050 mpu;
LiquidCrystal_I2C lcd(0x27, 16, 2);

int16_t prev_ax = 0, prev_ay = 0, prev_az = 0;

bool alertActive = false;
unsigned long vibTime = 0;

void setup() {

  Serial.begin(115200);

  pinMode(VIB_SENSOR, INPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  Wire.begin(21,22);

  lcd.begin();
  lcd.backlight();

  mpu.initialize();

  // WiFi
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi Connected");

  lcd.print("System Ready");
  delay(2000);
  lcd.clear();
}

// -------- EMAIL FUNCTION --------
void sendEmail()
{
  Serial.println("Sending Email...");

  smtp.debug(1);

  ESP_Mail_Session session;
  session.server.host_name = SMTP_HOST;
  session.server.port = SMTP_PORT;
  session.login.email = AUTHOR_EMAIL;
  session.login.password = AUTHOR_PASS;

  message.sender.name = "ESP32 Alert";
  message.sender.email = AUTHOR_EMAIL;
  message.subject = "VIBRATION ALERT!";
  message.addRecipient("User", RECIPIENT_EMAIL);

  message.text.content = "Real vibration detected! Please check immediately.";

  if (!smtp.connect(&session))
    return;

  if (!MailClient.sendMail(&smtp, &message))
    Serial.println("Error sending Email");
  else
    Serial.println("Email Sent Successfully!");
}

void loop() {

  int vibration = digitalRead(VIB_SENSOR);

  int16_t ax, ay, az;
  int16_t gx, gy, gz;

  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

  int diff = abs(ax - prev_ax) + abs(ay - prev_ay) + abs(az - prev_az);

  String status = (diff > 500) ? "UNSTABLE" : "STABLE";

  prev_ax = ax;
  prev_ay = ay;
  prev_az = az;

  // -------- VIBRATION DETECTED --------
  if (vibration == 1 && !alertActive)
  {
    vibTime = millis();   // start timer
    alertActive = true;

    lcd.clear();
    lcd.print("Vibration!");
  }

  // -------- WAIT 10 SEC --------
  if (alertActive)
  {
    lcd.setCursor(0,1);
    lcd.print("Wait 10 sec...");

    // If button pressed → cancel alert
    if (digitalRead(BUTTON_PIN) == LOW)
    {
      alertActive = false;
      lcd.clear();
      lcd.print("False Alarm");
      delay(2000);
    }

    // After 10 sec → send email
    if (millis() - vibTime > 10000)
    {
      lcd.clear();
      lcd.print("REAL ALERT!");

      sendEmail();

      delay(3000);
      alertActive = false;
    }
  }

  // -------- NORMAL DISPLAY --------
  if (!alertActive)
  {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("VIB:");
    lcd.print(vibration);

    lcd.setCursor(0,1);
    lcd.print("MPU:");
    lcd.print(status);
  }

  delay(500);
}
