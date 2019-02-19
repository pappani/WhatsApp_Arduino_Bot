# WhatsApp Arduino Bot
A remote accessible Python WhatsApp bot, and Arduino sketch to access it via GPRS

# Introduction
This project is a proof of concept.
The goal of this project is to make an Arduino based device capable of sending and receiving messages on WhatsApp.
The project is divided in two parts: Server (Python script running on a PC), and a Client (Arduino with GPRS connection).

# Description
The Server runs a Python script that uses a Selenium bot to interacts with WhatsApp Web, the script is then remotely controlled via simple HTTP requests from the Arduino GPRS.
I bound the server with a DynDNS address to access it from the outside.

The client is an Arduino equipped with a SIM-800L module to connect to the GPRS, a OLED display to print the incoming messages, and a battery to power the device.

As soon as the Arduino is powered on it connects to the GPRS operator, and then sends a request to the server. The server opens the web browser and displays the QR Code login page of WhatsApp Web.
The user now follow the WhatsApp Web login procedure using his smartphone. After that the Arduino is capable of sending and receiving WhatsApp messages remotely without using the smartphone (it still needs to remain connected to Wi-Fi or 3G).
