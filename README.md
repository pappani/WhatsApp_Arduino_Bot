# WhatsApp Arduino Bot
A remotly accessible Python WhatsApp bot, and an Arduino sketch to access it via GPRS

## Introduction
This project is a proof of concept.
The goal of this project is to make an Arduino based device, capable of sending and receiving messages via WhatsApp.
The project is divided in two parts: Server (Python script running on a PC), and a Client (Arduino with GPRS connection).

## Description
The Server runs a Python script that uses a Selenium bot to interact with WhatsApp Web, the script is then remotely controlled via a REST API from the Arduino GPRS module.
I bound the server to a static address in order to access it from outside.

The client is an Arduino Uno equipped with a SIM-800L module to connect to GPRS network, an OLED display to output the incoming messages, and a battery to power the device.

As soon as the Arduino is powered on, it connects to the GPRS network, and then sends a request to the server. The server then opens a Selenium browser session and displays the QR Code login page of WhatsApp Web.
The user now follows the WhatsApp Web login procedure using his smartphone. After that the Arduino is capable of sending and receiving WhatsApp messages remotely without using the smartphone (it still needs to remain connected to the internet).

## Example
![alt text](https://raw.githubusercontent.com/pappani/WhatsApp_Arduino_Bot/master/oled_out.jpg)
