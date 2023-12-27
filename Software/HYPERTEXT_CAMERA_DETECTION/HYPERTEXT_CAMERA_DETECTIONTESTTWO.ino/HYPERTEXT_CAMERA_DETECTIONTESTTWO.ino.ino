#include <parinlarp-project-1_inferencing.h>
#include <eloquent_esp32cam.h>
#include <eloquent_esp32cam/edgeimpulse/fomo.h>

using eloq::camera;
using eloq::ei::fomo;

#include <ESP32Servo.h>
Servo servo1;
Servo servo2;
Servo servo3;

const int servo1Pin = 2;
const int servo2Pin = 5;
const int servo3Pin = 12;


void setup() {
  servo1.attach(servo1Pin);
  servo2.attach(servo2Pin);
  servo3.attach(servo3Pin);
  delay(3000);
  Serial.begin(115200);
  Serial.println("__EDGE IMPULSE FOMO (NO-PSRAM)__");

  camera.pinout.aithinker();
  camera.brownout.disable();
  camera.resolution.yolo();
  camera.pixformat.rgb565();

  while (!camera.begin().isOk())
    Serial.println(camera.exception.toString());

  Serial.println("Camera OK");
  Serial.println("Put object in front of camera");
}

void loop() {
  if (!camera.capture().isOk()) {
    Serial.println(camera.exception.toString());
    return;
  }

  if (!fomo.run().isOk()) {
    Serial.println(fomo.exception.toString());
    return;
  }

  Serial.printf(
    "Found %d object(s) in %dms\n",
    fomo.count(),
    fomo.benchmark.millis());

  if (!fomo.foundAnyObject())
    return;

  fomo.forEach([](int i, bbox_t bbox) {
    const char* label = bbox.label.c_str();  // Convert String to const char*

    if (strcmp(label, "lactasoy") == 0 && bbox.proba >= 0.8) {
      delay(3000);
      Serial.println("Hello, WorldLACTASOY!");
      servo3.writeMicroseconds(2000);
      delay(500);
      servo3.writeMicroseconds(1500);
      delay(150);
      servo3.writeMicroseconds(1000);
      delay(150);

    } else if (strcmp(label, "mama") == 0 && bbox.proba >= 0.8) {
      delay(2100);
      Serial.println("Hello, WorldMAMA!");
      servo2.writeMicroseconds(2000);
      delay(500);
      servo2.writeMicroseconds(1500);
      delay(150);
      servo2.writeMicroseconds(1000);
      delay(150);

    } else {
      Serial.printf(
        "#%d) Found %s at (x = %d, y = %d) (size %d x %d). "
        "Proba is %.2f\n",
        i + 1,
        label,
        bbox.x,
        bbox.y,
        bbox.width,
        bbox.height,
        bbox.proba);
    }
  });
}
