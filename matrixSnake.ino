#include <FastLED.h>
#include <GyverPortal.h>

// получаем AJAX клики со страницы и значения с компонентов
#define AP_SSID "Midnight"
#define AP_PASS "0686511080"
#define ROW_LENGTH 16
#define ROW_HEIGHT 16
// For led chips like Neopixels, which have a data line, ground, and power, you just
// need to define DATA_PIN.  For led chipsets that are SPI based (four wires - data, clock,
// ground, and power), like the LPD8806 define both DATA_PIN and CLOCK_PIN
#define DATA_PIN 2

GyverPortal portal;
// Define the array of leds
CRGB leds[ROW_LENGTH * ROW_HEIGHT];

int currentPosition[2] = { 5, 5 };
int previousPosition[2] = { currentPosition[0], currentPosition[1] };

// конструктор страницы
void build() {
  BUILD_BEGIN();
  GP.THEME(GP_DARK);

  GP.TITLE("Matrix Snake", "t1");
  GP.HR();
  GP.BUTTON("left", "⬅");
  GP.BUTTON("up", "⬆");
  GP.BUTTON("right", "➡");
  GP.BUTTON("down", "⬇");

  BUILD_END();
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(AP_SSID, AP_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(WiFi.localIP());

  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, ROW_LENGTH * ROW_HEIGHT);
  FastLED.setBrightness(10);

  for (int row = 0; row < ROW_LENGTH; row++) {
    for (int i = 0; i < ROW_HEIGHT; i++) {
      leds[getLedIndex(row, i)] = CRGB::Black;
    }
  }
  leds[getLedIndex(currentPosition[0], currentPosition[1])] = CRGB::Blue;
  FastLED.show();
  delay(10);

  // подключаем конструктор и запускаем
  portal.attachBuild(build);
  portal.attach(action);
  portal.start();
}

void action() {
  // был клик по компоненту
  if (portal.click()) {

    if (portal.click("left")) {
      if (currentPosition[1] == 0) {
        currentPosition[1] = ROW_LENGTH - 1;
      } else {
        currentPosition[1]--;
      }
    }

    if (portal.click("up")) {
      if (currentPosition[0] == ROW_HEIGHT - 1) {
        currentPosition[0] = 0;
      } else {
        currentPosition[0]++;
      }
    }

    if (portal.click("right")) {
      if (currentPosition[1] == ROW_LENGTH - 1) {
        currentPosition[1] = 0;
      } else {
        currentPosition[1]++;
      }
    }

    if (portal.click("down")) {
      if (currentPosition[0] == 0) {
        currentPosition[0] = ROW_HEIGHT - 1;
      } else {
        currentPosition[0]--;
      }
    }

    leds[getLedIndex(currentPosition[0], currentPosition[1])] = CRGB::Blue;
    FastLED.show();
  }
}

int getLedIndex(int row, int index) {
  if (row % 2 == 0) {
    return row * ROW_LENGTH + index;
  } else {
    return (row + 1) * ROW_LENGTH - index - 1;
  }
}

void loop() {
  portal.tick();
  // for (int row = 0; row < ROW_LENGTH; row++) {
  //   for (int i = 0; i < ROW_HEIGHT; i++) {
  //     if (currentPosition[0] == row && currentPosition[1] == i) {
  //       leds[getLedIndex(row, i)] = CRGB::Blue;
  //     }
  //   }
  // }
  // FastLED.show();
  // delay(10);
}