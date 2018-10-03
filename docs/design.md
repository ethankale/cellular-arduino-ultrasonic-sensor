Design of a DIY Water Depth (Distance) Sensor
=============================================
##Parts List
- Ultrasonic Rangefinder ($85): Maxbotix weather-resistant.  Accurate to +-1 cm – about 0.03', so lower accuracy than a piezometer.  Might be able to compensate for that with some math and higher density of readings.  Adafruit.
- Arduino MKR GSM 1400 ($70): Arduino with built-in 2G and 3G capability.  Must pair with a SIM (Soracom, probably).  Mouser.
- Antenna ($30): This project will require an external antenna with good gain. GF-Link from Amazon.
- Battery Pack 4400 mAh ($20): Lithium Ion rechargeable.  Transmitting data requires at least a 1500 mAh battery pack; this should be more than enough. Adafruit.
- Battery Charger ($18): Needed to charge the battery from solar. Adafruit.
- Solar Panel ($40): So that we don't need a giant battery, or need to keep swapping out batteries over time. Adafruit.
- SD Board and Card ($13): The smallest Micro SD card we can order is 8 GB for $5, so this would be plenty for essentially infinite monitoring. Adafruit.
- Enclosure ($20): This is approximate.  Would need something like https://www.polycase.com/ml-45f-15.  Alternatively, a round pipe that could fit in a 2" casing and seal would work.

##Cost
The above parts together cost $296.  Adding an extra $30 for miscellaneous wires, screws cable glands, etc. and 10% for sales tax, and the total estimated cost is about $359.
Compare to total cost of Ott Ecolog 800 3G ($2,990) or In-Situ Tube 300 + Rugged Troll 200 ($2,400) – one-seventh to one-eight the cost, or savings per unit of between $2,040 and $2,630 (not including the cost of custom read-out hardware for the Ott and In-Situ solutions, usually ~$300/per user).

##Layout
These are initial thoughts – the final layout will have to be subject to change.
The whole unit should fit in 1.5" PVC, which will slide vertically into a 2" stilling well.  A 1.5" to .75" reducer in the bottom end will fit the ultrasonic rangefinder.  The top will have a removable cap, with two cable glands for the solar panel and the antenna.  The SIM will be directly accessible once the cap is removed.
A challenge with the pipe layout will be capping the top in a way that still allows it to fit in a 2" stilling well.  It will also be a challenge to connect antenna and solar wires through the cap, while still allowing the cap to be removed without damaging them.
