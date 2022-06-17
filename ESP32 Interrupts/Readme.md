
# [ESP32 Interrupts]([https://circuitdigest.com/microcontroller-projects/](https://circuitdigest.com/microcontroller-projects/esp32-interrupt))

<img src="https://github.com/Circuit-Digest/Basic-ESP32-Tutorials/blob/eb11fe6eee371c00dee686f5862c4f9329251140/ESP32%20Interrupts/Tittle.png" width="" alt="alt_text" title="image_tooltip">
<br>

<br>
<a href="https://circuitdigest.com/tags/ESP32"><img src="https://img.shields.io/static/v1?label=&labelColor=505050&message=ESP32 Tutorials Circuit Digest&color=%230076D6&style=social&logo=google-chrome&logoColor=%230076D6" alt="circuitdigest"/></a>
<br>

[<h1>Click here](https://circuitdigest.com/tags/ESP32) For other ESP32 tutorials.</h1>


<br>
<br>
Interrupts are used to handle events that do not happen during the normal execution of a program but when a specific trigger occurs.  For example, if we write a program to blink an LED the microcontroller will execute each command one by one. But if we want to monitor a switch to start or stop the blinking, it will be only possible after all other tasks before the checking is completed. Ie., it won’t be real-time. That’s where interrupts come to play. With interrupt, we do not need to continuously check the state of the digital input pin. When an interrupt occurs, the controller stops the execution of the main program, and a function is called upon known as ISR or the Interrupt Service Routine. The controller then executes the tasks inside the ISR and then gets back to the main program after the ISR execution is finished.
The ESP32 has a total of 32 interrupts for it’s each core. Each interrupt has a certain priority level, most (but not all) interrupts are connected to the interrupt mux. Because there are more interrupt sources than interrupts, some interrupts are shared with multiple interrupt sources.

<br>
[Note: As this projects are very simple we are only providing the code, schemaitic, and a few essential images if you want to get the images or code explanations do check out the Circuit Digest website.
<br>
<br>
