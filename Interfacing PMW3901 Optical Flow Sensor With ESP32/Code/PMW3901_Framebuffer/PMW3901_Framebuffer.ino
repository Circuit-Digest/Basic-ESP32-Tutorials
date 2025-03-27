#include <SPI.h>
#include <Bitcraze_PMW3901.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>

// WiFi credentials
const char* ssid = "SSID";
const char* password = "PASSWORD";

// PMW3901 (CS pin on GPIO 5)
Bitcraze_PMW3901 flow(5);

// Web server on port 80
AsyncWebServer server(80);

// Frame buffer (35x35 = 1225 bytes)
char frame[35 * 35];

void setup() {
  Serial.begin(115200);

  // Initialize PMW3901
  if (!flow.begin()) {
    Serial.println("PMW3901 init failed");
    while (1); // halt
  }

  flow.enableFrameBuffer();
  Serial.println("PMW3901 frame buffer enabled");

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  WiFi.setSleep(false); // improves performance
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.print("Connected. IP address: ");
  Serial.println(WiFi.localIP());

  // Serve HTML UI
  server.on("/", HTTP_GET, [](AsyncWebServerRequest* request) {
    String html = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <title>PMW3901 Frame Buffer</title>
  <style>
    canvas {
      display: block;
      margin: auto;
      image-rendering: pixelated;
      width: 350px;
      height: 350px;
      border: 1px solid black;
    }
    body {
      font-family: sans-serif;
      background: #111;
      color: #fff;
      text-align: center;
    }
    #fps {
      margin: 10px;
      font-size: 1.2em;
    }
  </style>
</head>
<body>
  <h1>PMW3901 Frame Buffer</h1>
  <div id="fps">FPS: 0</div>
  <canvas id="frameBuffer" width="35" height="35"></canvas>

  <script>
    const canvas = document.getElementById('frameBuffer');
    const ctx = canvas.getContext('2d');
    const imageData = ctx.createImageData(35, 35);

    // Ironhot pseudo-color palette (33 entries)
    const ironhot = [
      [0, 0, 0], [35, 0, 0], [64, 0, 0], [96, 0, 0], [128, 0, 0], [160, 0, 0],
      [192, 0, 0], [224, 0, 0], [255, 0, 0], [255, 32, 0], [255, 64, 0],
      [255, 96, 0], [255, 128, 0], [255, 160, 0], [255, 192, 0], [255, 224, 0],
      [255, 255, 0], [224, 255, 32], [192, 255, 64], [160, 255, 96],
      [128, 255, 128], [96, 255, 160], [64, 255, 192], [32, 255, 224],
      [0, 255, 255], [0, 224, 255], [0, 192, 255], [0, 160, 255],
      [0, 128, 255], [0, 96, 255], [0, 64, 255], [0, 32, 255], [0, 0, 255]
    ];

    let lastTime = performance.now();

    async function fetchFrameBuffer() {
      try {
        const response = await fetch('/data');
        const buffer = await response.arrayBuffer();
        const data = new Uint8Array(buffer);

        for (let i = 0; i < data.length; i++) {
          const val = data[i];
          const colorIndex = Math.floor((val / 255) * (ironhot.length - 1));
          const [r, g, b] = ironhot[colorIndex];
          const index = i * 4;
          imageData.data[index] = r;
          imageData.data[index + 1] = g;
          imageData.data[index + 2] = b;
          imageData.data[index + 3] = 255;
        }

        ctx.putImageData(imageData, 0, 0);

        const now = performance.now();
        const fps = (1000 / (now - lastTime)).toFixed(1);
        lastTime = now;
        document.getElementById('fps').innerText = `FPS: ${fps}`;

        requestAnimationFrame(fetchFrameBuffer);
      } catch (err) {
        console.error("Fetch error:", err);
        setTimeout(fetchFrameBuffer, 100);
      }
    }

    fetchFrameBuffer();
  </script>
</body>
</html>
    )rawliteral";
    request->send(200, "text/html", html);
  });

  // Binary data response
  server.on("/data", HTTP_GET, [](AsyncWebServerRequest* request) {
    flow.readFrameBuffer(frame);
    request->send_P(200, "application/octet-stream", (uint8_t*)frame, sizeof(frame));
  });

  server.begin();
}

void loop() {
  // Nothing here
}
