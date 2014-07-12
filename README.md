TDOG
====

TDOG, antenna Tracker Diversity and OSD Groundstation

The goal is to create a lightweight ready to use groundstation for the Ardupilot autopilot system (or any other that might use mavlink/minimOSD). 

Features:

- 433/915 or in my case 868 telemetry

- MinimOSD overlay

- 2x RX5808 modules in a diversity config.

- Bluetooth link

- 5.8GHz TX module to send the video on a different channel to my goggles...



Planed features:

- Atmega 1284 or 2560 replacment for the 328 used by minimOSD

- Switch-mode power supply&filtering, battery monitoring 

- Shielding of the analog video parts and of the telemetry module & metal casing 

- Automated channel switching (DJI for example lets you change your channels in-flight)

- Serial links (GPS, ADS-B/FLARM receivers etc.)

- Exposed pins/connectors for more internal&external expansions


Atltough the system is working as such there is a lot to do in terms of adding the extra features, prototyping a custom pcb and figuring out the placement of all the rf equip. and antennas to minimize interferences. 


I it is based on many open source projects, most notably the minimOSD, mavlink and ardupilot and I`d like to thank all the devloppers and contributers of this open source projects for their work!
