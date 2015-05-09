# analog2ppm

simple analog to ppm converter written in plain C

useful to drive any tx through the trainer port using any analog source (hacked cheap toy remote etc)

* fetch a arduino pro mini from ebay/banggood for 2 Eur (don't worry, we are NOT going to use the arduino software...)
* connect the ppm output (PORTB.2 or arduino(10)) to your standard radio (trainer port input)
* connect trim pots to the arduino using A0-A7
* call make program (connect arduino pro min with an ftdi cable)
* have fun ;)

# pinout

PPM output: 
PORTB.2 (labeled 10)


