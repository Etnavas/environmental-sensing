# Garden Sensor

![Inside view of sensor](/images/inside.jpg)
![Outside view of sensor](/images/outside.jpg)
---
![Webpage Image 1](/images/values_1.png)
![Webpage Image 2](/images/values_2.png)
---
## Description
- This is a environmental sensing device made using an Arduino ESP32. It can operate on its own with a power source. Its protective case fits the ESP32, and has room for a small battery. The case helps the device withstand rain and wind.

## Temperature

- The formula used to convert the value obtained by the sensor invloves subtracting 500 and then dividing the difference by 10. This gets the temperature in Celsius, which is then converted into Fahrenheit.

## Light

```
const int darkVal = 0;
const int brightVal = 4100;
```

- 0 is the lowest possible value.

- 4100 is slightly above the greatest value observed when pointing a flashlight directly at the sensor. This should be redone in sunlight rather than artificial light.

### Light Range:

- light <= 0 | light = 0% (dark)
- light >= 4100 | light = 100% (bright)

## Moisture

```
const int dryVal = 3500;
const int wetVal = 1000;
```

- 3500 is slightly above the lowest value observed when the sensor sat out in the open. This should be redone with dry soil.

- 1000 is slightly below the greatest value observed when my hand was wrapped around the sensor. This should be redone with wet soil.

### Moisture Range:

- moisture >= 3500 | moisture = 0% (dry)
- moisture <= 1000 | moisture = 100% (wet)

## Extra Notes

- The battery seen in the first image does not work.

- The ESP32 uses USB-C.

- Each line in the HTML string ends with '\' because it tells C++ that the string continues.

```
<meta http-equiv='refresh' content='5'/>\
```

- 5 is the page refresh rate in seconds, which you can change for less or more frequent updates

## Additions to Consider
 
- Improve UI.
- Log the dimensions of the space inside the case, and the space the entire sensor takes up (there is a small difference).
- Map the IP address to a human-readable domain name.
- Getting the light sensor to stick out the box by extending its prongs and protecting it from the weather.
- The impact of a case drop may damage the contents inside, so perhaps adding cushions on the corners to soften the fall.

## Contributing

- Fork this repo to build on it.