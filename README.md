# IoT_PlantMonitoring
This project aims to monitor a plant station using various hardware components and software tools. The system collects, processes, and displays data from the plant station to provide insights and ensure optimal plant health.
## Overview
The Plant Station Monitoring System utilizes a combination of hardware and software to monitor environmental parameters of a plant station. The main components include:
- Microbial Fuel Cell (MFC)
- Nucleo STM32
- AD7606 Analog-to-Digital Converter communicating with Nucleo STM32 via SPI
- Raspberry Pi, serving as a gateway and communicating with Nucleo STM32 via UART

Data collected from the sensors is processed and displayed using the following software tools:
- Node-RED for data flow orchestration
- InfluxDB for time-series data storage
- Grafana for data visualization

Here is a diagram illustrating the system architecture:
![Plant Station Monitoring System Diagram](https://github.com/GiuliaTitton/IoT_PlantMonitoring/blob/main/datasheet/Pictures/generalFlow.jpg)
